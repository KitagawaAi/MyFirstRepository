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

  if(buttonState == HIGH && !ButtonPressed){
    ledcolor = ledcolor + 1;
    ButtonPressed =true;
    //delay(100);
  }

  if(buttonState == LOW && ButtonPressed){
    ButtonPressed = false;
  }

  if(ledcolor == 0){
  digitalWrite(RledPin, HIGH);
  digitalWrite(GledPin, HIGH);
  digitalWrite(BledPin, HIGH); 
  }
  else if(ledcolor == 1){
  //RED
  digitalWrite(RledPin, LOW);
  digitalWrite(GledPin, HIGH);
  digitalWrite(BledPin, HIGH); 
  }
  else if(ledcolor == 2){
  //GREEN
  digitalWrite(RledPin, HIGH);
  digitalWrite(GledPin, LOW);
  digitalWrite(BledPin, HIGH); 
  }
  else if(ledcolor == 3){
  //BLUE
  digitalWrite(RledPin, HIGH);
  digitalWrite(GledPin, HIGH);
  digitalWrite(BledPin, LOW); 
  }
  else if(ledcolor == 4){
  digitalWrite(RledPin, LOW);
  digitalWrite(GledPin, LOW);
  digitalWrite(BledPin, HIGH); 
  }
  //purple
  else if(ledcolor == 5){
  digitalWrite(RledPin, LOW);
  digitalWrite(GledPin, HIGH);
  digitalWrite(BledPin, LOW); 
  }
  //cyan
  else if(ledcolor == 6){
  digitalWrite(RledPin, HIGH);
  digitalWrite(GledPin, LOW);
  digitalWrite(BledPin, LOW); 
  }
  //white
  else if(ledcolor == 7){
  digitalWrite(RledPin, LOW);
  digitalWrite(GledPin, LOW);
  digitalWrite(BledPin, LOW); 
  }
  else if(ledcolor == 8){
  ledcolor = 0;
  }
}