# System Architecture

```text
DHT22 ----------------------> ESP32
Potentiometer (soil input) -> ESP32
                               |-- GPIO 26 --> LED indicator (virtual pump state)
                               |
                               +-- Wi-Fi / HTTPS --> ThingSpeak
                                      Field 1: Temperature
                                      Field 2: Humidity
                                      Field 3: Soil Moisture
                                      Field 4: Irrigation Status
```

Decision: soil moisture below 30% => irrigation status 1 and LED ON; otherwise status 0 and LED OFF.
