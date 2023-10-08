#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Aécio's Labs";
const char* password = "presentedopai2";
const char* mqttServer = "192.168.0.199"; // IP do broker MQTT
const int mqttPort = 1883;

const char* mqttTopic = "led/control";
const char* clientmqttId = "ESP32Transmissor";

WiFiclient espclient;
PubSubclient clientmqtt(espclient);

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

  // Configura o clientm MQTT
  clientmqtt.setServer(mqttServer, mqttPort);

  while (!clientmqtt.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    if (clientmqtt.connect(clientmqttId)) {
      Serial.println("Conectado ao servidor MQTT");
    } else {
      Serial.println("Falha na conexão ao servidor MQTT. Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void loop() {
  // Envie uma mensagem para ligar o LED
  clientmqtt.publish(mqttTopic, "ligar");
  delay(5000); // Envie o sinal a cada 5 segundos
}
