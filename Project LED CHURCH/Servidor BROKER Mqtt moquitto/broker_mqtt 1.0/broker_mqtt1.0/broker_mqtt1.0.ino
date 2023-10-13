#include "sMQTTBroker.h"
#include <WiFiManager.h>



sMQTTBroker broker;
unsigned long Time;
unsigned long freeRam;




void setup() {
  Serial.begin(115200);

   //Wifi manager
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("LEDs Broker Server","comunica2024IOT");
  if(!res) {
      Serial.println("Failed to connect");
      ESP.restart();
  } 
  else {
      //if you get here you have connected to the WiFi    
      Serial.println("connected...yeey :)");
  }

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
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
