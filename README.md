# Movement 👋

## The main idea ✨

Make first gestures with robohand 😱

## Devices

TI LaunchPad CC1352R1 x2
TI Sensors BoosterPack “BOOSTXL-SENSORS”
Laptop or PC
Ajax special edition super robohand

## What is the project about

This project was made to gain knowledge about servo-motors, servo-motor driver PCA9685, gyroscope, accelerometer, madgwick filter, radio protocol

## How does it works

First microcontroller gets raw data from gyroscope and accelerometer (“BOOSTXL-SENSORS”), and puts this data into the madgwick filter. After that, transmit filtered x coordinate into another microcontroller. Second launchpad gets coordinate and puts all fingers in the correct position.

## Building the program

1. Download/Clone the source code.
2. Open CodeComposerStudio.
3. Build the program.
4. Connect devices.
5. Open UnitFlash program and find device.
6. In UnitFlash (find button Program) -> Flash Image -> Browse -> TX (dir) -> Debug (dir) -> choose TX.out(file). (to the first launchpad)
7. In UnitFlash (find button Program) -> Flash Image -> Browse -> RX (dir) -> Debug (dir) -> choose RX.out(file). (to the first launchpad)
8. In UnitFlash Load Image for both.
9. Well done & good luck!😎
