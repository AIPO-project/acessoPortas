
#ifndef CONFIG_HA_H
#define CONFIG_HA_H

#include "EspMQTTClient.h"
#include "Connect.h"
#include <ArduinoJson.h>

class ConfigHA{
  private:

  bool auto_discovery = true;      //default to false and provide end-user interface to allow toggling 
  
  public:

  void haDiscovery();
};

#endif
