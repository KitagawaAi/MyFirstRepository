

// the setup function runs once when you press reset or power the board
const int pushButton = 2;
const int ledPin = 3;

int buttonState = 0;
int ledState = LOW;
int ledcolor = 0;
bool ButtonPressed = false;
String currentcolor = "led";
unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(pushButton, INPUT);
}

void loop() {
  buttonState = digitalRead(pushButton);
  Serial.print("Current Color: ");
  Serial.println(currentcolor);
  if (buttonState == HIGH && !ButtonPressed){
    ledcolor = ledcolor + 1;
    ButtonPressed = true;
  }
  if (buttonState == LOW && !ButtonPressed){
    ButtonPressed = false;
  }

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  if (ledState == LOW) {
      ledState = HIGH;
    } else {
    ledState = LOW;
    }
  }
}
  
