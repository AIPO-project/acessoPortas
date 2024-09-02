#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
//#include <NfcAdapter.h>

#define NDEF_DEBUG false

class Sensor{

  private:
  //declaracao de objetos para uso na 
  PN532_I2C pn532_i2c;
  PN532 nfc;
  //PN532 nfc = PN532(pn532_i2c);
  //NfcAdapter nfc = NfcAdapter(pn532_i2c);
  bool pn532Fail=true;

  public:

  Sensor();

  void iniciar();

};

#endif