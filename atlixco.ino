// Shift register 1
#define shift1_data 4
#define shift1_latch 5
#define shift1_clock 6
  
// Step motor
#define stepper_in1 0 
#define stepper_in2 1 
#define stepper_in3 2 
#define stepper_in4 3 
int stepper_step=0; 
int stepper_steps_left=0; 
int stepper_delay=800;
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
    Serial.println(stepper_steps_left);
  }
} 

void stepper_run() { 
  switch(stepper_step) { 
    case 0: // 00000001
    write_shift1(1);
    break; 
    case 1: // 00000011
    write_shift1(3);
    break; 
    case 2: // 00000010
    write_shift1(2);
    break; 
    case 3: // 00000110
    write_shift1(6);
    break; 
    case 4: // 00000100
    write_shift1(4);
    break; 
    case 5: // 00001100
    write_shift1(12);
    break; 
    case 6: // 00001000
    write_shift1(8);
    break; 
    case 7: // 00001001
    write_shift1(9);
    break; 
    default: 
    write_shift1(0);
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

void write_shift1(int shift) {
  digitalWrite(shift1_latch, LOW);
  shiftOut(shift1_data, shift1_clock, LSBFIRST, shift);
  digitalWrite(shift1_latch, HIGH);
}
