const int PIN_R = 3;
const int PIN_G = 6;
const int PIN_B = 5;

const bool COMMON_ANODE = false;

const unsigned long SHOW_PERIOD_MS = 15000UL;

void setup() {
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
}

void setRGB(uint8_t r, uint8_t g, uint8_t b) {
  if (COMMON_ANODE) {
    analogWrite(PIN_R, 255 - r);
    analogWrite(PIN_G, 255 - g);
    analogWrite(PIN_B, 255 - b);
  } else {
    analogWrite(PIN_R, r);
    analogWrite(PIN_G, g);
    analogWrite(PIN_B, b);
  }
}

void setHSV(float h, float s, float v) {
  int i = int(h * 6);
  float f = h * 6 - i;
  float p = v * (1 - s);
  float q = v * (1 - f * s);
  float t = v * (1 - (1 - f) * s);

  float r, g, b;
  switch (i % 6) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    default: r = v; g = p; b = q; break;
  }

  setRGB((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255));
}

float ease(float t) {
  if (t < 0) t = 0;
  if (t > 1) t = 1;
  return 0.5f * (1.0f - cos(PI * t));
}

void loop() {
  unsigned long now = millis();
  unsigned long t = now % SHOW_PERIOD_MS;  

  if (t < 2000) {
    if ((t % 200) < 100) {
      setRGB(255, 255, 255);  
    } else {
      setRGB(20, 20, 20);     
    }
  }

  else if (t < 5000) {
    unsigned long tt = t - 2000;      
    float p = (float)tt / 3000.0f;   
    float e = ease(p);                

    uint8_t startR = 255, startG = 255, startB = 255;

    float hr = 0.0f, sr = 1.0f, vr = 1.0f;
    float r, g, b;
    {
      int i = int(hr * 6);
      float f = hr * 6 - i;
      float p0 = vr * (1 - sr);
      float q0 = vr * (1 - f * sr);
      float t0 = vr * (1 - (1 - f) * sr);
      switch (i % 6) {
        case 0: r = vr; g = t0; b = p0; break;
        case 1: r = q0; g = vr; b = p0; break;
        case 2: r = p0; g = vr; b = t0; break;
        case 3: r = p0; g = q0; b = vr; break;
        case 4: r = t0; g = p0; b = vr; break;
        default: r = vr; g = p0; b = q0; break;
      }
    }
    uint8_t endR = (uint8_t)(r * 255);
    uint8_t endG = (uint8_t)(g * 255);
    uint8_t endB = (uint8_t)(b * 255);

    uint8_t outR = startR + (uint8_t)((endR - startR) * e);
    uint8_t outG = startG + (uint8_t)((endG - startG) * e);
    uint8_t outB = startB + (uint8_t)((endB - startB) * e);

    setRGB(outR, outG, outB);
  }

  else {
    unsigned long tt = t - 5000;        
    float progress = (float)tt / 10000.0f;  
    float hue = fmod(progress, 1.0f);      
    setHSV(hue, 1.0, 1.0);                  
  }
}
