const int buttonPin = 2;
const int RledPin = 3;
const int GledPin = 4;
const int BledPin = 5;

int buttonState = 0;
int ledcolor = 0;
bool ButtonPressed = false;

void setup() {
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH && !ButtonPressed) {
    ledcolor++;
    ButtonPressed = true;
    colorTransitionEffect();
  }

  if (buttonState == LOW && ButtonPressed) {
    ButtonPressed = false;
  }

  if (ledcolor > 7) ledcolor = 0;
  setColorByMode(ledcolor);
}

void setColorByMode(int mode) {
  switch (mode) {
    case 0: setColor(1, 1, 1); break; // off / white dim
    case 1: setColor(0, 1, 1); break; // red
    case 2: setColor(1, 0, 1); break; // green
    case 3: setColor(1, 1, 0); break; // blue
    case 4: setColor(0, 0, 1); break; // yellow
    case 5: setColor(0, 1, 0); break; // purple
    case 6: setColor(1, 0, 0); break; // cyan
    case 7: rainbowEffect();  break;  // 彩虹閃爍模式
  }
}

void setColor(int r, int g, int b) {
  digitalWrite(RledPin, r);
  digitalWrite(GledPin, g);
  digitalWrite(BledPin, b);
}

void rainbowEffect() {
  for (int i = 0; i < 3; i++) {
    setColor(0, 1, 1); delay(150); // red
    setColor(1, 0, 1); delay(150); // green
    setColor(1, 1, 0); delay(150); // blue
    setColor(0, 0, 1); delay(150); // yellow
    setColor(0, 1, 0); delay(150); // purple
    setColor(1, 0, 0); delay(150); // cyan
  }
  setColor(0, 0, 0); // 最後回白光
}

void colorTransitionEffect() {
  for (int i = 0; i < 3; i++) {
    setColor(1, 1, 1); delay(100);
    setColor(0, 0, 0); delay(100);
  }
}
