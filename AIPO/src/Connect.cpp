#include "Connect.h"

EspMQTTClient client(
  SSD_WIFI,
  SENHA_WIFI,
  ENDERECO_IP_BROKER,  // MQTT Broker server ip
  LOGIN_BROKER,   // Can be omitted if not needed
  SENHA_BROKER,   // Can be omitted if not needed
  NOME_CLIENTE,     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

const String topic_name = NOME_TOPICO;
