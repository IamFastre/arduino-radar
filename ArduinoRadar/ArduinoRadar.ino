/*
  This code controls a servo motor to rotate an ultrasonic sensor and measure distances. 
  The sensor scans from 15 to 165 degrees and back, calculating the distance to objects 
  and sending the angle and distance data to the serial port. The data can be used in the 
  Processing IDE to visualize a real-time radar.
  
  Board: Arduino Uno R4 (or R3)
  Component: Ultrasonic distance Sensor(HC-SR04)

  Source Code From：https://howtomechatronics.com/projects/arduino-radar-project/
*/

// Includes the Servo library
#include <Servo.h>.

struct sen {
  bool enabled;
  int trig;
  int echo;
};

// Defines Trig and Echo pins of the Ultrasonic Sensor 1
const sen sensor1 = {
  .enabled = true,
  .trig = 11,
  .echo = 12,
};

// Defines Trig and Echo pins of the Ultrasonic Sensor 2
const sen sensor2 = {
  .enabled = false,
  .trig = 13,
  .echo = 14,
};

// Servo setup
Servo myServo; // Creates a servo object for controlling the servo motor
const int servoPin = 10;
// Servo angle range
const int min_angle = 0;
const int max_angle = 180;

// Variables for the duration and the distance
long duration;
int distance;

void setup() {
  if (sensor1.enabled) {
    pinMode(sensor1.trig, OUTPUT);  // Sets the trigPin as an Output
    pinMode(sensor1.echo, INPUT);   // Sets the echoPin as an Input
  }

  if (sensor2.enabled) {
    pinMode(sensor2.trig, OUTPUT);  // Sets the trigPin as an Output
    pinMode(sensor2.echo, INPUT);   // Sets the echoPin as an Input
  }

  Serial.begin(9600);
  myServo.attach(servoPin);  // Defines on which pin is the servo motor attached
}

void loop() {
  // Rotates the servo motor from `min_angle` to `max_angle`
  for (int i = min_angle; i <= max_angle; i++) {
    myServo.write(i);
    delay(30);

    // Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree
    distance = calculateDistance(sensor1.trig, sensor1.echo);
    sendPackage(i, distance);
  }

  // Rotates the servo motor back from `max_angle` to `min_angle`
  for (int i = max_angle; i > min_angle; i--) {
    myServo.write(i);
    delay(30);

    // Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree
    distance = calculateDistance(sensor1.trig, sensor1.echo);
    sendPackage(i, distance);
  }
}

void sendPackage(int angle, int distance) {
  Serial.print(angle);     // Sends the current degree into the Serial Port
  Serial.print(",");       // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  Serial.print(distance);  // Sends the distance value into the Serial Port
  Serial.print(".");       // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  // Serial.print("\n");      // Sends a new line for readability
}

// Function to read the sensor data and calculate the distance
int calculateDistance(int trig, int echo) {
  digitalWrite(trig, LOW);   // Set trig pin to low to ensure a clean pulse
  delayMicroseconds(2);      // Delay for 2 microseconds
  digitalWrite(trig, HIGH);  // Send a 10 microsecond pulse by setting trig pin to high
  delayMicroseconds(10);     // Delay for 10 microseconds
  digitalWrite(trig, LOW);   // Set trig pin back to low

  // Measure the pulse width of the echo pin and calculate the distance value
  float distance = pulseIn(echo, HIGH) / 58.00;  // Formula: (340m/s * 1us) / 2
  return distance;
}