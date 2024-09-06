#include "Sinalizacao.h"


void Sinalizacao::iniciar(){
  //colocar aqui a inicialização dos pinos e bibliotecas da sinalização
  pinMode(PIN_CONEXAO, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_AZUL, OUTPUT);
  pinMode(PIN_VERDE, OUTPUT);
  pinMode(PIN_VERMELHO, OUTPUT);


}

// void Sinalizacao::desligarLedRgb(){
  
// }

void Sinalizacao::somSucesso(){
  //Colocar aqui a implementação do som quando for bem sucedida a consulta
}

void Sinalizacao::somTentativa(){
  //Colocar aqui a implementação do som quando for iniciar a consulta
}

void Sinalizacao::somFracasso(){
  //Colocar aqui a implementação do som quando não for bem sucedida a consulta
}


void Sinalizacao::ledConexao() {
  static unsigned long ledMqttPrevTime = 0;
  unsigned long time_ms = millis();
  bool ledStatus = false;
  if ( (WiFi.status() == WL_CONNECTED)) {
    if (client.isMqttConnected()) {
      if ( (time_ms - ledMqttPrevTime) >= LED_INTERVAL_MQTT) {
        ledStatus = !digitalRead(PIN_CONEXAO);
        digitalWrite(PIN_CONEXAO, ledStatus);
        ledMqttPrevTime = time_ms;
      }
    }
    else {
      digitalWrite(PIN_CONEXAO, LOW); //liga led
    }
  }
  else {
    digitalWrite(PIN_CONEXAO, HIGH); //desliga led
  }
}