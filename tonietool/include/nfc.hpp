#pragma once

#ifndef NFC_HPP
#define NFC_HPP

#include <PN5180.h>
#include <PN5180ISO15693.h>

// getting the pinout right is tricky, for more info see
// https://github.com/ATrappmann/PN5180-Library/issues/23#issuecomment-582076363
// https://mechatronicsblog.com/esp8266-nodemcu-pinout-for-arduino-ide/
#define PN5180_NSS 2   // D4
#define PN5180_BUSY 16 // D0
#define PN5180_RST 0   // D3

bool pn5180_init(uint8_t (&productVersion)[2], uint8_t (&firmwareVersion)[2], uint8_t (&eepromVersion)[2]);
//bool pn5180_init(uint8_t* productVersion, uint8_t* firmwareVersion, uint8_t* eepromVersion);
//void pn5180_loop();
bool pn5180_loop(uint8_t (&uid)[8]);
void showIRQStatus(uint32_t irqStatus);

#endif // NFC_HPP

