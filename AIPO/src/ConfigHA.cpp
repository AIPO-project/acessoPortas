#include "ConfigHA.h"

void ConfigHA::iniciar(){
    client.setMaxPacketSize(512);    
}

void ConfigHA::haDiscovery() {
  char topic[128];
  if (auto_discovery) {
      char buffer1[512];
      char uid[128];
      DynamicJsonDocument doc(512);
      doc.clear();

      Serial.println("Adicionando controle porta...");
      //Create unique topic based on devUniqueID
      strcpy(topic, "homeassistant/lock/");
      strcat(topic, client.getMqttClientName());
      strcat(topic, "/config");
      //Create unique_id based on devUniqueID
      //strcpy(uid, getUniqueID().c_str());
      //Create JSON payload per HA documentation
      doc["name"] = client.getMqttClientName();
      doc["obj_id"] = "mqtt_porta";
      doc["uniq_id"] = client.getMqttClientName();
      doc["stat_t"] = topic_name+"/"+client.getMqttClientName();
      doc["cmd_t"] = topic_name +"/"+ client.getMqttClientName()+"/cmd";
      doc["value_template"] = "{{ value_json.state }}";
      doc["availability"][0]["topic"] = TOPIC_AVAILABLE;
      JsonObject device = doc.createNestedObject("device");
      device["ids"] = client.getMqttClientName();
      device["name"] = "Trancas das Portas";
  //      device["mf"] = "lennedy";
      device["mdl"] = "ESP8266";
      device["sw"] = "0.06_pn532Test-01";
      device["hw"] = "0.1";
  //      device["cu"] = "http://192.168.1.226/config";  //web interface for device, with discovery toggle
      serializeJson(doc, buffer1);
      //Publish discovery topic and payload (with retained flag)
      client.publish(topic, buffer1, true);

  } else {

      //Remove all entities by publishing empty payloads
      //Must use original topic, so recreate from original Unique ID
      //This will immediately remove/delete the device/entities from HA
      Serial.println("Removing discovered devices...");

      //Lux Sensor
      strcpy(topic, "homeassistant/porta/");
      strcat(topic, client.getMqttClientName());
      strcat(topic, "/config");
      client.publish(topic, "");

      Serial.println("Devices Removed...");
  }
}