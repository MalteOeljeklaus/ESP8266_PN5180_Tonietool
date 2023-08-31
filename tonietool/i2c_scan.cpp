#include "include/i2c_scan.hpp"

#include <Wire.h>

std::vector<byte> scan_i2c(HardwareSerial& Serial)
{
    std::vector<byte> ret;

    byte error, address;
    int nDevices;
    Serial.println("Scanning I2C...");
    nDevices = 0;
    for(address = 1; address < 127; address++ ) {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
      if (error == 0) {
        Serial.print("I2C device found at address 0x");
        if (address<16) {
          Serial.print("0");
        }
        Serial.println(address,HEX);
        ret.push_back(address);
        nDevices++;
      }
      else if (error==4) {
        Serial.print("Unknow error at address 0x");
        if (address<16) {
          Serial.print("0");
        }
        Serial.println(address,HEX);
      }    
    }
    if (nDevices == 0) {
      Serial.println("No I2C devices found\n");
    }

    return ret;
}

byte find_display(const std::vector<byte>& addresses, Adafruit_SSD1306& display, HardwareSerial& Serial)
{
  for(const byte& a : addresses)
  {
    Serial.print("Testing address 0x");
    if (a<16) {
      Serial.print("0");
    }
    Serial.println(a,HEX);

    if(display.begin(SSD1306_SWITCHCAPVCC, a)) {
      return a;
    }
  }
  return 255;
}
