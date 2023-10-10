#include <WiFi.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <WiFiManager.h>


const char* mqttServer = "192.168.0.110"; // IP do broker MQTT
const int mqttPort = 1883;
const char* mqttTopic = "STAGE";
const char* clientID = "ESP32Receptor";

#define pinoR 15
#define pinoG 2
#define pinoB 4

int eered = 0, eegreen = 1, eeblue = 2;

WiFiClient espClient;
PubSubClient client(espClient);

void saveeeprom(int valor, int endereco) {
  EEPROM.write(endereco, valor);
  EEPROM.commit(); // Grava as alterações na EEPROM
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  int RED, GREEN, BLUE;
  sscanf(message.c_str(), "%d,%d,%d", &RED, &GREEN, &BLUE);

  Serial.print(" R: ");Serial.print(RED);Serial.print(" G: ");Serial.print(GREEN);Serial.print(" B: ");Serial.println(BLUE);
  analogWrite(pinoR, RED);
  analogWrite(pinoG, GREEN);
  analogWrite(pinoB, BLUE);
  saveeeprom(RED, eered);
  saveeeprom(GREEN, eegreen);
  saveeeprom(BLUE, eeblue);
}

void setup() {
  Serial.begin(115200);//inicia o serial
  EEPROM.begin(512);//inicia a biblioteca EEPROM

  //inicia os leds com as cores salvas pela ultima vez
  pinMode(pinoR, OUTPUT); // Configura o pino RED como saída
  pinMode(pinoG, OUTPUT); // Configura o pino GREEN como saída
  pinMode(pinoB, OUTPUT); // Configura o pino BLUE como saída

  analogWrite(pinoR, EEPROM.read(eered));
  analogWrite(pinoG, EEPROM.read(eegreen));
  analogWrite(pinoB, EEPROM.read(eeblue));


  //Wifi manager
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("LEDs PALCO","comunica2024IOT");
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


  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Tentando se reconectar ao MQTT...");
    if (client.connect(clientID)) {
      Serial.println("Conectado ao MQTT Broker!");
      client.subscribe(mqttTopic);
    } else {
      Serial.print("Falha na conexão - Erro: ");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos.");
      delay(5000);
    }
  }
}
