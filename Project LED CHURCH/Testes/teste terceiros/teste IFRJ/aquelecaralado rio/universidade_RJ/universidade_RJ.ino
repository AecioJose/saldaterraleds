#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
 
#define LED_BUILTIN 2   //Configura o pino do LED embutido
 
//Configura o nome da rede sem fio que se deseja conectar
const char* ssid = "Aécio's Labs";
const char* password = "presentedopai2";
 
WebServer server(80);
 
void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
 
 
  //Configura o baudrate da comunicação serial
  Serial.begin(115200);
 
  //Configura a conexão à rede sem fio
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  
  server.on("/on", handleOn);
  
  server.on("/off", handleOff);
  
  /*server.on("/inline", []() {
   server.send(200, "text/plain", "this works as well");
  });*/
  
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void) {
 server.handleClient();
 delay(2);//allow the cpu to switch to other tasks
}

void handleRoot() {
 String html = "";
 html+= "<style>a {font-size:40px; background-color:#CCCCCC;}</style>";
 html+= "<meta charset='UTF-8'>";
 html += "<h1>Exemplo SoftAP IFRJ campus Niterói</h1>";
 html += "Clique <a href=\"/on\">aqui</a> para ligar o LED.<br><br><br>";
 html += "Clique <a href=\"/off\">aqui</a></h2> para desligar o LED.";
 html += "<h3>Autor: Luiz Oliveira</h3>";
 server.send(200, "text/html", html);
}
 
void handleOn() {
 digitalWrite(LED_BUILTIN, 1);
 handleRoot();
}
 
void handleOff() {
 digitalWrite(LED_BUILTIN, 0);
 handleRoot();
}
 
void handleNotFound() {
 digitalWrite(LED_BUILTIN, 1);
 String message = "File Not Found\n\n";
 message += "URI: ";
 message += server.uri();
 message += "\nMethod: ";
 message += (server.method() == HTTP_GET) ? "GET" : "POST";
 message += "\nArguments: ";
 message += server.args();
 message += "\n";
 for (uint8_t i = 0; i < server.args(); i++) {
   message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
 }
 server.send(404, "text/plain", message);
 digitalWrite(LED_BUILTIN, 0);
}
 