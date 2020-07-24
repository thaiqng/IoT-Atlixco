#define IN1 8 
#define IN2 9 
#define IN3 10 
#define IN4 11 
#define BUTTON 12

int Steps=0; 
boolean Direction=true;
int steps_left=0; 

void setup() { 
  Serial.begin(115200); 
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 
  pinMode(BUTTON, INPUT);
} 

void loop() { 
  if (digitalRead(BUTTON)==HIGH) {
    Direction=!Direction; 
    steps_left=4095; 
  }
  
  while(steps_left>0) { 
    delayMicroseconds(1000);
    stepper(); 
    steps_left--; 
    Serial.println(steps_left);
  }
} 

void stepper() { 
  switch(Steps) { 
    case 0: 
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW); 
    digitalWrite(IN3, LOW); 
    digitalWrite(IN4, HIGH); 
    break; 
    case 1: 
    digitalWrite(IN1, LOW); 
    digitalWrite(IN2, LOW); 
    digitalWrite(IN3, HIGH); 
    digitalWrite(IN4, HIGH); 
    break; 
    case 2: 
    digitalWrite(IN1, LOW); 
    digitalWrite(IN2, LOW); 
    digitalWrite(IN3, HIGH); 
    digitalWrite(IN4, LOW); 
    break; 
    case 3: 
    digitalWrite(IN1, LOW); 
    digitalWrite(IN2, HIGH); 
    digitalWrite(IN3, HIGH); 
    digitalWrite(IN4, LOW); 
    break; 
    case 4: 
    digitalWrite(IN1, LOW); 
    digitalWrite(IN2, HIGH); 
    digitalWrite(IN3, LOW); 
    digitalWrite(IN4, LOW); 
    break; 
    case 5: 
    digitalWrite(IN1, HIGH); 
    digitalWrite(IN2, HIGH); 
    digitalWrite(IN3, LOW); 
    digitalWrite(IN4, LOW); 
    break; 
    case 6: 
    digitalWrite(IN1, HIGH); 
    digitalWrite(IN2, LOW); 
    digitalWrite(IN3, LOW); 
    digitalWrite(IN4, LOW); 
    break; 
    case 7: 
    digitalWrite(IN1, HIGH); 
    digitalWrite(IN2, LOW); 
    digitalWrite(IN3, LOW); 
    digitalWrite(IN4, HIGH); 
    break; 
    default: 
    digitalWrite(IN1, LOW); 
    digitalWrite(IN2, LOW); 
    digitalWrite(IN3, LOW); 
    digitalWrite(IN4, LOW); 
    break; 
  } 
  SetDirection(); 
} 

void SetDirection() { 
  if(Direction==true) { 
    Steps++;
  } 
  if(Direction==false) { 
    Steps--; 
  } 
  if(Steps>7) {
    Steps=0;
  } 
  if(Steps<0) {
    Steps=7; 
  } 
}
