#include <WiFi.h> //conexao wifi
#include <PubSubClient.h> //bibli do mqtt
#include <WebServer.h> //bibli do servidor web

const char* ssid = "Aécio's Labs";
const char* password = "presentedopai2";
const char* mqttServer = "192.168.0.199"; // IP do broker MQTT(rede interna)
const int mqttPort = 1883;

const char* mqttTopic = "led/control";
const char* clientmqttId = "ESP32Transmissor";

WiFiClient espClient;
PubSubClient clientmqtt(espClient);

WebServer server(80);

//Prototipo das funções
void requisicao(String);

//Pagina html
#include "html.h"


void setup() {
  Serial.begin(115200);
  delay(10);


  // Conecta-se à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Conectado à rede Wi-Fi");

  // Configura o clientmqtte MQTT
  clientmqtt.setServer(mqttServer, mqttPort);

  while (!clientmqtt.connected()) {
    Serial.println("Conectando ao BROKER MQTT...");
    if (clientmqtt.connect(clientmqttId)) {
      Serial.println("Conectado ao BROKER MQTT");
    } else {
      Serial.println("Falha na conexão ao BROKER MQTT. Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }


  //Servidor
  server.on("/", handleRoot);
  
  server.on("/TETO=ON", handleOn);
  
  server.on("/off", handleOff);
  
  /*server.on("/inline", []() {
   server.send(200, "text/plain", "this works as well");
  });*/
  
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");

  Serial.print("IP para se conectar: http://");
  Serial.println(WiFi.localIP());
}




//Reconectar MQTT
void reconnect() {
  // Reconecta-se ao servidor BROKER MQTT
  while (!clientmqtt.connected()) {
    Serial.println("Tentando reconectar ao BROKER MQTT...");
    if (clientmqtt.connect(clientmqttId)) {
      Serial.println("Reconectado ao BROKER MQTT");
    } else {
      Serial.println("Falha na reconexão ao BROKER MQTT. Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}


/*
//função requisição
void requisicao(String request){
  if(request.indexOf("/TETO=ON") != -1){
    clientmqtt.publish(mqttTopic, "ligar");
  }
}*/


void loop() {
  // Mantenha a conexão MQTT ativa
  if (!clientmqtt.connected()) {
    reconnect();
  }
  clientmqtt.loop();

  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks

  /*
  WiFiClient client = server.available(); //Verificando se alguem esta conectado no server
  if(client){//se n tiver client conectado
    Serial.println("Novo client conectado");
    while(!client.available()){//Enquanto cliente estiver conectado
      delay(2);//delay de 2 milesegundos
    }
    String request = client.readStringUntil('\r');//Faz leitura da primeira linha da requisição
    requisicao(request); //trata o request por meio da funcao requisicao
    client.flush(); //aguarda ate que os dados de saida sejam enviados ao cliente
    client.println("HTTP/1.1 200 OK");//Escreve para o cliente a versao do http
    client.println("Content-Type: text/html");//escreve para o cliente o tipo de conteudo(txt html)
    client.println("");
    client.println(TETO);

    delay(2);
    Serial.println("Cliente Desconectado");
    Serial.println("");
  }*/
}


void handleRoot() {/*
 String html = "";
 html+= "<style>a {font-size:40px; background-color:#CCCCCC;}</style>";
 html+= "<meta charset='UTF-8'>";
 html += "<h1>Exemplo SoftAP IFRJ campus Niterói</h1>";
 html += "Clique <a href=\"/on\">aqui</a> para ligar o LED.<br><br><br>";
 html += "Clique <a href=\"/off\">aqui</a></h2> para desligar o LED.";
 html += "<h3>Autor: Luiz Oliveira</h3>";*/
 server.send(200, "text/html", TETO);
}
 
void handleOn() {
 clientmqtt.publish(mqttTopic, "ligar");
 handleRoot();
}
 
void handleOff() {
 clientmqtt.publish(mqttTopic, "ligar");
 handleRoot();
}
 
void handleNotFound() {
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
}
 
