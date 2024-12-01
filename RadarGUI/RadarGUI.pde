import processing.serial.*; // for serial communication
import java.awt.event.KeyEvent; // for reading the data from the serial port

/* ========================================================================== */
/*                                   CONFIGS                                  */
/* ========================================================================== */

final String COM_PORT = "COM1";
final int BAUD_RATE = 9600;

final boolean FULLSCREEN = true;
final int SCREEN_WIDTH = 1920;
final int SCREEN_HEIGHT = 1080;

final boolean SHOW_CREDITS = true;
final boolean SHOW_ANGLES = true;
final boolean SHOW_DISTANCES = true;

final String[] CREDITS = { "IamFastre" }; // Don't tell your teacher I made it for you

/* =============== You don't have to edit anything past this. =============== */

Serial myPort;
PFont orcFont;

int angle;
int distance;
float radarRadius;

color primary   = #0c0c0c;
color secondary = #666666;
color tertiary  = #eeeeee;
color accent    = #1ed760;
color hot       = #d71e1e;

// You can use variables to change settings in this function
void settings() {
  if (FULLSCREEN)
    fullScreen();

  size(SCREEN_WIDTH, SCREEN_HEIGHT);
  smooth(50);
}

void setup() {
  orcFont = loadFont("./Assets/OCRAExtended-30.vlw");
  myPort = new Serial(this, COM_PORT, BAUD_RATE); // Start the serial communication
  myPort.bufferUntil('.'); // Reads the data up to the character '.' to be parsed, in the format of: `angle,distance`.

  // For the initial 
  noStroke();
  fill(accent);
  rect(0, 0, width, height); 
}

void draw() {
  // Set these values for the rest of the render process
  radarRadius = height * 0.85 / 2;
  textFont(orcFont);

  // Setting background color
  noStroke();
  fill(0, 10);
  rect(0, 0, width, height); 

  // Calling the draw functions
  drawRadar(); 
  drawRadarText(); 
  drawRadarAnalog();
  drawInfoBox(); 
}

// Starts reading data from the Serial Port
void serialEvent(Serial myPort) {
  // Reads the data from the Serial Port up to the character '.' and puts it into the String variable `data`.
  String data = myPort.readStringUntil('.');
  data = data.substring(0, data.length() - 1);

  int i = data.indexOf(","); // Find the character ',' and puts it into the variable `i`

  angle = int(data.substring(0, i));
  distance = int(data.substring(i + 1, data.length()));
}

void drawRadar() {
  pushMatrix();
  translate(width / 1.4, height / 2);

  strokeWeight(4);
  stroke(accent);

  /* ================================ Circles =============================== */

  // Center Dot
  fill(accent);
  circle(0, 0, 8);

  // Circles Range
  noFill();
  circle(0, 0, radarRadius * 0.166 * 2);
  circle(0, 0, radarRadius * 0.333 * 2);
  circle(0, 0, radarRadius * 0.500 * 2);
  circle(0, 0, radarRadius * 0.666 * 2);
  circle(0, 0, radarRadius * 0.833 * 2);
  circle(0, 0, radarRadius * 1.000 * 2);

  /* ================================= Lines ================================ */

  stroke(accent, 2);
  // Axis Lines
  line(-radarRadius, 0, radarRadius, 0); // X
  line(0, -radarRadius, 0, radarRadius); // Y

  // Diagonal Lines
  line(radarRadius * sin(PI/4), -radarRadius * sin(PI/4), -radarRadius * sin(PI/4), radarRadius * sin(PI/4)); // /
  line(-radarRadius * sin(PI/4), -radarRadius * sin(PI/4), radarRadius * sin(PI/4), radarRadius * sin(PI/4)); // \

  popMatrix();
}

void angleText(int theta) {
  // Rotates the matrix around the center of the radar
  translate(width / 1.4, height / 2);
  rotate(-radians(theta));

  textAlign(RIGHT);
  text(str(theta) + "°", radarRadius, 0);

  resetMatrix();
}

void drawRadarText() {
  pushMatrix();

  textSize(20);
  fill(accent);

  /* ================================ Angles ================================ */

  if (SHOW_ANGLES) {
    resetMatrix();
    angleText(45*0);
    angleText(45*1);
    angleText(45*2);
    angleText(45*3);
    angleText(45*4);
    angleText(45*5);
    angleText(45*6);
    angleText(45*7);
  }

  /* =============================== Distances ============================== */

  if (SHOW_DISTANCES) {
    float margin = height * 0.015;

    resetMatrix();
    fill(secondary);
    textSize(12);
    textAlign(CENTER);
    translate(width / 1.4, height / 2);
    rotate(PI/8);

    text("10\ncm", -radarRadius * 0.166 - margin, 0);
    text("20\ncm", -radarRadius * 0.333 - margin, 0);
    text("30\ncm", -radarRadius * 0.500 - margin, 0);
    text("40\ncm", -radarRadius * 0.666 - margin, 0);
    text("50\ncm", -radarRadius * 0.833 - margin, 0);
    text("60\ncm", -radarRadius * 1.000 - margin, 0);
  }

  popMatrix();
}

void drawRadarAnalog() {
  pushMatrix();
  translate(width / 1.4, height / 2);
  rotate(-radians(angle)); // Rotates the analog according to the current angle

  // To know how much green:red there will be
  float percentage = (distance / 60.0);

  // Red part
  // - is thinner to vanish under the green part
  // - takes the whole length
  stroke(hot);
  strokeWeight(5.5);
  line(0, 0, radarRadius, 0);


  // Green part
  // - covers the red part
  // - takes less length when is object is near
  stroke(accent);
  strokeWeight(6);
  line(0, 0, radarRadius * percentage, 0);

  popMatrix();
}

void infoText(String key, String value, float index, float boxW, float margin) {
  // Key
  fill(secondary);
  text(key + ": ", boxW * 0.15 - margin, (height * 0.2) + (height * 0.075 * index));
  // Value
  fill(tertiary);
  text(value, boxW * 0.5 - margin, (height * 0.2) + (height * 0.075 * index));
}

void drawInfoBox() {
  float margin = width * 0.015;
  float boxW = width / 2.33;
  float boxH = height - margin;

  pushMatrix();
  translate(0, 0);

  fill(primary);
  stroke(secondary);
  strokeWeight(3);

  /* ================================== Box ================================= */

  rect(margin, margin, boxW - margin, boxH - margin);

  /* ================================= Text ================================= */

  // Title
  textSize(60);
  textAlign(LEFT);
  fill(accent);
  text("Arduino Radar", margin * 2.5, margin * 4.5);

  // Properties
  textSize(36);
  textAlign(LEFT);
  infoText("Object", distance > 60 ? "In Range" : "Out of Range", 0, boxW, margin);
  infoText("Angle", str(angle) + "°", 1, boxW, margin);
  infoText("Distance", str(distance) + "cm", 2, boxW, margin);

  // Credits
  if (SHOW_CREDITS) {
    textSize(14);
    textAlign(LEFT);
    fill(accent);

    // Place the 'By:' on the same height as the first credited
    text("By:", margin * 2, height - margin * (2.5 + 0.5 * CREDITS.length));

    fill(tertiary);
    for (int i = 0; i < CREDITS.length; i++) {
      String s = CREDITS[i];
      // Determine the height at which the name lies according to index and array length
      text(
        s + (i == CREDITS.length - 1 ? "." : ","),
        margin * 3.5, height - margin * (2.5 + 0.5 * CREDITS.length - i)
      );
    }
  }

  popMatrix();
}
