#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Aécio's Labs";
const char* password = "presentedopai2";
const char* mqttServer = "192.168.0.199"; // IP do broker MQTT
const int mqttPort = 1883;

const char* mqttTopic = "led/control";
const char* clientId = "ESP32Receptor";

const int ledPin = 15; // Pino do LED

WiFiClient espClient;
PubSubClient client(espClient);

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

  // Configura o cliente MQTT
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    if (client.connect(clientId)) {
      Serial.println("Conectado ao servidor MQTT");
      client.subscribe(mqttTopic);
    } else {
      Serial.println("Falha na conexão ao servidor MQTT. Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }

  // Configura o pino do LED como saída
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Inicialmente, o LED está desligado
}

void loop() {
  // Mantenha a conexão MQTT ativa
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Função de callback chamada quando uma mensagem MQTT é recebida
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (message == "ligar") {
    digitalWrite(ledPin, !digitalRead(ledPin)); // Liga o LED
  }
}

void reconnect() {
  // Reconecta-se ao servidor MQTT
  while (!client.connected()) {
    Serial.println("Tentando reconectar ao servidor MQTT...");
    if (client.connect(clientId)) {
      Serial.println("Reconectado ao servidor MQTT");
      client.subscribe(mqttTopic);
    } else {
      Serial.println("Falha na reconexão ao servidor MQTT. Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}
