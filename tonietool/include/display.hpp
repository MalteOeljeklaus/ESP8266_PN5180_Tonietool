#pragma once

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <string>

#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

bool display_init();
void display_clear();
void display_set_cursor(const unsigned int& x, const unsigned int& y);
void display_print(const std::string& string);




#endif // DISPLAY_HPP