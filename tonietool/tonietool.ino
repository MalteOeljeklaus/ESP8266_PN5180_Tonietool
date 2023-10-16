#include <vector>

#include <Wire.h>
 
#include "include/display.hpp"

bool valid_display;

void setup() {
  delay(500);          
  Wire.setClockStretchLimit(40000);

  Wire.begin();
  Serial.begin(115200);
  Serial.println("\n");
  Serial.println("\nTonietool booting...");
  valid_display=false;
  while(!valid_display)
  {
    valid_display = display_init();
  }
  if (valid_display)
  {
    display_print("Tonietool booting...\n");
    display_splash_logo();
  }

}
 
void loop() {

  delay(5000);          
}
