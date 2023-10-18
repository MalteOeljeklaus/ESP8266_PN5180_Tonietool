#include "include/nfc.hpp"

PN5180ISO15693 pn5180_device(PN5180_NSS, PN5180_BUSY, PN5180_RST);


bool pn5180_init(uint8_t (&productVersion)[2], uint8_t (&firmwareVersion)[2], uint8_t (&eepromVersion)[2])
{
  productVersion[0] = productVersion[1] = 255;
  firmwareVersion[0] = firmwareVersion[1] = 255;
  eepromVersion[0] = eepromVersion[1] = 255;
  
  pn5180_device.begin();
  Serial.println("Resetting PN5180 nfc reader chip");
  pn5180_device.reset();
  Serial.println("Reading product version");
  pn5180_device.readEEprom(PRODUCT_VERSION, productVersion, sizeof(productVersion));
  Serial.print(F("Product version=0x"));
  if (productVersion[1]<16)
    Serial.print("0");
  Serial.print(productVersion[1], HEX);
  Serial.print(".0x");
  if (productVersion[0]<16)
    Serial.print("0");
  Serial.println(productVersion[0], HEX);

  if (0xff == productVersion[1]) { // if product version 255, the initialization failed
    Serial.println(F("PN5180 initialization failed"));
    Serial.flush();
    return false;
  }

  Serial.println(F("Reading firmware version..."));
  pn5180_device.readEEprom(FIRMWARE_VERSION, firmwareVersion, sizeof(firmwareVersion));
  Serial.print(F("Firmware version=0x"));
  if (firmwareVersion[1]<16)
    Serial.print("0");
  Serial.print(firmwareVersion[1], HEX);
  Serial.print(".0x");
  if (firmwareVersion[0]<16)
    Serial.print("0");
  Serial.println(firmwareVersion[0], HEX);

  Serial.println(F("Reading EEPROM version..."));
  pn5180_device.readEEprom(EEPROM_VERSION, eepromVersion, sizeof(eepromVersion));
  Serial.print(F("EEPROM version=0x"));
  if (eepromVersion[1]<16)
    Serial.print("0");
  Serial.print(eepromVersion[1], HEX);
  Serial.print(".0x");
  if (eepromVersion[0]<16)
    Serial.print("0");
  Serial.println(eepromVersion[0], HEX);
  Serial.println(F("Enable RF field..."));
  pn5180_device.setupRF();

  return true;
}
