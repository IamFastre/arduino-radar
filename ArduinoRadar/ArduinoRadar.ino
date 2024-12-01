// Includes the Servo library
#include <Servo.h>.

// Struct to carry info about a sensor
struct sen {
  const bool enabled; // Used to define pin modes and for sent packages
  const int trig;     // Sensor trigger pin
  const int echo;     // Sensor echo pin
  int distance;       // The distance the sensor detects

  // Update the value of `distance`
  // Only runs if sensor is enabled
  void updateDistance() {
    if (enabled) {
      digitalWrite(trig, LOW);  // Set trig pin to low to ensure a clean pulse
      delayMicroseconds(2);     // Delay for 2 microseconds
      digitalWrite(trig, HIGH); // Send a 10 microsecond pulse by setting trig pin to high
      delayMicroseconds(10);    // Delay for 10 microseconds
      digitalWrite(trig, LOW);  // Set trig pin back to low

      // Measure the pulse width of the echo pin and calculate the distance value
      distance = pulseIn(echo, HIGH) / 58.00;  // Formula: (340m/s * 1us) / 2
    }
  }
};

/* ========================================================================== */

// Serial setup
const int serialPort = 9600;
const bool sendNewLine = false;

// Servo setup
Servo myServo;
const int servoPin = 2; //* CONFIGURE FOR YOURSELF IF YOU WANT

// Defines Trig and Echo pins of the Ultrasonic Sensor 1
//* CONFIGURE FOR YOURSELF IF YOU WANT
sen sensor1 = {
  .enabled = true,
  .trig = 3,
  .echo = 4,
};

// Defines Trig and Echo pins of the Ultrasonic Sensor 2
//* CONFIGURE FOR YOURSELF IF YOU WANT
sen sensor2 = {
  .enabled = false,
  .trig = 5,
  .echo = 6,
};

// Servo angle range
//* CONFIGURE FOR YOURSELF IF YOU WANT
const int min_angle = 0;
const int max_angle = 180;

/* ========================================================================== */

void setup() {
  Serial.begin(serialPort); // Start serial connection as port `serialPort`
  myServo.attach(servoPin); // Attaches the `servoPin` to the `myServo` object

  if (sensor1.enabled) {
    // Set input and output of sensor 1
    pinMode(sensor1.trig, OUTPUT);
    pinMode(sensor1.echo, INPUT);
  }

  if (sensor2.enabled) {
    // Set input and output of sensor 2
    pinMode(sensor2.trig, OUTPUT);
    pinMode(sensor2.echo, INPUT);
  }
}

void loop() {
  // Rotates the servo motor from `min_angle` to `max_angle`
  for (int i = min_angle; i <= max_angle; i++) {
    myServo.write(i);
    delay(30);

    // Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree
    sensor1.updateDistance();
    sensor2.updateDistance();
    sendPackage(i, sensor1.distance, sensor2.distance);
  }

  // Rotates the servo motor back from `max_angle` to `min_angle`
  for (int i = max_angle; i > min_angle; i--) {
    myServo.write(i);
    delay(30);

    // Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree
    sensor1.updateDistance();
    sensor2.updateDistance();
    sendPackage(i, sensor1.distance, sensor2.distance);
  }
}

/* ========================================================================== */

void sendPackage(int angle, int distance1, int distance2) {
  Serial.print(angle);        // Sends the current degree into the Serial Port

  if (sensor1.enabled) {
    Serial.print(",");        // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    Serial.print(distance1);  // Sends the distance value into the Serial Port
  }

  if (sensor2.enabled) {
    Serial.print(",");        // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    Serial.print(distance2);  // Sends the distance value into the Serial Port
  }

  Serial.print(".");          // Sends addition character right next to the previous value needed later in the Processing IDE for indexing

  if (sendNewLine)
    Serial.print("\n");         // Sends a new line for readability
}
