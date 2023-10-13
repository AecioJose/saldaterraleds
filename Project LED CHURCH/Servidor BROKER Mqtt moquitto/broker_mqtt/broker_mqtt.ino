#include "sMQTTBroker.h"


sMQTTBroker broker;
unsigned long Time;
unsigned long freeRam;

//IPAddress local_IP(192, 168, 0, 199);
//IPAddress gateway(192, 168, 12, 1);
//IPAddress subnet(255, 255, 255, 0);
//IPAddress primaryDNS(8, 8, 8, 8);
//IPAddress secondaryDNS(8, 8, 4, 4);
//
void setup() {
  Serial.begin(115200);
/*
  if(!WiFi.config(local_IP, gateway, subnet)){
    Serial.println("STA Failed to configure");
  }*/

  WiFi.begin("Comunicacao_2G", "comunicacao2024");
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
  }
  Serial.println("Connection established!");
  delay(1000);

  Serial.println("");
  Serial.print("IP addres:  ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC addres:  ");
  Serial.println(WiFi.macAddress());

  Serial.print("SubnetMask:  ");
  Serial.println(WiFi.subnetMask());

  Serial.print("Gateway IP:  ");
  Serial.println(WiFi.gatewayIP());

  Serial.print("DNS:  ");
  Serial.println(WiFi.dnsIP());

  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");

  const unsigned short mqttPort = 1883;
  broker.init(mqttPort);
  //all done
}

void loop() {
  broker.update();

  if(millis()-Time>1000){//codigo para printar ram a cada 1s
    Time = millis();
    if(ESP.getFreeHeap() != freeRam){
      freeRam = ESP.getFreeHeap();
      Serial.println("");
      Serial.print("Ram: ");
      Serial.print(freeRam);
      
    }
  }

}
