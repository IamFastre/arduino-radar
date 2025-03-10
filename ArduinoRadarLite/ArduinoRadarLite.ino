// Includes the Servo library
#include <Servo.h>.

/* ============================= Configurations ============================= */

// Serial setup
const int SERIAL_PORT = 9600;

// Servo setup
const int SERVO_PIN = 2;

// Servo angle range
const int MIN_ANGLE = 0;
const int MAX_ANGLE = 180;

// Defines Trig and Echo pins of the ultrasonic sensor 1
const int SENSOR1_TRIG = 3;
const int SENSOR1_ECHO = 4;

const int SENSOR2_TRIG = 5;
const int SENSOR2_ECHO = 6;

/* ========================================================================== */

Servo myServo;

void setup() {
  Serial.begin(SERIAL_PORT); // Start serial connection as port `SERIAL_PORT`
  myServo.attach(SERVO_PIN); // Attaches the `SERVO_PIN` to the `myServo` object

  // Set input and output of sensor 1
  pinMode(SENSOR1_TRIG, OUTPUT);
  pinMode(SENSOR1_ECHO, INPUT);

  // Set input and output of sensor 2
  pinMode(SENSOR2_TRIG, OUTPUT);
  pinMode(SENSOR2_ECHO, INPUT);
}

void loop() {
  // Rotates the servo motor from `MIN_ANGLE` to `MAX_ANGLE`
  for (int i = MIN_ANGLE; i <= MAX_ANGLE; i++) {
    myServo.write(i);
    delay(30);

    // Calls a function for calculating the distance
    // measured by the each sensor for each degree
    // then sends the package
    sendPackage(
      i,                                          // The Angle
      updateDistance(SENSOR1_TRIG, SENSOR1_ECHO), // Sensor 1 distance
      updateDistance(SENSOR2_TRIG, SENSOR2_ECHO)  // Sensor 2 distance
    );
  }

  // Rotates the servo motor back from `MAX_ANGLE` to `MIN_ANGLE`
  for (int i = MAX_ANGLE; i > MIN_ANGLE; i--) {
    myServo.write(i);
    delay(30);

    // Calls a function for calculating the distance
    // measured by the each sensor for each degree
    // then sends the package
    sendPackage(
      i,                                          // The Angle
      updateDistance(SENSOR1_TRIG, SENSOR1_ECHO), // Sensor 1 distance
      updateDistance(SENSOR2_TRIG, SENSOR2_ECHO)  // Sensor 2 distance
    );
  }
}

/* ========================================================================== */

float updateDistance(int trig, int echo) {
  digitalWrite(trig, LOW);  // Set trig pin to low to ensure a clean pulse
  delayMicroseconds(2);     // Delay for 2 microseconds
  digitalWrite(trig, HIGH); // Send a 10 microsecond pulse by setting trig pin to high
  delayMicroseconds(10);    // Delay for 10 microseconds
  digitalWrite(trig, LOW);  // Set trig pin back to low

  // Wait for a high pulse in `echo`
  unsigned long pulseDuration = pulseIn(echo, HIGH);

  // Measure the pulse width of the echo pin and calculate the distance value
  return pulseDuration / 58.00; // Formula: (340m/s * 1μs) / 2
}

/* ========================================================================== */

void sendPackage(int angle, int distance1, int distance2) {
  Serial.print(angle);      // Sends the current degree into the Serial Port

  Serial.print(",");        // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  Serial.print(distance1);  // Sends the distance value into the Serial Port

  Serial.print(",");        // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  Serial.print(distance2);  // Sends the distance value into the Serial Port

  Serial.print(".");        // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
}
