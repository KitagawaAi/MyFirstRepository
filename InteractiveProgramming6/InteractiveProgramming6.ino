const int PIN_BTN = 2;
const int PIN_R   = 3; 
const int PIN_G   = 6; 
const int PIN_B   = 5;

const bool COMMON_ANODE = false;

const bool GREEN_PWM = true; 

const unsigned long DEBOUNCE_MS     = 25;
const unsigned long LONG_PRESS_MS   = 2000; 
const unsigned long HAPPY_HOLD_MS   = 3000; 
const unsigned long SUPER_HAPPY_MS  = 4000; 
const unsigned long SAD_TIMEOUT_MS  = 3000; 

const unsigned long RAINBOW_MS      = 900;   
const unsigned long SAD_GRAD_MS     = 2200;  

enum Mood { SUPER_HAPPY, HAPPY, IDLE, SAD };
Mood mood = IDLE;

bool stableBtn = HIGH;  
bool lastStableBtn = HIGH;
unsigned long lastEdgeMs = 0;
bool pressing = false;
unsigned long pressStartMs = 0;

unsigned long nowMs = 0;
unsigned long lastInteractionMs = 0;
unsigned long happyUntilMs = 0;
unsigned long superHappyUntilMs = 0;

inline bool isPwmPin(int pin){
  return (pin==3 || pin==5 || pin==6 || pin==9 || pin==10 || pin==11);
}

void writeChannel(int pin, int value, bool pwmCapable){
  value = constrain(value, 0, 255);

  if (!pwmCapable){
    bool on = (value > 0);
    if (COMMON_ANODE) digitalWrite(pin, on ? LOW : HIGH); 
    else              digitalWrite(pin, on ? HIGH : LOW);  
    return;
  }
  int out = COMMON_ANODE ? (255 - value) : value;
  analogWrite(pin, out);
}

void setRGB(uint8_t r, uint8_t g, uint8_t b){
  writeChannel(PIN_R, r, true);                           
  writeChannel(PIN_G, g, GREEN_PWM && isPwmPin(PIN_G));   
  writeChannel(PIN_B, b, true);                           
}

void setOnOff(bool rOn, bool gOn, bool bOn){
  setRGB(rOn ? 255 : 0, gOn ? 255 : 0, bOn ? 255 : 0);
}

float easeInOutCos(float t){
  if (t < 0) t = 0; if (t > 1) t = 1;
  return 0.5f * (1.0f - cos(2.0f * PI * t));
}

float pingPong(unsigned long t, unsigned long periodMs){
  float p = fmod((float)t, (float)periodMs) / (float)periodMs; // 0~1
  return (p < 0.5f) ? (p*2.0f) : (2.0f - p*2.0f);
}

void enterHappy(unsigned long t){
  mood = HAPPY;
  happyUntilMs = t + HAPPY_HOLD_MS;
  lastInteractionMs = t;
}

void enterSuperHappy(unsigned long t){
  mood = SUPER_HAPPY;
  superHappyUntilMs = t + SUPER_HAPPY_MS;
  lastInteractionMs = t;
}

void updateMoodByTime(unsigned long t){
  if (mood == SUPER_HAPPY && t < superHappyUntilMs) return;
  if (mood == HAPPY       && t < happyUntilMs)      return;

  if (t - lastInteractionMs >= SAD_TIMEOUT_MS) mood = SAD;
  else                                         mood = IDLE;
}

void renderSuperHappy(unsigned long t){
  float p = fmod((float)t, (float)RAINBOW_MS) / (float)RAINBOW_MS; 
  uint8_t r=0,g=0,b=0;
  if (p < 1.0f/3.0f){
    float x = p * 3.0f;                
    r = (uint8_t)(255 * (1.0f - x));
    g = (uint8_t)(255 * x);
  } else if (p < 2.0f/3.0f){
    float x = (p - 1.0f/3.0f) * 3.0f;  
    g = (uint8_t)(255 * (1.0f - x));
    b = (uint8_t)(255 * x);
  } else {
    float x = (p - 2.0f/3.0f) * 3.0f;  
    b = (uint8_t)(255 * (1.0f - x));
    r = (uint8_t)(255 * x);
  }
  setRGB(r,g,b);
}

void renderHappy(){
  setOnOff(true, false, false);
}

void renderIdle(){
  setOnOff(false, true, false);
}

void renderSad(unsigned long t){
  float e = easeInOutCos( pingPong(t, SAD_GRAD_MS) ); // 0→1→0 平滑
  int r = (int)(255 * e);
  int g = (int)(255 * e);
  int b = 255;

  if (!GREEN_PWM || !isPwmPin(PIN_G)){
    const int steps = 5;                   
    int bucket = (g * steps) / 256;        
    g = bucket * (255 / (steps - 1));
  }

  setRGB(r, g, b);
}

void setup(){
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_BTN, INPUT_PULLUP);

  setOnOff(false, true, false);
  mood = IDLE;
  lastInteractionMs = millis();
}

void loop(){
  nowMs = millis();

  bool raw = digitalRead(PIN_BTN); 
  static bool lastRaw = raw;
  if (raw != lastRaw) lastEdgeMs = nowMs;
  if (nowMs - lastEdgeMs > DEBOUNCE_MS){
    if (raw != stableBtn){
      stableBtn = raw;

      if (stableBtn == LOW){
        pressing = true;
        pressStartMs = nowMs;
      } else {
        if (pressing){
          unsigned long dur = nowMs - pressStartMs;
          if (dur >= LONG_PRESS_MS) enterSuperHappy(nowMs);
          else                      enterHappy(nowMs);
          pressing = false;
        }
      }
    }
  }
  lastRaw = raw;

  if (pressing && (nowMs - pressStartMs >= LONG_PRESS_MS) && mood != SUPER_HAPPY){
    enterSuperHappy(nowMs);
  }

  updateMoodByTime(nowMs);

  switch (mood){
    case SUPER_HAPPY: renderSuperHappy(nowMs); break; 
    case HAPPY:       renderHappy();           break; 
    case IDLE:        renderIdle();            break; 
    case SAD:         renderSad(nowMs);        break; 
  }
}
