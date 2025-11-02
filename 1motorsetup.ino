const int dirPin = 26;
const int stepPin = 25;
const int enablePin = 27;

void setup(){
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, HIGH);
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  delay(2000);
  digitalWrite(dirPin, HIGH);

}

void loop(){
  digitalWrite(stepPin, HIGH);
  delay(5);
  digitalWrite(stepPin, LOW);
  delay(5);
}