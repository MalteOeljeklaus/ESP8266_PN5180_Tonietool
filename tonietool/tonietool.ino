#include <vector>

#include <Wire.h>
 
#include "include/display.hpp"
#include "include/nfc.hpp"

bool valid_display, valid_pn5180;

void setup() {
  delay(500);          
  Wire.setClockStretchLimit(40000);

  Wire.begin();
  Serial.begin(115200);
  Serial.println("\n");
  Serial.println("\nTonietool booting...");
  valid_display=false;
  valid_display = display_init();
  if (valid_display)
    display_print("Tonietool booting...\n");

  uint8_t productVersion[2];
  uint8_t firmwareVersion[2];
  uint8_t eepromVersion[2];
  valid_pn5180 = pn5180_init(productVersion, firmwareVersion, eepromVersion);

  if (valid_display)
  {
    display_print("p");
    display_print(productVersion[1]);
    display_print(".");
    display_print(productVersion[0]);
    display_print(" f");
    display_print(firmwareVersion[1]);
    display_print(".");
    display_print(firmwareVersion[0]);
    display_print(" e");
    display_print(eepromVersion[1]);
    display_print(".");
    display_print(eepromVersion[0]);
    display_print("\n");
  }

  if(valid_display && !valid_pn5180)
    display_print("NFC init failed\n");  

  if (valid_display && valid_pn5180)
    display_splash_logo();

}


void loop()
{
  uint8_t uid[8];
  if (pn5180_loop(uid))
  {
    display_clear(0, 13, 128, 16);
    display_set_cursor(0,13);
    display_print("tag found: yes");
    display_set_cursor(0,21);
//    display_print("ID:");
    for (int i=0; i<8; i++) {
      display_print(uid[7-i]); // LSB is first
      if (i < 2)
        display_print(":");
    }
  }
  else
  {
    display_clear(0, 13, 128, 8);
    display_set_cursor(0,13);
    display_print("tag found: no");
  }

}
