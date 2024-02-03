#include "Servo.h"

const int JOYSTICK_X_PIN = 26;
const int JOYSTICK_Y_PIN = 27;
const int JOYSTICK_SW_PIN = 0;

Servo base, shoulder, elbow, wrist, gripper;
int base_angle = 90, shoulder_angle = 90, elbow_angle = 90, wrist_angle = 90;
bool gripper_closed = false;

void setup() {
  Serial.begin(9600);
	
  base.attach(1);
  shoulder.attach(2);
  elbow.attach(3);
  wrist.attach(4);
  gripper.attach(5);

  analogReadResolution(12);  // Pico has an 12-bit ADC!
  pinMode(JOYSTICK_SW_PIN, INPUT);

  base.write(base_angle);
  shoulder.write(shoulder_angle);
  elbow.write(elbow_angle);
  wrist.write(wrist_angle);
  gripper.write(gripper_closed ? 130 : 50);
}

void loop() {
  double x = get_joystick(JOYSTICK_X_PIN);
  double y = -get_joystick(JOYSTICK_Y_PIN);

	// basically deadzone
  if (abs(x) < 0.05) {
    x = 0;
  }
  if (abs(y) < 0.05) {
    y = 0;
  }

  // Hardcoding some "speed" constants here :)
  base_angle += x * 2.5;
  shoulder_angle += y * 2.5;
  elbow_angle += y * 2.5;
  wrist_angle -= y * 2.5;

  base_angle = constrain(base_angle, 0, 180);
  shoulder_angle = constrain(shoulder_angle, 0, 180);
  elbow_angle = constrain(elbow_angle, 0, 180);
  wrist_angle = constrain(wrist_angle, 0, 180);

	// TODO: error-checking
  base.write(base_angle);
  shoulder.write(shoulder_angle);
  elbow.write(elbow_angle);
  wrist.write(wrist_angle);
  gripper.write(digitalRead(JOYSTICK_SW_PIN) == HIGH ? 140 : 40);

  serial_print_arm();

  delay(20);
}

void move_to(int base_angle, int shoulder_angle, int elbow_angle, int wrist_angle, int gripper_angle) {
  // base_angle = constrain();

  base.write(base_angle);
  shoulder.write(shoulder_angle);
  elbow.write(elbow_angle);
  wrist.write(wrist_angle);
  gripper.write(gripper_angle);
}

// An alternate version of moveTo() that accepts an array is useful.
void move_to_array(int angles[]) {
  // Remember, arrays start indexing at 0.
  move_to(angles[0], angles[1], angles[2], angles[3], angles[4]);
}

void serial_print_arm() {
  Serial.print("base ");
  Serial.print(base_angle);

  Serial.print(", shoulder ");
  Serial.print(shoulder_angle);

  Serial.print(", elbow ");
  Serial.print(elbow_angle);

  Serial.print(", wrist ");
  Serial.print(wrist_angle);

  Serial.print(", gripper ");
  Serial.print(gripper_closed);

  Serial.print(", joy_x ");
  Serial.print(get_joystick(JOYSTICK_X_PIN));

  Serial.print(", joy_y ");
  Serial.print(get_joystick(JOYSTICK_Y_PIN));

  Serial.print(", joy_sw ");
  Serial.println(digitalRead(JOYSTICK_SW_PIN));
}

double get_joystick(int analog_pin) {
  int a = analogRead(analog_pin);
  int analogMax = pow(2, 12);

  return mapf(a, 0, analogMax, -1.0, 1.0);
}

double mapf(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
