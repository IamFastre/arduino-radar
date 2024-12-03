// Includes the Servo library
#include <Servo.h>.

enum DebugLevel {
  NONE,
  BASIC,
  VERBOSE,
};

const bool DEBUG_LEVEL = NONE;

void debug(int lvl, char s[]...) {
  if (lvl <= DEBUG_LEVEL)
    Serial.println(s);
}

/* ========================================================================== */

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
      debug(VERBOSE, "Wrote LOW to trig:", String(trig));

      delayMicroseconds(2);     // Delay for 2 microseconds
      debug(VERBOSE, "Waited 2μs");

      digitalWrite(trig, HIGH); // Send a 10 microsecond pulse by setting trig pin to high
      debug(VERBOSE, "Wrote HIGH to trig:", String(trig));

      delayMicroseconds(10);    // Delay for 10 microseconds
      debug(VERBOSE, "Waited 10μs");

      digitalWrite(trig, LOW);  // Set trig pin back to low
      debug(VERBOSE, "Wrote LOW to trig:", String(trig));

      // Wait for a high pulse in `echo`
      unsigned long pulseDuration = pulseIn(echo, HIGH);
      debug(VERBOSE, "Pulse duration from echo:", String(echo), " took ", String(pulseDuration), "ms");

      // Measure the pulse width of the echo pin and calculate the distance value
      distance = pulseDuration / 58.00; // Formula: (340m/s * 1μs) / 2
    }
  }
};

/* ============================= Configurations ============================= */

// Serial setup
const int SERIAL_PORT = 9600;
const bool SEND_NEWLINE = false;

// Servo setup
const int SERVO_PIN = 2;

// Servo angle range
const int MIN_ANGLE = 0;
const int MAX_ANGLE = 180;

// Defines Trig and Echo pins of the ultrasonic sensor 1
sen sensor1 = {
  .enabled = true,
  .trig = 3,
  .echo = 4,
};

// Defines Trig and Echo pins of the ultrasonic sensor 2
sen sensor2 = {
  .enabled = false,
  .trig = 5,
  .echo = 6,
};

/* ========================================================================== */

const int LOOP_DELAY = DEBUG_LEVEL ? 240 : 30;
Servo myServo;

void setup() {
  Serial.begin(SERIAL_PORT); // Start serial connection as port `SERIAL_PORT`
  myServo.attach(SERVO_PIN); // Attaches the `SERVO_PIN` to the `myServo` object
  debug(BASIC, "Servo attached.");

  if (sensor1.enabled) {
    // Set input and output of sensor 1
    pinMode(sensor1.trig, OUTPUT);
    pinMode(sensor1.echo, INPUT);
    debug(BASIC, "Sensor #1 connected.");
  }

  if (sensor2.enabled) {
    // Set input and output of sensor 2
    pinMode(sensor2.trig, OUTPUT);
    pinMode(sensor2.echo, INPUT);
    debug(BASIC, "Sensor #2 connected.");
  }
}

void loop() {
  // Rotates the servo motor from `MIN_ANGLE` to `MAX_ANGLE`
  for (int i = MIN_ANGLE; i <= MAX_ANGLE; i++) {
    myServo.write(i);
    debug(VERBOSE, "Servo angle (asc): ", String(i));
    delay(LOOP_DELAY);

    // Calls a function for calculating the distance
    // measured by the each sensor for each degree
    sensor1.updateDistance();
    sensor2.updateDistance();
    sendPackage(i, sensor1.distance, sensor2.distance);
  }

  // Rotates the servo motor back from `MAX_ANGLE` to `MIN_ANGLE`
  for (int i = MAX_ANGLE; i > MIN_ANGLE; i--) {
    myServo.write(i);
    debug(VERBOSE, "Servo angle (desc): ", String(i));
    delay(LOOP_DELAY);

    // Calls a function for calculating the distance
    // measured by the each sensor for each degree
    sensor1.updateDistance();
    sensor2.updateDistance();
    sendPackage(i, sensor1.distance, sensor2.distance);
  }
}

/* ========================================================================== */

void sendPackage(int angle, int distance1, int distance2) {
  if (DEBUG_LEVEL) {
    debug(BASIC, "•--- Package Start ---•");
    debug(BASIC, "Angle: ", String(angle));
    if (sensor1.enabled)
      debug(BASIC, "Distance #1: ", String(distance1));
    if (sensor2.enabled)
      debug(BASIC, "Distance #2: ", String(distance2));
    debug(BASIC, "•---- Package End ----•");
    return;
  }

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

  if (SEND_NEWLINE)
    Serial.print("\n");       // Sends a new line for readability
}
