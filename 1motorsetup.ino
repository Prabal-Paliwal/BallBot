const int numMotors = 2;
const int dirs[numMotors] = {26, 32};
const int steps[numMotors] = {25, 18};
const int ENs[numMotors] = {27, 33};


void setup(){
  for(int i = 0; i < numMotors; i++){
    pinMode(ENs[i], OUTPUT);
    pinMode(dirs[i], OUTPUT);
    pinMode(steps[i], OUTPUT);
  }
  delay(2000);
}

void loop(){
  if(touchRead(T0) < 50){
    for(int i = 0; i < numMotors; i++){
      digitalWrite(ENs[i], HIGH);
      if(touchRead(T3) < 50){
        digitalWrite(dirs[i], LOW);
      }else{
        digitalWrite(dirs[i], HIGH);
      }
      digitalWrite(steps[i], HIGH);
    }
      delayMicroseconds(5000);
      for(int i = 0; i < numMotors; i++){
      digitalWrite(steps[i], LOW);
      }
      delayMicroseconds(5000);
  }else{
    for(int i = 0; i < numMotors; i++){
      digitalWrite(steps[i], LOW);
      digitalWrite(ENs[i], LOW);
    }
  }
}
