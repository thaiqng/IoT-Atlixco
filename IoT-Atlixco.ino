// Door lock
const int correct_password[4] = {1, 1, 1, 0};
const int possible_digits[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int input_password[4];
bool door_open = false;

// Button 1
#define button1_in 2
const int button1_timeout = 60000; // Time out in 60000 milliseconds (1 minute)
unsigned int button1_elapsed_time = 0;
int button1_state = -1; // Digit pointer from 0 to 3, -1 as init, 4 as check password

// Button 2
#define button2_in 3
int button2_state[4] = {0, 0, 0, 0}; // To cycle through possible_digits

// Step motor
#define stepper_out1 4
#define stepper_out2 5
#define stepper_out3 6
#define stepper_out4 7
int stepper_in1;
int stepper_in2;
int stepper_in3;
int stepper_in4;
int stepper_step = 0;
int stepper_steps_left = 0;
int stepper_delay = 1000;
boolean stepper_direction = true;

// Shift register 1
#define shift1_data 8
#define shift1_latch 9
#define shift1_clock 10

void setup()
{
  Serial.begin(115200);

  pinMode(button1_in, INPUT);

  pinMode(button2_in, INPUT);

  pinMode(stepper_out1, OUTPUT);
  pinMode(stepper_out2, OUTPUT);
  pinMode(stepper_out3, OUTPUT);
  pinMode(stepper_out4, OUTPUT);

  pinMode(shift1_data, OUTPUT);
  pinMode(shift1_latch, OUTPUT);
  pinMode(shift1_clock, OUTPUT);
}

void loop()
{
  if (door_open == false)
  {
    if (digitalRead(button1_in) == HIGH)
    {
      button1_state++;
      Serial.println("Button 1: " + String(button1_state));
      Serial.println("Button 2: " + String(button2_state[button1_state]));
      delay(500);
    }

    if (button1_state == 0 && button1_elapsed_time == 0)
    {
      button1_elapsed_time = millis();
    }

    if (button1_state == 4)
    {
      button1_elapsed_time = 0;
      Serial.println("Password input: " + String(input_password[0]) + String(input_password[1]) + String(input_password[2]) + String(input_password[3]));

      if (check_password() == true)
      {
        open_door();
      }
      else
      {
        Serial.println("Wrong password");
        buttons_reset(2000);
      }
    }

    if (millis() - button1_elapsed_time >= button1_timeout)
    {
      Serial.println("Password input timeout");
      buttons_reset(2000);
    }

    if ((button1_state != -1 || button1_state != 4) && digitalRead(button2_in) == HIGH)
    {
      if (button2_state[button1_state] == 9)
      {
        button2_state[button1_state] = 0;
        Serial.println("Button 2: " + String(button2_state[button1_state]));
      }
      else
      {
        button2_state[button1_state]++;
        Serial.println("Button 2: " + String(button2_state[button1_state]));
      }
      delay(500);
    }
    input_password[button1_state] = possible_digits[button2_state[button1_state]];
  }
  else if (digitalRead(button1_in) == HIGH)
  {
    lock_door();
  }
}

void stepper_run(bool clockwise)
{
  stepper_direction = !stepper_direction;
  stepper_steps_left = 4095;

  while (stepper_steps_left > 0)
  {
    delayMicroseconds(stepper_delay);
    stepper_choose_step(clockwise);
    stepper_steps_left--;
  }

  if (clockwise == true)
  {
    Serial.println("Door unlocked");
  }
  else
  {
    Serial.println("Door locked");
  }
}

void stepper_choose_step(bool clockwise)
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
  stepper_set_direction(clockwise);
}

void stepper_set_direction(bool clockwise)
{
  if (clockwise == true)
  {
    stepper_step++;
  }
  if (clockwise == false)
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
  stepper_in1 = bitRead(steps, 0);
  stepper_in2 = bitRead(steps, 1);
  stepper_in3 = bitRead(steps, 2);
  stepper_in4 = bitRead(steps, 3);
  digitalWrite(stepper_out1, stepper_in1);
  digitalWrite(stepper_out2, stepper_in2);
  digitalWrite(stepper_out3, stepper_in3);
  digitalWrite(stepper_out4, stepper_in4);
}

void write_shift1(int shift)
{
  // shift = 128•b7 + 64•b6 + 32•b5 + 16•b4 + 8•b3 + 4•b2 + 2•b1 + b0
  digitalWrite(shift1_latch, LOW);
  shiftOut(shift1_data, shift1_clock, LSBFIRST, shift);
  digitalWrite(shift1_latch, HIGH);
}

void buttons_reset(int ms)
{
  button1_state = -1;
  for (int i = 0; i < 4; i++)
  {
    button2_state[i] = 0;
  }
  button1_elapsed_time = 0;
  delay(ms);
}

bool check_password()
{
  for (int n = 0; n < 4; n++)
  {
    if (input_password[n] != correct_password[n])
    {
      return false;
    }
  }

  return true;
}

void open_door()
{
  Serial.println("Unlocking door");
  door_open = true;
  stepper_run(true);
  buttons_reset(500);
}

void lock_door()
{
  Serial.println("Locking door");
  door_open = false;
  stepper_run(false);
  buttons_reset(500);
}