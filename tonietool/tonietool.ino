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
 
void loop() {

  delay(5000);          
}
