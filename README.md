# Arduino Radar (Actually Sonar)

A simple radar system using the *Arduino Uno*, a *servo motor*, and an *ultrasonic sensor* (*HC-SR04*).
The radar scans the environment, measures distances, and visualizes the data in real-time using **Processing** .


## Circuit Diagram

<div>
  <img
    src="./Media/Diagram.png"
    width="100%"
    alt="Circuit Diagram"
  />
  <center style="font-size: 10px;">
    Through <a href="https://wokwi.com/">Wokwi</a>
  </center>
</div>

## Instructions

1. Build the circuit.
1. Upload `ArduinoRadar.ino` to your Arduino board.
1. Take note of the current Arduino port.
1. Open `GUI/RadarGUI.pde` in the [Processing IDE](https://processing.org/).
1. Modify the configs to ensure the correct COM port.
1. Run the **Processing** sketch to visualize the radar data.

## Arduino Code

The Arduino code is located in `ArduinoRadar/ArduinoRadar.ino`. \
This code controls the servo motor to rotate the ultrasonic sensor and measure distances, sending the data to the serial port for **Processing** to read.

## Processing Code

The Processing code is in the `GUI` folder. \
The files include:

- `Assets/OCRAExtended-30.vlw`: Font file used by the **Processing** sketch.
- `RadarGUI.pde`: Main **Processing** code for visualizing radar data.

---

### Acknowledgments

This project is based on a tutorial from [How To Mechatronics](https://howtomechatronics.com/projects/arduino-radar-project/) by [Dejan](https://howtomechatronics.com/author/howtom12_wp/).
