// Shift register 1
#define shift1_data 5
#define shift1_latch 6
#define shift1_clock 7

// Step motor
#define stepper_in1 8
#define stepper_in2 9
#define stepper_in3 10
#define stepper_in4 11
int stepper_step = 0;
int stepper_steps_left = 0;
int stepper_delay = 1000;
boolean stepper_direction = true;

// Button
#define button_in 12

void setup()
{
  Serial.begin(115200);
  pinMode(shift1_data, OUTPUT);
  pinMode(shift1_latch, OUTPUT);
  pinMode(shift1_clock, OUTPUT);
  pinMode(button_in, INPUT);
  write_shift1(0);
}

void loop()
{
  if (digitalRead(button_in) == HIGH)
  {
    stepper_direction = !stepper_direction;
    stepper_steps_left = 4095;
  }

  while (stepper_steps_left > 0)
  {
    delayMicroseconds(stepper_delay);
    stepper_run();
    stepper_steps_left--;
    Serial.println(stepper_steps_left);
  }
}

void stepper_run()
{
  switch (stepper_step)
  {
  case 0: // 00000001
    stepper_make_step(1);
    break;
  case 1: // 00000011
    stepper_make_step(3);
    break;
  case 2: // 00000010
    stepper_make_step(2);
    break;
  case 3: // 00000110
    stepper_make_step(6);
    break;
  case 4: // 00000100
    stepper_make_step(4);
    break;
  case 5: // 00001100
    stepper_make_step(12);
    break;
  case 6: // 00001000
    stepper_make_step(8);
    break;
  case 7: // 00001001
    stepper_make_step(9);
    break;
  default:
    stepper_make_step(0);
    break;
  }
  stepper_set_direction();
}

void stepper_set_direction()
{
  if (stepper_direction == true)
  {
    stepper_step++;
  }
  if (stepper_direction == false)
  {
    stepper_step--;
  }
  if (stepper_step > 7)
  {
    stepper_step = 0;
  }
  if (stepper_step < 0)
  {
    stepper_step = 7;
  }
}

void stepper_make_step(int steps)
{
  int stepper_out1 = bitRead(steps, 0);
  int stepper_out2 = bitRead(steps, 1);
  int stepper_out3 = bitRead(steps, 2);
  int stepper_out4 = bitRead(steps, 3);
  digitalWrite(stepper_in1, stepper_out1);
  digitalWrite(stepper_in2, stepper_out2);
  digitalWrite(stepper_in3, stepper_out3);
  digitalWrite(stepper_in4, stepper_out4);
}

void write_shift1(int shift)
{
  digitalWrite(shift1_latch, LOW);
  shiftOut(shift1_data, shift1_clock, LSBFIRST, shift);
  digitalWrite(shift1_latch, HIGH);
}
