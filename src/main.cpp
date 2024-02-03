#include <Arduino.h>
#include <Servo.h>

// Pin constants are here so it's easy to change them if we re-organize the breadboard
const int BASE_SERVO_PIN = 1;
const int SHOULDER_SERVO_PIN = 2;
const int ELBOW_SERVO_PIN = 3;
const int WRIST_SERVO_PIN = 4;
const int GRIPPER_SERVO_PIN = 5;
const int JOYSTICK_X_PIN = 26;
const int JOYSTICK_Y_PIN = 27;
const int JOYSTICK_SW_PIN = 0;
const int BOARD_LED = 25;

const int ANALOG_READ_RESOLUTION = 12; // Pico has a 12-bit ADC!

Servo base, shoulder, elbow, wrist, gripper;
int base_angle = 90, shoulder_angle = 90, elbow_angle = 90, wrist_angle = 90;
bool gripper_closed = false;

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

double mapf(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * Gets joystick state
 * 
 * @param analog_pin pin number
 * @return either 1, -1, or 0 depending on if the joystick is in the forward, backward, or center positions respectively
 */
double get_joystick(int analog_pin) {
  int value = analogRead(analog_pin);
  
  //Serial.println(value); // DEBUGGING

  int analogMin = pow(2, 0) - 1;
  int analogMax = pow(2, ANALOG_READ_RESOLUTION) - 1;

  // NOTE: Swapping from mapf() to map() has the side effect of the arm simply
  //       moving or not moving, not moving slower/faster depending on joystick
  //       position. that can be fixed later, I just wanna make work first 
  return map(value, analogMin, analogMax, -1.0, 1.0);
}

// This is basically just for debugging
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

void setup() {
  Serial.begin(9600);
	
  base.attach(BASE_SERVO_PIN);
  shoulder.attach(SHOULDER_SERVO_PIN);
  elbow.attach(ELBOW_SERVO_PIN);
  wrist.attach(WRIST_SERVO_PIN);
  gripper.attach(GRIPPER_SERVO_PIN);

  analogReadResolution(ANALOG_READ_RESOLUTION);
  pinMode(JOYSTICK_SW_PIN, INPUT);

  // Light up Pico LED just to show that it has power and the code is running
  pinMode(BOARD_LED, PIN_OUTPUT);
  digitalWrite(BOARD_LED, HIGH);

  base.write(base_angle);
  shoulder.write(shoulder_angle);
  elbow.write(elbow_angle);
  wrist.write(wrist_angle);
  gripper.write(gripper_closed ? 130 : 50);
}

void loop() {
  double x = get_joystick(JOYSTICK_X_PIN);
  double y = -get_joystick(JOYSTICK_Y_PIN);

  serial_print_arm(); // DEBUGGING

  // Hardcoding some "speed" constants here :)
  int new_base_angle = base_angle + (x * 2.5);
  int new_shoulder_angle = shoulder_angle + (y * 2.5);
  int new_elbow_angle = elbow_angle + (y * 2.5);
  int new_wrist_angle = wrist_angle - (y * 2.5);

  // Force the angles to max out at 180 and only go as low as 0
  new_base_angle = constrain(new_base_angle, 0, 180);
  new_shoulder_angle = constrain(new_shoulder_angle, 0, 180);
  new_elbow_angle = constrain(new_elbow_angle, 0, 180);
  new_wrist_angle = constrain(new_wrist_angle, 0, 180);

  // Tell the servos how we want them to move, but only tell them to move if the new
  // value is gonna be different than the current one
  if (new_base_angle != base_angle) {
    base_angle = new_base_angle;
    base.write(base_angle);
  }
  if (new_shoulder_angle != shoulder_angle) {
    shoulder_angle = new_shoulder_angle;
    shoulder.write(shoulder_angle);
  }
  if (new_elbow_angle != elbow_angle) {
    elbow_angle = new_elbow_angle;
    elbow.write(elbow_angle);
  }
  if (new_wrist_angle != wrist_angle) {
    wrist_angle = new_wrist_angle;
    wrist.write(wrist_angle);
  }

  // If joystick button is pressed (HIGH), tell gripper to grip, else un-grip
  if (digitalRead(JOYSTICK_SW_PIN) == HIGH) {
    gripper.write(140);
  } else {
    gripper.write(40);
  }
  
  // wait 100 milliseconds between loops, which makes this code run ~10 times per second
  delay(100);
}