#include "include/display.hpp"
#include "include/i2c_scan.hpp"
#include "include/logo.hpp"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
byte screen_address;


bool display_init()
{
  screen_address = find_display(scan_i2c(Serial), display, Serial);

  if (screen_address != 255)
  {
    Serial.print("Display found at address 0x");
    if (screen_address<16) {
      Serial.print("0");
    }
    Serial.println(screen_address,HEX);

    display_clear();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.cp437(true);         // Use full 256 char 'Code Page 437' font
    display_set_cursor(0,0);

    display_print("Found display at 0x");
    if (screen_address<16) {
      display_print("0");
    }
    display.println(screen_address,HEX);
    display.display();
  }
  else
  {
    Serial.println("Error: couldn't find a display");
  }

  return screen_address != 255;
}

void display_clear()
{
    display.clearDisplay();
}

void display_set_cursor(const unsigned int& x, const unsigned int& y)
{
  display.setCursor(x,y);
}

void display_print(const std::string& string)
{
  display.print(string.c_str());
  display.display();
}

void display_print(const uint8_t& i)
{
  if (i<16)
    display.print("0");
  display.print(i, HEX);
  display.display();
}

void display_draw_bitmap(const int16_t& posx, const int16_t& posy, const uint8_t bitmap[], const int16_t& width, const int16_t& height)
{
  display.drawBitmap(posx, posy, bitmap, width, height, 1);
  display.display();
}

void display_draw_logo(const int16_t& posx, const int16_t& posy)
{
  display_draw_bitmap(posx, posy, logo_bmp, 128, 12);
}

void display_splash_logo()
{
  for (int16_t i=64; i>=0; i--)
  {
    display.fillRect(0, i+1, 128, 12, 0);
    display_draw_logo(0, i);
    delay(50);
  }

}
