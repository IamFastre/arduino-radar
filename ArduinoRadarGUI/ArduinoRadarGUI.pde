import processing.serial.*; // for serial communication
import java.awt.event.KeyEvent; // for reading the data from the serial port
import java.io.IOException; // for error handling

/* ========================================================================== */
/*                                   CONFIGS                                  */
/* ========================================================================== */

String COM_PORT = "COM1";

boolean SHOW_CREDITS = true;
boolean SHOW_ANGLES = true;
boolean SHOW_DISTANCES = true;

/* ========================================================================== */

Serial myPort;
PFont orcFont;

String angle = "";
String distance = "";
String data = "";

int iAngle, iDistance;

int index1=0;
int index2=0;

color primary   = #0c0c0c;
color secondary = #666666;
color tertiary  = #eeeeee;
color accent    = #1ed760;
color hot       = #d71e1e;

float radarRadius = 0;

void setup() {
  fullScreen();

  size (1920, 1080);

  smooth();
  myPort = new Serial(this, COM_PORT, 9600); // Start the serial communication
  myPort.bufferUntil('.'); // Reads the data from the serial port up to the character '.'. So actually it reads this: angle,distance.
  orcFont = loadFont("OCRAExtended-30.vlw");
}

void draw() {
  // Set these values for the rest of the render process
  radarRadius = height * 0.85 / 2;
  textFont(orcFont);

  // Setting background color
  noStroke();
  fill(primary);
  rect(0, 0, width, height); 

  // Calling the draw functions
  drawRadar(); 
  drawRadarText(); 
  drawRadarAnalog();
  drawInfoBox(); 
}

// starts reading data from the Serial Port
void serialEvent(Serial myPort) {
  // reads the data from the Serial Port up to the character '.' and puts it into the String variable "data".
  data = myPort.readStringUntil('.');
  data = data.substring(0,data.length()-1);
  
  index1 = data.indexOf(","); // find the character ',' and puts it into the variable "index1"
  angle= data.substring(0, index1); // read the data from position "0" to position of the variable index1 or thats the value of the angle the Arduino Board sent into the Serial Port
  distance= data.substring(index1+1, data.length()); // read the data from position "index1" to the end of the data pr thats the value of the distance
  
  // converts the String variables into Integer
  iAngle = int(angle);
  iDistance = int(distance);
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

  stroke(accent, 50);
  // Axis Lines
  line(-radarRadius, 0, radarRadius, 0); // X
  line(0, -radarRadius, 0, radarRadius); // Y

  // Diagonal Lines
  line(radarRadius * sin(PI/4), -radarRadius * sin(PI/4), -radarRadius * sin(PI/4), radarRadius * sin(PI/4)); // /
  line(-radarRadius * sin(PI/4), -radarRadius * sin(PI/4), radarRadius * sin(PI/4), radarRadius * sin(PI/4)); // \

  popMatrix();
}

void angleText(int theta) {
  float radius = radarRadius;

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
  rotate(-radians(iAngle));

  float percentage = (iDistance / 60.0);

  stroke(hot);
  strokeWeight(5.5);
  line(0, 0, radarRadius, 0);

  stroke(accent);
  strokeWeight(6);
  line(0, 0, radarRadius * percentage, 0);

  popMatrix();
}

void infoText(String key, String value, float index, float boxW, float margin) {
  textAlign(LEFT);

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

  noFill();
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

  // Credits
  if (SHOW_CREDITS) {
    textSize(14);
    fill(accent);
    text("By:", margin * 2, height - margin * 3.5);
    fill(tertiary);
    text("Younis Raed Abbas,\nHussein Hassan Ali,\nAli Loay Hussein.", margin * 3.5, height - margin * 3.5);
  }

  // Other Properties
  textSize(36);
  textAlign(RIGHT);
  infoText("Object", iDistance > 60 ? "In Range" : "Out of Range", 0, boxW, margin);
  infoText("Angle", str(iAngle) + "°", 1, boxW, margin);
  infoText("Distance", str(iDistance) + "cm", 2, boxW, margin);

  popMatrix();
}
