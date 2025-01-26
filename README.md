## SAFE BEATS

Electronics Club IITI![Image](https://github.com/user-attachments/assets/234352d5-e301-4e26-965a-c0740555825b)

## Description
Safe beat is an arduino based health monitor. It functions to read and analyze the health vitals of user namely the BPM and blood oxygen levels using the MAX30100 sensor. 
## Table of Contents 
1. [Installation](#installation)
2. [Usage](#usage)
3. [Features](#features)


## [Installation](#Installation)
In order to run the code you must have the following libraries installed on your arduino IDE beforehand 
- `LiquidCrystal.h`
- `Keypad.h`
- `Max30100_PulseOximeter.h`
- `Wire.h`

## [Usage](#Usage)
The desired usecase for the project was aimed to replicate the working of a health instrument using an arduino, however it is not limited to the same. The code has an integrated database that is curated to span a variety of age groups and individuals. The readings collected by the instrumentation are run by this dataset and any anamoly is thus detected.


## [Features](#features)
The hardware of this project is put in place to provide a bit askew user interface. The use of the keypad and LCD monitor form the basis of all imput and outputs in the circuit other than the serial monitor. An added utility in the project is that on detecting an anomalous health vital the system initiates a cooldown exercise protocol instructed to the user by means of the LCD display. 


## Installation
Steps to install and set up the project:
```bash
git clone https://github.com/Digivjay-P/Safe-Beat
cd Safe-Beat
# Install required libraries on the Arduino IDE as mentioned above.
