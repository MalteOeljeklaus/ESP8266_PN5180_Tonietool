#include <vector>

#include <Wire.h>
 
#include "include/display.hpp"

bool valid_display;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\n");
  Serial.println("\nTonietool booting...");
  valid_display = display_init();
  if (valid_display)
    display_print("Tonietool booting...\n");

}
 
void loop() {

  delay(5000);          
}
