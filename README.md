# Virtual Smart Agriculture Monitoring and Automated Irrigation System

A fully virtual IoT demonstration built with an ESP32 in Wokwi, a DHT22 temperature/humidity sensor, a potentiometer used as a soil-moisture sensor simulator, an LED used as a virtual pump indicator, and ThingSpeak for cloud data storage and visualization.

> **Important scope note:** This project demonstrates irrigation decision logic. The LED represents the pump's ON/OFF state. There is no physical water pump and no water is moved.

## Features

- Reads temperature and relative humidity from a virtual DHT22.
- Simulates soil-moisture readings using a potentiometer connected to ESP32 ADC GPIO 34.
- Applies a configurable dry-soil threshold (default: below 30%).
- Turns a virtual LED ON when irrigation is required and OFF otherwise.
- Uploads readings to ThingSpeak over Wi-Fi using HTTPS.
- Displays four cloud fields: temperature, humidity, soil moisture, and irrigation status.

## Technology stack

- Wokwi ESP32 simulator
- ESP32 Arduino framework / C++
- DHT sensor library for ESPx
- ThingSpeak cloud channel

## Pin connections

| Component | Pin | ESP32 |
|---|---|---|
| DHT22 | VCC | 3V3 |
| DHT22 | DATA | GPIO 15 |
| DHT22 | GND | GND |
| Potentiometer | VCC | 3V3 |
| Potentiometer | SIG | GPIO 34 |
| Potentiometer | GND | GND |
| LED | Anode through 220 ohm resistor | GPIO 26 |
| LED | Cathode | GND |

DHT22 NC pin is left unconnected. The resistor should be in series with the LED.

## ThingSpeak channel setup

Configure the channel fields as follows:

| Field | Name | Meaning |
|---|---|---|
| Field 1 | Temperature | Degrees Celsius |
| Field 2 | Humidity | Relative humidity (%) |
| Field 3 | Soil Moisture | Simulated moisture (%) |
| Field 4 | Irrigation Status | `1` = irrigation required; `0` = not required |

The project was configured using channel ID `3599710`. The channel ID is not a secret; the **Write API Key is secret**.

## Setup

1. Open the Wokwi project containing the ESP32, DHT22, potentiometer, and LED.
2. Open `src/sketch.ino` and copy its contents into Wokwi's `sketch.ino`.
3. Ensure the `DHTesp` library is installed. In Wokwi, add it through Library Manager or create `libraries.txt` containing:
   ```
   DHTesp
   ```
4. In the source code, replace `PASTE_YOUR_WRITE_API_KEY_HERE` with your own ThingSpeak **Write API Key**.
5. Do not publish your real API key in a public repository. For a public GitHub repo, replace it with the placeholder again before committing.
6. Start the Wokwi simulation and open Serial Monitor at 115200 baud.
7. Open the ThingSpeak channel's Private View to see incoming data.

The sketch uploads every 20 seconds to respect the usual minimum update interval of a free ThingSpeak channel.

## Decision logic

- If soil moisture is below 30%, `irrigationStatus = 1` and the LED turns ON.
- If soil moisture is 30% or above, `irrigationStatus = 0` and the LED turns OFF.

The potentiometer-to-moisture conversion is a simulation mapping. It is not a calibrated measurement of real soil.

## Test cases

| Test | Simulated soil moisture | Expected LED | Expected Field 4 |
|---|---:|---|---:|
| Dry soil | Below 30% | ON | 1 |
| Moist soil | 30% or above | OFF | 0 |

Also verify that Wi-Fi connects, DHT22 readings are printed, and ThingSpeak returns a positive entry ID for an accepted update. A ThingSpeak response of `0` indicates that the update was not accepted.

## Security note

The example uses `WiFiClientSecure::setInsecure()` to simplify TLS setup in a learning simulator. This disables server certificate verification and is not suitable for production systems. Use certificate validation in a real deployment. Never commit a real Write API Key to a public repository.

## Limitations and future enhancements

- No physical pump or water-flow control is implemented.
- Soil moisture is simulated with a potentiometer.
- Future work could include a calibrated capacitive soil sensor, a properly rated driver and pump, pump runtime limits, low-water protection, alerts, and certificate-validated cloud communication.

## Project details to personalize

- Student: `[Your Name]`
- Institution: `[College / University]`
- Department: `[Department]`
- Guide: `[Project Guide]`
- Academic year: `[Year]`
