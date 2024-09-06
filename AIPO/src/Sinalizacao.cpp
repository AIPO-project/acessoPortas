#include "Sinalizacao.h"


void Sinalizacao::iniciar(){
  //colocar aqui a inicialização dos pinos e bibliotecas da sinalização
  pinMode(PIN_CONEXAO, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_AZUL, OUTPUT);
  pinMode(PIN_VERDE, OUTPUT);
  pinMode(PIN_VERMELHO, OUTPUT);

  desligarLedRgb();
}

void Sinalizacao::SinalizarAcessoPermitido(){
  ligarLedRgbVerde();
  somSucesso();
  delay(1000);
  desligarLedRgb();
  
}

void Sinalizacao::SinalizarAcessoNegado(){
  ligarLedRgbVermelho();
  somFracasso();
  delay(1000);
  desligarLedRgb();
}
 
void Sinalizacao::desligarLedRgb(){
  digitalWrite(PIN_AZUL, HIGH);
  digitalWrite(PIN_VERDE, HIGH);
  digitalWrite(PIN_VERMELHO, HIGH);
}
 
void Sinalizacao::ligarLedRgbVerde(){
    digitalWrite(PIN_AZUL, HIGH);
    digitalWrite(PIN_VERDE, LOW);
    digitalWrite(PIN_VERMELHO, HIGH);
}

void Sinalizacao::ligarLedRgbVermelho(){
    digitalWrite(PIN_AZUL, HIGH);
    digitalWrite(PIN_VERDE, HIGH);
    digitalWrite(PIN_VERMELHO, LOW);
}

void Sinalizacao::ligarLedRgbAzul(){
    digitalWrite(PIN_AZUL, LOW);
    digitalWrite(PIN_VERDE, HIGH);
    digitalWrite(PIN_VERMELHO, HIGH);
}

void Sinalizacao::ligarLedRgbAmarelo(){
    digitalWrite(PIN_AZUL, HIGH);
    digitalWrite(PIN_VERDE, LOW);
    digitalWrite(PIN_VERMELHO, LOW);
}

void Sinalizacao::ligarLedRgbMagenta(){
    digitalWrite(PIN_AZUL, LOW);
    digitalWrite(PIN_VERDE, HIGH);
    digitalWrite(PIN_VERMELHO, LOW);
}

void Sinalizacao::ligarLedRgbCiano(){
    digitalWrite(PIN_AZUL, LOW);
    digitalWrite(PIN_VERDE, LOW);
    digitalWrite(PIN_VERMELHO, HIGH);
}

void Sinalizacao::ligarLedRgbBranco(){
    digitalWrite(PIN_AZUL, LOW);
    digitalWrite(PIN_VERDE, LOW);
    digitalWrite(PIN_VERMELHO, LOW);
}

void Sinalizacao::somSucesso(){
  tone(PIN_BUZZER, 1000, 200); // Toca uma nota de 1000 Hz por 200ms
  delay(200);
  tone(PIN_BUZZER, 1500, 200); // Toca uma nota de 1500 Hz por 200ms
  delay(200);
  tone(PIN_BUZZER, 2000, 200); // Toca uma nota de 2000 Hz por 200ms
  delay(200);
}

void Sinalizacao::somTentativa(){
  //Colocar aqui a implementação do som quando for iniciar a consulta
}

void Sinalizacao::somFracasso(){
  tone(PIN_BUZZER, 500, 500);
  tone(PIN_BUZZER, 500, 500); 
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