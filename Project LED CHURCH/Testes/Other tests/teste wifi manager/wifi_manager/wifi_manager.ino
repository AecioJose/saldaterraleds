#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <WiFiManager.h>

#define LED 5

WiFiServer server(8090);

void setup() {

    // wifimanager
    Serial.begin(115200);

    //Initializing ports
    pinMode(LED, OUTPUT);
    digitalWrite(LED, 0);


    delay(50);

    //Wifi manager
    WiFiManager wm;
    bool res;
    res = wm.autoConnect("LEDs Comunicacao","comunica2024IOT");
    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

    while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
    }

    //DNS
    if (!MDNS.begin("saldaterra_leds")) {
        Serial.println("Error setting up MDNS responder!");
        while(1) {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");

    // Start TCP (HTTP) server
    server.begin();
    Serial.println("TCP server started");

    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
}

int value = 0;

void loop() {
    //webServer
    WiFiClient client = server.available();//Listen for incoming clients
    if(client){
      Serial.println("NewClient");
      String currentLine = "";
      while (client.connected()){
        if(client.available()){
          char c = client.read();
          Serial.write(c);
          if (c == '\n'){
            if(currentLine.length() == 0){
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();

              client.println("<center>");
              client.println("<br>");
              client.println("<a href=\"/bot\"\"><button>Botao</button></a>");
              client.println("<br>");

              client.println("</center>");

              client.println();
              break;
            } else {
              currentLine = "";
            }
          } else if(c != '\r'){
            currentLine += c;
          }

          if (currentLine.endsWith("GET /bot")) {
            digitalWrite(LED, !digitalRead(LED));
          }
        }
      }
      client.stop();
      Serial.println("client Disconnected.");
    }
    delay(15);
}
