// Collin Robotics Club, 10/7/23
// To understand this code, you should probably check out:
// - the Arduino docs on Servo, and
// - Iterating through 2D arrays in C++.
// Yes, you could have written this without arrays, but that's no fun.

#include <Servo.h>

int pins[] = { 0, 1, 2, 3, 4, 5 };  // Each Servo requires a control pin. On the Pico W, we're conveniently using pins 0 through 5.
Servo servos[5];                    // Array of Servos, but initialized to NULL

// This runs when the chip wakes up
void setup() {
    // For each of the SIX servos (and respectively, their pins):
    for (int i = 0; i < 5; i++) {
        servos[i] = Servo();        // Now they're initialized!
        servos[i].attach(pins[i]);  // and bound to their pin
    }
}

// This block runs forever until the chip has no power source
void loop() {
    parallel_sweep();
}

void parallel_sweep() {
    // Sweeps all 6 servos through their range of motion
    for (int pos = 0; pos <= 180; pos += 5) {  // goes from 0 degrees to 180 degrees
        for (int i = 0; i < 5; i++) {
            servos[i].write(pos);
        }
        delay(30);  // Remember, this is 30 milliseconds; 0.03 seconds
                    // Note that the delay is outside the loop that sets the servos, so that we're not waiting for each servo to get to each degree.
    }

    // goes from 180 degrees to 0 degrees; same stuff otherwise.
    // This could be made into a function, feel free to experiment with that :)
    for (int pos = 180; pos >= 0; pos -= 5) {
        for (int i = 0; i < 5; i++) {  // Note that this is HARDCODED to 6 servos!
                                         // It should really be sizeof(servos)/sizeof(servos[0])
                                         // ... or use std::vector<Servo>, like an ArrayList in Java.
            servos[i].write(pos);
        }
        delay(30);
    }
}

// kept for posterity, but goes through each servo's range of motion individually.
// Might be useful for testing it safely. Keeping for posterity anyways.
void one_by_one_sweep() {
    for (int i = 0; i < 5; i++) {
        for (int pos = 0; pos <= 180; pos += 5) {  // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            servos[i].write(pos);  // tell servo to go to position in variable 'pos'
            delay(30);             // waits 15ms for the servo to reach the position
        }
        for (int pos = 180; pos >= 0; pos -= 5) {  // goes from 180 degrees to 0 degrees
            servos[i].write(pos);                    // tell servo to go to position in variable 'pos'
            delay(30);                               // waits 15ms for the servo to reach the position
        }
    }
}
