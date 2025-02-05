#include <Ultrasonic.h>  // Include library for Ultrasonic sensor
#include <Servo.h>       // Include library for controlling servos
#include <ESP32Servo.h>  // Library for the ESP32 servos
#include <Wire.h>        // Include library for I2C communication
#include "BatteryPower.h" // Custom Freenove library for battery monitoring
#include "TouchSensor.h"  // Custom Freenove library for touch sensor
#include "Buzzer.h"       // Custom Freenove library for buzzer

// Constants and Pin Definitions
#define ULTRASONIC_TRIG_PIN 32
#define ULTRASONIC_ECHO_PIN 12
#define TOUCH_SENSOR_PIN 15
#define BATTERY_PIN 36

// Initialize sensors and components
Ultrasonic ultrasonic(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN);
Servo servo1, servo2;
int ledPin = 2;  // LED indicator

// Thresholds
int batteryThreshold = 3300;  // Example threshold in mV
int obstacleDistance = 20;    // Distance in cm

// Function to check battery level and warn if low
void checkBatteryLevel() {
    int batteryLevel = getBatteryVoltage();
    if (batteryLevel < batteryThreshold) {
        setBuzzer(1000);  // Emit a warning beep
        delay(500);
        setBuzzer(0);     // Stop beep
    }
}

// Function to handle obstacle detection
void avoidObstacle() {
    long distance = ultrasonic.MeasureInCentimeters();
    if (distance > 0 && distance < obstacleDistance) {
        // Move backward
        servo1.write(135);  // Adjust for backward movement
        servo2.write(45);
        delay(1000);
        servo1.write(90);   // Stop
        servo2.write(90);
    }
}

// Function to perform a greeting motion on touch sensor activation
void greetingMotion() {
    if (digitalRead(TOUCH_SENSOR_PIN) == HIGH) {
        for (int i = 0; i < 3; i++) {
            servo1.write(45);    // Wave or nod action
            delay(200);
            servo1.write(135);
            delay(200);
        }
        servo1.write(90);  // Reset position
    }
}

// Function to randomly choose a direction to move
void randomMovement() {
    int randomDirection = random(0, 3);  // Generate random movement choice
    
    if (randomDirection == 0) {  // Move forward
        servo1.write(45);
        servo2.write(135);
    } else if (randomDirection == 1) {  // Turn left
        servo1.write(45);
        servo2.write(45);
    } else if (randomDirection == 2) {  // Turn right
        servo1.write(135);
        servo2.write(135);
    }
    delay(1000);  // Duration of movement
    servo1.write(90);  // Stop movement
    servo2.write(90);
}

void setup() {
    Serial.begin(115200);
    setupBuzzer();
    setupAdc();
    pinMode(TOUCH_SENSOR_PIN, INPUT);
    servo1.attach(14);  // Attach servos to control pins
    servo2.attach(12);
    servo1.write(90);  // Set servos to neutral position
    servo2.write(90);
}

void loop() {
    checkBatteryLevel();   // Check and warn if battery is low
    avoidObstacle();       // Check and avoid obstacle if detected
    greetingMotion();      // Perform greeting if touch sensor is activated
    randomMovement();      // Move in a random direction
    delay(2000);           // Delay between each action
}
