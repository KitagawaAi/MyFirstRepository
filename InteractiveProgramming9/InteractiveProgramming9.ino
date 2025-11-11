const int PIN_BTN = 2;   
const int PIN_LED = 3;   

const unsigned long DEBOUNCE_MS = 25;

const unsigned long BLINK_SLOW_MS = 800;
const unsigned long BLINK_MID_MS  = 400;
const unsigned long BLINK_FAST_MS = 150;

int mode = 0;  

bool stableBtn = HIGH;
unsigned long lastEdgeMs = 0;

unsigned long lastBlinkMs = 0;
bool ledOn = false;     

void setLed(bool on) {
  if (on) digitalWrite(PIN_LED, LOW);  
  else    digitalWrite(PIN_LED, HIGH);
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BTN, INPUT_PULLUP);

  setLed(true);  
}

void loop() {
  unsigned long nowMs = millis();

  bool raw = digitalRead(PIN_BTN);
  static bool lastRaw = raw;

  if (raw != lastRaw) {
    lastEdgeMs = nowMs;
  }

  if (nowMs - lastEdgeMs > DEBOUNCE_MS) {
    if (raw != stableBtn) {
      stableBtn = raw;

      if (stableBtn == LOW) {  
        mode++;
        if (mode > 3) mode = 0;  

        if (mode == 0) {
          setLed(true); 
        } else {
          ledOn = false;
          setLed(false);
          lastBlinkMs = nowMs;
        }
      }
    }
  }
  lastRaw = raw;

  switch (mode) {
    case 0: // 恆亮
      setLed(true);
      break;

    case 1: // 慢閃
      if (nowMs - lastBlinkMs >= BLINK_SLOW_MS) {
        ledOn = !ledOn;
        setLed(ledOn);
        lastBlinkMs = nowMs;
      }
      break;

    case 2: // 中閃
      if (nowMs - lastBlinkMs >= BLINK_MID_MS) {
        ledOn = !ledOn;
        setLed(ledOn);
        lastBlinkMs = nowMs;
      }
      break;

    case 3: // 快閃
      if (nowMs - lastBlinkMs >= BLINK_FAST_MS) {
        ledOn = !ledOn;
        setLed(ledOn);
        lastBlinkMs = nowMs;
      }
      break;
  }
}
