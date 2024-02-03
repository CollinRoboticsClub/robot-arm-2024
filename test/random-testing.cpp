// Random stuff i was testing, will delete once it's in VCS

#include "Arduino.h"
#include "Servo.h"

const int JOYSTICK_X_PIN = 26;
const int JOYSTICK_Y_PIN = 27;
const int JOYSTICK_SW_PIN = 0;
const int TEST_SERVO_PIN = 1;
const int BOARD_LED = 25;

Servo test;
int test_angle;

bool is_joystick_centered(int x, int y) {
  const int CENTER_X = 0.50;
  const int CENTER_Y = 0.50;
  const int TOLERANCE = 0.05;

  if (x > CENTER_X + TOLERANCE || x < CENTER_X - TOLERANCE) {
    return false;
  } else if (y > CENTER_Y + TOLERANCE || y < CENTER_Y - TOLERANCE) {
    return false;
  } else {
    return true;
  }
}

double mapf(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double get_joystick(int analog_pin) {
  int a = analogRead(analog_pin);
  int analogMax = pow(2, 12);

  return mapf(a, 0, analogMax, -1.0, 1.0);
}

void setup() {
  Serial.begin(9600);
	
  test.attach(TEST_SERVO_PIN);

  analogReadResolution(12);  // Pico has a 12-bit ADC!
  pinMode(JOYSTICK_SW_PIN, PIN_INPUT);
  pinMode(BOARD_LED, PIN_OUTPUT);
  digitalWrite(BOARD_LED, HIGH);

  test.write(0);
}

void loop() {
  double x = get_joystick(JOYSTICK_X_PIN);
  double y = -get_joystick(JOYSTICK_Y_PIN);

  // debugging
  Serial.print("joy_x: ");
  Serial.print(x);
  Serial.print(", joy_y: ");
  Serial.print(y);
  Serial.print(", joy_sw: ");
  Serial.println(digitalRead(JOYSTICK_SW_PIN));


  // NOTE: i think the issue is that the default joystick pos is (-0.50, 0.50) not (0, 0)
  test_angle += x * 2.5;
  test.write(test_angle);

  delay(200);
}