#include "EspMQTTClient.h"
#include <ArduinoJson.h>
#include "ConnectDataIFRN.h"
#include "ConfigHA.h"

#define NDEF_DEBUG false
#include <Arduino.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
//#include <NfcAdapter.h>

#define DATA_INTERVAL 20000       // Intervalo para adquirir novos dados do sensor (milisegundos).
// Os dados serão publidados depois de serem adquiridos valores equivalentes a janela do filtro
#define AVAILABLE_INTERVAL 60000  // Intervalo para enviar sinais de available (milisegundos)
#define READ_SENSOR_INTERVAL 5000  // Intervalo para fazer leituras consecutivas da chave (milisegundos)
#define LED_INTERVAL_MQTT 1000        // Intervalo para piscar o LED quando conectado no broker
#define JANELA_FILTRO 1         // Número de amostras do filtro para realizar a média

//declaracao de objetos para uso na 
PN532_I2C pn532_i2c(Wire);
PN532 nfc = PN532(pn532_i2c);
//PN532 nfc(pn532_i2c);
//NfcAdapter nfc = NfcAdapter(pn532_i2c);

byte RESET_PIN = 14;
byte ACIONAMENTO_PIN = 13;
byte BUZZER_PIN = 12;
byte BLINK_PIN = LED_BUILTIN;


unsigned long dataIntevalPrevTime = 0;      // will store last time data was send
unsigned long availableIntevalPrevTime = 0; // will store last time "available" was send
unsigned long sensorReadTimePrev = 0;        // irá amazenar a última vez que o sensor for lido

String idChave;
HomeAssistant HA;  //trata da configuuração dos dispositivos no home assistant
bool pn532Fail=true;

void setup()
{
  Serial.begin(115200);
  Serial.println("what the hell");

  pinMode(BLINK_PIN, OUTPUT);
  pinMode(ACIONAMENTO_PIN, OUTPUT); // Sets the trigPin as an Output
//  pinMode(BUZZER_PIN, OUTPUT); // Sets the echoPin as an Input  
  pinMode(RESET_PIN, OUTPUT);

  iniciarPN532();

  // Optional functionalities of EspMQTTClient
  //client.enableMQTTPersistence();
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  //client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  client.enableLastWillMessage(TOPIC_AVAILABLE, "offline");  // You can activate the retain flag by setting the third parameter to true
  //client.setKeepAlive(60); 
  WiFi.mode(WIFI_STA);
}

void resetPN532(){
  //Trecho de codigo para resetar o PN532
  digitalWrite(RESET_PIN, LOW);
  delay(100);
  digitalWrite(RESET_PIN, HIGH);
  delay(100);
}

void iniciarPN532(){

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
}

void atuador(const String payload) {

  if(payload == "UNLOCK"){
    digitalWrite(ACIONAMENTO_PIN, HIGH);
    delay(500);
    digitalWrite(ACIONAMENTO_PIN, LOW);
    Serial.println("abertura realizada");
  }
  else{
    //Ativar buzzer
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("Sem autorização");
  }

}

void onConnectionEstablished()
{

  HA.init();
  HA.haDiscovery(); 
  client.subscribe(topic_name +"/"+ client.getMqttClientName()+"/cmd", atuador);

  availableSignal();

}

void availableSignal() {
  client.publish(topic_name +"/"+ client.getMqttClientName() + "/available", "online");
}

bool readSensor() {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    //Serial.println("Didn't find PN53x board");
    pn532Fail=true;
  }

  if(pn532Fail){
    //Serial.println("Erro");
    iniciarPN532();
    return false;
  }

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 5);

  if(success){

    idChave = "";
    
    for(int i = 0; i < uidLength; i++) {
      if(uid[i] < 0x10) {
        idChave += '0';
      }

      idChave += String(uid[i], HEX);

      if(i < uidLength-1)
        idChave += ' ';
    }
  }

  return success;
}

void metodoPublisher() {

  StaticJsonDocument<300> jsonDoc;

  jsonDoc["RSSI"] = WiFi.RSSI();
  jsonDoc["state"] = "LOCKED";

  if(pn532Fail)
    jsonDoc["erro PN532"] = true;
  else
    jsonDoc["erro PN532"] = false;

  jsonDoc["heap"]     = ESP.getFreeHeap();
  jsonDoc["stack"]    = ESP.getFreeContStack();


  String payload = "";
  serializeJson(jsonDoc, payload);

  client.publish(topic_name+"/"+client.getMqttClientName(), payload);
}

void sendChave(){

  StaticJsonDocument<300> jsonDoc;

  jsonDoc["RSSI"] = WiFi.RSSI();
  jsonDoc["idChave"] = idChave;
  jsonDoc["idFechadura"] = client.getMqttClientName();

  String payload = "";
  serializeJson(jsonDoc, payload);

  client.publish(topic_name+"/acesso", payload);

}

void blinkLed() {
  static unsigned long ledWifiPrevTime = 0;
  static unsigned long ledMqttPrevTime = 0;
  unsigned long time_ms = millis();
  bool ledStatus = false;

  if ( (WiFi.status() == WL_CONNECTED)) {
    if (client.isMqttConnected()) {
      if ( (time_ms - ledMqttPrevTime) >= LED_INTERVAL_MQTT) {
        ledStatus = !digitalRead(BLINK_PIN);
        digitalWrite(BLINK_PIN, ledStatus);
        ledMqttPrevTime = time_ms;
      }
    }
    else {
      digitalWrite(BLINK_PIN, LOW); //liga led
    }
  }
  else {
    digitalWrite(BLINK_PIN, HIGH); //desliga led
  }
}

void loop()
{
  unsigned long time_ms = millis();
  
  client.loop();

  bool connected = client.isConnected();

  if ((time_ms - dataIntevalPrevTime >= DATA_INTERVAL) && connected) {
    //client.executeDelayed(1 * 100, metodoPublisher);
    metodoPublisher();
    dataIntevalPrevTime = time_ms;
  }

  if ((time_ms - availableIntevalPrevTime >= AVAILABLE_INTERVAL) && connected){
    //client.executeDelayed(1 * 500, availableSignal);
    availableSignal();
    availableIntevalPrevTime = time_ms;
  }

  if((time_ms - sensorReadTimePrev >= READ_SENSOR_INTERVAL)){
    if( readSensor() && connected){
      //client.executeDelayed(1 * 100, sendChave);
      sendChave();
      sensorReadTimePrev = time_ms;
    }
  }

  blinkLed();

}
