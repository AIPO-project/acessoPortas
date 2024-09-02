#include "Sensor.h"

Sensor::Sensor():pn532_i2c(Wire),nfc(pn532_i2c){

}

void Sensor::iniciar(){
/*
  resetPN532();
  nfc.begin(); //inicia o leitor de nfc/rfid

  uint32_t versiondata = nfc.getFirmwareVersion();
  if ( !versiondata ) {
    pn532Fail=true;

    Serial.println("Didn't find PN53x board");    
  }
  else{
    nfc.SAMConfig();
    pn532Fail=false;

    Serial.println(versiondata);
  }
*/
}