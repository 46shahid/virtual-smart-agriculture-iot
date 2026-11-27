# Wokwi wiring reference

- DHT22 VCC -> ESP32 3V3
- DHT22 DATA -> GPIO 15
- DHT22 GND -> GND
- DHT22 NC -> not connected
- Potentiometer VCC -> 3V3
- Potentiometer SIG -> GPIO 34
- Potentiometer GND -> GND
- GPIO 26 -> 220 ohm resistor -> LED anode
- LED cathode -> GND

Share the 3V3 and GND rails where appropriate. Verify the LED polarity and series resistor.
