#ifndef SINALIZACAO_H
#define SINALIZACAO_H

#include <Arduino.h>
#include <EspMQTTClient.h>
#include "Connect.h"

#define PIN_CONEXAO  2  //valor para pino de conexão
#define PIN_VERMELHO 12 //valor para pino de sinalização
#define PIN_AZUL     16 //valor para pino de sinalização 
#define PIN_VERDE    14 //valor para pino de sinalização 
#define PIN_BUZZER   15 //valor para pino do buzzer

#define LED_INTERVAL_MQTT 1000        // Intervalo para piscar o LED quando conectado no broker

class Sinalizacao{

  public:
  void iniciar();

  void SinalizarAcessoPermitido();
  void SinalizarAcessoNegado();

  void somSucesso();
  void somTentativa();
  void somFracasso();

  inline void ligarConexao(){}
  inline void desligarConexao(){}

  inline void ligarVerde(){}
  inline void desligarVerde(){}

  void ledConexao();

  private:
  void desligarLedRgb();

  void ligarLedRgbVermelho();
  void ligarLedRgbVerde();
  void ligarLedRgbAzul();
  void ligarLedRgbAmarelo();
  void ligarLedRgbMagenta();
  void ligarLedRgbCiano();
  void ligarLedRgbBranco();
};

#endif