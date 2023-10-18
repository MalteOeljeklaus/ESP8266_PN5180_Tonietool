# ESP8266_PN5180_Tonietool

## Setup of IDE
* Ensure that your PC has drivers for CH340 USB-to-serial chip installed. On linux it can be fiddly, make sure you have the correct [rights](https://support.arduino.cc/hc/en-us/articles/360016495679-Fix-port-access-on-Linux) and no [conflicting packages](https://stackoverflow.com/questions/70123431/why-would-ch341-uart-is-disconnected-from-ttyusb)
* Install Arduino IDE
* Add support package for ESP8266 NodeMCU board, see [instructions](https://blog.opendatalab.de/codeforbuga/2016/07/02/arduino-ide-mit-nodemcu-esp8266) and [board manager url](http://arduino.esp8266.com/stable/package_esp8266com_index.json)
* In library manager add "Adafruit SSD1306". Chose "Select all" when asked to install additional dependencies "Adafruit BusIO" and "Adafruit GFX Library".
* Add library "[PN5180-Library](https://github.com/ATrappmann/PN5180-Library/)" from zip file using the "Sketch" menu.

## Compile and upload MCU software
* Open sketch "tonietool.ino" and select board "NodeMCU 1.0"
* Click the check button to test if the sketch can compile successfully.
* Connect Tonietool device to USB, select correct serial port in Arduino IDE depending on your system configuration.
* Click the upload button.
