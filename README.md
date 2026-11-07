# Smart Industrial Cable Health Monitoring System

An ESP32-based IoT prototype for monitoring cable-related operating conditions using voltage, magnetic-field, vibration, temperature and humidity sensors. Sensor readings are displayed on a Blynk IoT dashboard, and a buzzer provides a local alert when the programmed fault conditions are detected.

> **Project note:** This is an academic prototype. It is not a certified electrical protection or cable-fault-location device. Do not connect the prototype directly to mains wiring without appropriate isolation, enclosure, fusing and qualified supervision.

## Project images

### Circuit diagram
![Circuit diagram](Images/circuit_diagram.jpg)

### Hardware prototype
![Hardware prototype](Images/hardware_prototype.jpg)

### Blynk IoT dashboard
![Blynk dashboard](Images/blynk_dashboard.jpg)

## Features

- Monitors voltage using a ZMPT-based voltage sensor input.
- Reads Hall-effect sensor output.
- Detects vibration using a digital vibration sensor.
- Measures temperature and humidity using a DHT11.
- Classifies the condition as `NORMAL`, `MIDDLE LEVEL` or `FAULT` using configurable thresholds.
- Activates a buzzer when a fault condition is detected.
- Sends sensor values and status to the Blynk IoT dashboard.
- Prints readings and status to the Serial Monitor.

## Hardware components

- ESP32 development board
- 230 V AC to 12 V transformer (for the isolated prototype power supply)
- Bridge rectifier, filter capacitor and 7805 regulator (as shown in the circuit)
- ZMPT voltage sensor module
- Hall-effect sensor module
- Vibration sensor module
- DHT11 temperature and humidity sensor
- Buzzer
- Relay and associated driver/interface circuitry
- Connecting wires and prototype board

## Software and platforms

- Arduino IDE
- ESP32 board support package
- Blynk IoT
- `WiFi.h`
- `BlynkSimpleEsp32.h`
- `DHT.h`

## Pin configuration

| Function | ESP32 GPIO |
|---|---:|
| Voltage sensor analog output | GPIO 34 |
| Hall-effect sensor analog output | GPIO 35 |
| Vibration sensor digital output | GPIO 27 |
| DHT11 data | GPIO 4 |
| Buzzer control | GPIO 23 |

## Blynk virtual pin mapping

| Virtual pin | Data |
|---|---|
| V0 | Voltage estimate |
| V1 | Hall-effect ADC value |
| V2 | Temperature |
| V3 | Humidity |
| V4 | Vibration state |
| V5 | System status |

Configure matching datastreams and dashboard widgets in your Blynk template.

## Getting started

1. Install Arduino IDE and the ESP32 board package.
2. Install the **Blynk** and **DHT sensor library** libraries from the Arduino Library Manager.
3. Open `Code/cable_health_monitoring.ino`.
4. Enter your own Blynk Template ID, Template Name, Auth Token, Wi-Fi SSID and password.
5. Confirm the GPIO connections match your actual hardware.
6. Calibrate the sensor thresholds for your modules and test setup.
7. Upload the sketch to the ESP32.
8. Open Serial Monitor at **115200 baud** and view the Blynk dashboard.

## Fault-detection logic

- **FAULT:** Hall value reaches the fault threshold, vibration input is HIGH, temperature reaches the fault threshold, or estimated voltage is outside the configured extreme limits.
- **MIDDLE LEVEL:** No fault condition is present, but a Hall, temperature or voltage reading crosses its warning threshold.
- **NORMAL:** None of the configured warning or fault conditions is present.

Thresholds are prototype settings and must be validated experimentally. Sensor polarity and output behavior can vary by module.

## Important calibration and safety notes

- The voltage calculation in this sketch is a simple ADC scaling estimate, **not a calibrated AC RMS measurement**. A proper AC voltage measurement requires suitable signal conditioning, sampling and calibration against a trusted meter.
- Verify that the voltage sensor module provides an ESP32-safe output. ESP32 GPIOs are **not 5 V tolerant**.
- The 230 V AC section is hazardous. Use a properly rated isolated supply, fuse, insulated enclosure, strain relief and safe clearances. Have mains-side wiring checked by a qualified person.
- Check the vibration sensor's output polarity and set `VIB_PIN` logic accordingly.
- Calibrate Hall and temperature thresholds using normal and simulated test conditions.
- The DHT11 can occasionally return invalid readings; the sketch skips invalid temperature/humidity values when sending them to Blynk.

## Repository contents

```text
Smart-Cable-Health-Monitoring-System/
├── README.md
├── Code/
│   └── cable_health_monitoring.ino
├── Images/
│   ├── circuit_diagram.jpg
│   ├── hardware_prototype.jpg
│   └── blynk_dashboard.jpg
└── LICENSE
```

## Future improvements

- Implement calibrated RMS voltage measurement.
- Add event logging and timestamped fault history.
- Add configurable warning and fault thresholds through the dashboard.
- Improve electrical isolation, enclosure and protection design.
- Validate the detection logic using controlled test cases.

## Author

Add your name, department, college and project team members here.

## License

This project is shared under the MIT License. See [LICENSE](LICENSE).
