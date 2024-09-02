#ifndef SINALIZACAO_H
#define SINALIZACAO_H

#include <Arduino.h>
#include <EspMQTTClient.h>
#include "Connect.h"

#define PIN_CONEXAO  2  //valor aleatorio para pino de conexão somente para exemplificar 
#define PIN_VERMELHO 1  //valor aleatorio para pino de conexão somente para exemplificar 
#define PIN_AZUL     3  //valor aleatorio para pino de conexão somente para exemplificar 
#define PIN_VERDE    5  //valor aleatorio para pino de conexão somente para exemplificar 

#define LED_INTERVAL_MQTT 1000        // Intervalo para piscar o LED quando conectado no broker

class Sinalizacao{

  public:
  void iniciar();

  void somSucesso();
  void somTentativa();
  void somFracasso();

  inline void ligarConexao(){}
  inline void desligarConexao(){}

  inline void ligarVerde(){}
  inline void desligarVerde(){}

  void ledConexao();

};

#endif