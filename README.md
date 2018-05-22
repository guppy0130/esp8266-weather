# ESP8266 Weather Display
[Powered by Dark Sky](https://darksky.net/poweredby/)

## Hardware
### Components
* ESP8266 (I used a NodeMCU v1)
* Waveshare 1.54" E-Paper display

### Build Instructions
1. Connect ESP8266 and Waveshare E-Paper display according to the [ZinggJM/GxEPD repo](https://github.com/ZinggJM/GxEPD)

## Software
### Pre-reqs
* Arduino IDE (v1.8.5)
	* [GxEPD library](https://github.com/ZinggJM/GxEPD)
	* [ArduinoJSON library](https://github.com/bblanchon/ArduinoJson)
	* [ESP8266 Arduino Core](https://github.com/esp8266/Arduino)
* [DarkSky API Key](https://darksky.net/dev/docs)

### Build Instructions
1. Connect PC to ESP8266
2. Obtain drivers from [silabs.com](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers)
3. Follow instructions from the [esp8266/Arduino repo](https://github.com/esp8266/Arduino) to set up IDE/board communication
4. Obtain DarkSky API key
5. Replace SSID, password, GMT offset, DarkSky API key, and GPS coordinates in sketch
6. Upload sketch to ESP8266
