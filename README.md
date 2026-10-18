# Smart Fire & Gas Leak Alarm System 🔥💨

An advanced Arduino-based environmental monitoring system that detects fires and gas leaks. It provides real-time data on an OLED display, triggers visual alerts, and physically actuates a servo motor to simulate fire suppression or room ventilation based on the specific threat.

## Features
* **Real-time Monitoring:** Continuously tracks temperature, humidity, and combustible gas levels.
* **OLED Status Display:** Shows current environmental readings or specific emergency instructions.
* **Latched Alarm System:** Once a hazard is detected, the alarm locks into an active state until physically reset by a human operator.
* **Multi-Tiered Threat Response:**
  * **Fire Alert:** Turns on a solid red LED and sweeps a servo to 180° (simulating a fire suppression sprinkler or extinguisher).
  * **Gas Leak:** Flashes the red LED and moves the servo to 90° (simulating the opening of a ventilation window).

## Why Three Sensors? (Sensor Fusion)
A standard smoke detector can easily be fooled by everyday activities like cooking (e.g., making a *tadka*) or someone smoking nearby, leading to annoying false alarms. 

To solve this, this project uses **Sensor Fusion** to accurately classify the threat:
* **Actual Fire (Suppression Mode):** Smoke/Gas detected + Temperature High (> 35°C) + Humidity Low (< 40%). 
* **Cooking/Gas Leak (Ventilation Mode):** Smoke/Gas detected + Normal Temperature and Humidity. 
* **Safe:** Gas levels normal (< 550).

## Hardware Components
* Arduino Uno (or compatible)
* DHT11 Temperature and Humidity Sensor
* MQ Series Gas Sensor (e.g., MQ-2)
* SSD1306 OLED Display (SPI version)
* Servo Motor
* Red LED (with appropriate current-limiting resistor)
* Push Button
* Breadboard & Jumper Wires

## Wiring Diagram
*(Note: Upload an image of your schematic to your GitHub repository and replace "link_to_image.png" below with the actual file name)*

![Project Wiring Diagram](link_to_image.png)

## Pin Configuration 

| Component | Arduino Pin | Notes |
| :--- | :--- | :--- |
| **DHT11 Sensor** | Digital 7 | Data pin |
| **MQ Gas Sensor** | Analog A0 | Analog out from sensor |
| **Red LED** | Digital 4 | Connect through a 220Ω resistor |
| **Servo Motor** | Digital 6 | PWM control pin |
| **Reset Button** | Digital 2 | Wire to ground (uses internal pull-up) |
| **OLED MOSI** | Digital 11 | SPI Data |
| **OLED CLK** | Digital 13 | SPI Clock |
| **OLED CS** | Digital 10 | Chip Select |
| **OLED RESET** | Digital 9 | Display Reset |
| **OLED DC** | Digital 8 | Data/Command |

## Required Libraries
Install the following libraries via the Arduino Library Manager (`Sketch` -> `Include Library` -> `Manage Libraries...`):
1. **Adafruit GFX Library** by Adafruit
2. **Adafruit SSD1306** by Adafruit
3. **DHT sensor library** by Adafruit (Ensure you also have the *Adafruit Unified Sensor* library installed)
4. **Servo** (Included in the standard Arduino IDE)

## How to Install and Run
1. Wire your components according to the Pin Configuration table above.
2. Clone or download this repository and open `Fire_Alarm.ino` in the Arduino IDE.
3. Install the required libraries if you haven't already.
4. Select your Arduino board and COM port from the `Tools` menu.
5. Click **Upload**.

## System Logic Walkthrough
1. **Safe State:** When gas is normal (< 550), the system rests. The servo stays at 0°, the LED is off, and the OLED displays the current temperature and humidity.
2. **Detection:** If the analog gas reading exceeds `550`, the system "latches" the alarm variable (`alarmLatched = true`).
3. **Evaluation:**
   * If Temperature > 35°C **and** Humidity < 40%, the system classifies the threat as a **Fire**. It triggers the suppression sequence.
   * Otherwise, it classifies the threat as a **Gas Leak** and triggers the ventilation sequence.
4. **Reset:** The system remains in alarm mode regardless of changing sensor values until a user presses the reset button connected to Pin 2, confirming the area is clear.

