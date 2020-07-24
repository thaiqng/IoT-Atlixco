// Shift register 1
#define shift1_data 4
#define shift1_latch 5
#define shift1_clock 6
byte shift1=0;

// Step motor
#define stepper_in1 0 
#define stepper_in2 1 
#define stepper_in3 2 
#define stepper_in4 3 
int stepper_step=0; 
int stepper_steps_left=0; 
int stepper_delay=1000;
boolean stepper_direction=true;

// Button
#define button_in 12

void setup() { 
  Serial.begin(115200); 
  pinMode(shift1_data, OUTPUT); 
  pinMode(shift1_latch, OUTPUT); 
  pinMode(shift1_clock, OUTPUT); 
  pinMode(button_in, INPUT);
} 

void loop() {
  if (digitalRead(button_in) == HIGH) {
    stepper_direction=!stepper_direction; 
    stepper_steps_left=4095; 
  }
  
  while(stepper_steps_left>0) { 
    delayMicroseconds(stepper_delay);
    stepper_run(); 
    stepper_steps_left--; 
    //Serial.println(stepper_steps_left);
  }
} 

void stepper_run() { 
  switch(stepper_step) { 
    case 0: 
    bitWrite(shift1, stepper_in1, 0); 
    bitWrite(shift1, stepper_in2, 0); 
    bitWrite(shift1, stepper_in3, 0); 
    bitWrite(shift1, stepper_in4, 1); 
    write_shift1();
    break; 
    case 1: 
    bitWrite(shift1, stepper_in1, 0); 
    bitWrite(shift1, stepper_in2, 0); 
    bitWrite(shift1, stepper_in3, 1); 
    bitWrite(shift1, stepper_in4, 1); 
    write_shift1();
    break; 
    case 2: 
    bitWrite(shift1, stepper_in1, 0); 
    bitWrite(shift1, stepper_in2, 0); 
    bitWrite(shift1, stepper_in3, 1); 
    bitWrite(shift1, stepper_in4, 0); 
    write_shift1();
    break; 
    case 3: 
    bitWrite(shift1, stepper_in1, 0); 
    bitWrite(shift1, stepper_in2, 1); 
    bitWrite(shift1, stepper_in3, 1); 
    bitWrite(shift1, stepper_in4, 0); 
    write_shift1();
    break; 
    case 4: 
    bitWrite(shift1, stepper_in1, 0); 
    bitWrite(shift1, stepper_in2, 1); 
    bitWrite(shift1, stepper_in3, 0); 
    bitWrite(shift1, stepper_in4, 0); 
    write_shift1();
    break; 
    case 5: 
    bitWrite(shift1, stepper_in1, 1); 
    bitWrite(shift1, stepper_in2, 1); 
    bitWrite(shift1, stepper_in3, 0); 
    bitWrite(shift1, stepper_in4, 0); 
    write_shift1();
    break; 
    case 6: 
    bitWrite(shift1, stepper_in1, 1); 
    bitWrite(shift1, stepper_in2, 0); 
    bitWrite(shift1, stepper_in3, 0); 
    bitWrite(shift1, stepper_in4, 0); 
    write_shift1();
    break; 
    case 7: 
    bitWrite(shift1, stepper_in1, 1); 
    bitWrite(shift1, stepper_in2, 0); 
    bitWrite(shift1, stepper_in3, 0); 
    bitWrite(shift1, stepper_in4, 1); 
    write_shift1();
    break; 
    default: 
    shift1=0;
    write_shift1();
    break; 
  } 
  stepper_set_direction(); 
} 

void stepper_set_direction() { 
  if(stepper_direction==true) { 
    stepper_step++;
  } 
  if(stepper_direction==false) { 
    stepper_step--; 
  } 
  if(stepper_step>7) {
    stepper_step=0;
  } 
  if(stepper_step<0) {
    stepper_step=7; 
  } 
}

void write_shift1() {
  digitalWrite(shift1_latch, LOW);
  shiftOut(shift1_data, shift1_clock, LSBFIRST, shift1);
  digitalWrite(shift1_latch, HIGH);
}
