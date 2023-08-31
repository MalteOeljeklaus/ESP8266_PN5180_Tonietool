#pragma once

#ifndef I2C_SCAN_HPP
#define I2C_SCAN_HPP

#include <vector>
#include <Arduino.h>
#include <HardwareSerial.h>
#include <Adafruit_SSD1306.h>

std::vector<byte> scan_i2c(HardwareSerial& Serial);
byte find_display(const std::vector<byte>& addresses, Adafruit_SSD1306& display, HardwareSerial& Serial);

#endif // I2C_SCAN_HPP