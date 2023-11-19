#include "include/nfc.hpp"
#include "include/password.hpp"


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

int32_t loopCnt = 0;
bool errorFlag = false;

bool pn5180_loop(uint8_t (&uid)[8]) {
  if (errorFlag) {
    uint32_t irqStatus = pn5180_device.getIRQStatus();
    showIRQStatus(irqStatus);

    if (0 == (RX_SOF_DET_IRQ_STAT & irqStatus)) { // no card detected
      Serial.println(F("*** No card detected!"));
    }

    pn5180_device.reset();
    pn5180_device.setupRF();

    errorFlag = false;
  }

  Serial.println(F("----------------------------------"));
  Serial.print(F("Loop #"));
  Serial.println(loopCnt++);

  // code for unlocking an ICODE SLIX2 protected tag
  for (uint8_t (&password)[4]: passwords)
  {
    ISO15693ErrorCode myrc = pn5180_device.unlockICODESLIX2(password);
    if (ISO15693_EC_OK == myrc) {
      Serial.println("unlockICODESLIX2 successful");
    }
  }

  
/* 
  // code for set a new SLIX2 privacy password
  nfc.getInventory(uid);
  Serial.println("set new password"); 
  
  ISO15693ErrorCode myrc2 = nfc.newpasswordICODESLIX2(password, standardpassword, uid);
  if (ISO15693_EC_OK == myrc2) { 
   Serial.println("sucess! new password set");    
  }else{
   Serial.println("fail! no new password set: "); 
   Serial.println(nfc.strerror(myrc2));  
   Serial.println(" "); 
  } 
 */
  
//  uint8_t uid[8];
  ISO15693ErrorCode rc = pn5180_device.getInventory(uid);
  if (ISO15693_EC_OK != rc) {
    Serial.print(F("Error in getInventory: "));
    Serial.println(pn5180_device.strerror(rc));
    errorFlag = true;
    return !errorFlag;
  }
  Serial.print(F("Inventory successful, UID="));
  for (int i=0; i<8; i++) {
    Serial.print(uid[7-i], HEX); // LSB is first
    if (i < 2) Serial.print(":");
  }
  Serial.println();

  Serial.println(F("----------------------------------"));
  uint8_t blockSize, numBlocks;
  rc = pn5180_device.getSystemInfo(uid, &blockSize, &numBlocks);
  if (ISO15693_EC_OK != rc) {
    Serial.print(F("Error in getSystemInfo: "));
    Serial.println(pn5180_device.strerror(rc));
    errorFlag = true;
    return !errorFlag;
  }
  Serial.print(F("System Info retrieved: blockSize="));
  Serial.print(blockSize);
  Serial.print(F(", numBlocks="));
  Serial.println(numBlocks);

  Serial.println(F("----------------------------------"));
  uint8_t readBuffer[blockSize];
  for (int no=0; no<numBlocks; no++) {
    rc = pn5180_device.readSingleBlock(uid, no, readBuffer, blockSize);
    if (ISO15693_EC_OK != rc) {
      Serial.print(F("Error in readSingleBlock #"));
      Serial.print(no);
      Serial.print(": ");
      Serial.println(pn5180_device.strerror(rc));
      errorFlag = true;
      return !errorFlag;
    }
    Serial.print(F("Read block #"));
    Serial.print(no);
    Serial.print(": ");
    for (int i=0; i<blockSize; i++) {
      if (readBuffer[i] < 16) Serial.print("0");
      Serial.print(readBuffer[i], HEX);
      Serial.print(" ");
    }
    Serial.print(" ");
    for (int i=0; i<blockSize; i++) {
      if (isprint(readBuffer[i])) {
        Serial.print((char)readBuffer[i]);
      }
      else Serial.print(".");
    }
    Serial.println();
  }

//#ifdef WRITE_ENABLED
//  Serial.println(F("----------------------------------"));
//  uint8_t *writeBuffer = malloc(blockSize);
//  for (int i=0; i<blockSize; i++) {
//    writeBuffer[i] = 0x80 + i;
//  }
//  for (int no=0; no<numBlocks; no++) {
//    rc = pn5180_device.writeSingleBlock(uid, no, writeBuffer, blockSize);
//    if (ISO15693_EC_OK == rc) {
//      Serial.print(F("Wrote block #"));
//      Serial.println(no);
//    }
//    else {
//      Serial.print(F("Error in writeSingleBlock #"));
//      Serial.print(no);
//      Serial.print(": ");
//      Serial.println(pn5180_device.strerror(rc));
//      errorFlag = true;
//      return;
//    }
//  }
//#endif /* WRITE_ENABLED */

/*
  // code for locking an ICODE SLIX2 protected tag   
  ISO15693ErrorCode myrc = nfc.lockICODESLIX2(password);
  if (ISO15693_EC_OK == myrc) {
    Serial.println("lockICODESLIX2 successful");
    delay(5000);
*/
  return !errorFlag;
  delay(1000);
}

void showIRQStatus(uint32_t irqStatus) {
  Serial.print(F("IRQ-Status 0x"));
  Serial.print(irqStatus, HEX);
  Serial.print(": [ ");
  if (irqStatus & (1<< 0)) Serial.print(F("RQ "));
  if (irqStatus & (1<< 1)) Serial.print(F("TX "));
  if (irqStatus & (1<< 2)) Serial.print(F("IDLE "));
  if (irqStatus & (1<< 3)) Serial.print(F("MODE_DETECTED "));
  if (irqStatus & (1<< 4)) Serial.print(F("CARD_ACTIVATED "));
  if (irqStatus & (1<< 5)) Serial.print(F("STATE_CHANGE "));
  if (irqStatus & (1<< 6)) Serial.print(F("RFOFF_DET "));
  if (irqStatus & (1<< 7)) Serial.print(F("RFON_DET "));
  if (irqStatus & (1<< 8)) Serial.print(F("TX_RFOFF "));
  if (irqStatus & (1<< 9)) Serial.print(F("TX_RFON "));
  if (irqStatus & (1<<10)) Serial.print(F("RF_ACTIVE_ERROR "));
  if (irqStatus & (1<<11)) Serial.print(F("TIMER0 "));
  if (irqStatus & (1<<12)) Serial.print(F("TIMER1 "));
  if (irqStatus & (1<<13)) Serial.print(F("TIMER2 "));
  if (irqStatus & (1<<14)) Serial.print(F("RX_SOF_DET "));
  if (irqStatus & (1<<15)) Serial.print(F("RX_SC_DET "));
  if (irqStatus & (1<<16)) Serial.print(F("TEMPSENS_ERROR "));
  if (irqStatus & (1<<17)) Serial.print(F("GENERAL_ERROR "));
  if (irqStatus & (1<<18)) Serial.print(F("HV_ERROR "));
  if (irqStatus & (1<<19)) Serial.print(F("LPCD "));
  Serial.println("]");
}
