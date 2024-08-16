#ifndef SINALIZACAO_H
#define SINALIZACAO_H

#define PIN_CONEXAO  1  //valor aleatorio para pino de conexão somente para exemplificar 
#define PIN_VERMELHO 2  //valor aleatorio para pino de conexão somente para exemplificar 
#define PIN_AZUL     3  //valor aleatorio para pino de conexão somente para exemplificar 
#define PIN_VERDE    5  //valor aleatorio para pino de conexão somente para exemplificar 

class Sinalizacao{

  public:
  void init();

  inline void ligarConexao(){}
  inline void desligarConexao(){}

  inline void ligarVerde(){}
  inline void desligarVerde(){}

};

#endif