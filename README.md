# Weather Station
![Weather](https://i.ibb.co/4W5jHH2/weather.jpg)

Arduino Based Online Weather Station's Firmware.

### Build
The source code is verified to be compatible with [Platform IO](https://platformio.org/) and [Arduino IDE](https://www.arduino.cc/).

### Configure
- Setup in **Secret.h** your WiFi.  
- Change the data sending logic to your own.

### Hardware
- [Arduino Mini](https://store.arduino.cc/usa/arduino-mini-05).
- Any 3.3V - 5V DC Arduino compitable Power Supply.
- Temperature, Humidity DHT-22.
- ESP-01-S or any esp8266 WiFi module with Serial.

**Optional**
- BME280 Temperature, Humidity, Pressure Sensor. Replaces DHT-22.
- DS3231 Mini RTC. Get time offline.
- MH-Z19 CO2 Sensor or better.
- 2004 i2c Display. Or any i2c display.
- Lux sensor. A photoresistor is enough if it's just for backlit control, not for an accurate light level data.
- RGB Led Module. Any.
- Resistors and other common electronic parts.

### Wiring of Base Version
![Wiring](https://i.ibb.co/D7K6z10/0616039442245.jpg)
