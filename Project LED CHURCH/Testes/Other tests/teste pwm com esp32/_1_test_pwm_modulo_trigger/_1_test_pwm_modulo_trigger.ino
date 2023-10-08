#include <Arduino.h>

const int pwmPin = 5;  // Pino PWM a ser usado (você pode escolher outro)
const int fadeDuration = 2000;  // Duração de cada etapa (2 segundos)

void setup() {
  pinMode(pwmPin, OUTPUT);
}

void loop() {
  // Etapa 1: 100% de brilho
  analogWrite(pwmPin, 255);
  delay(fadeDuration);

  // Etapa 2: 0% de brilho
  analogWrite(pwmPin, 0);
  delay(fadeDuration);

  // Etapa 3: 50% de brilho
  analogWrite(pwmPin, 127);
  delay(fadeDuration);

  // Etapa 4: Aumento gradual de 0% a 100% de brilho
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(pwmPin, brightness);
    delay(fadeDuration / 255);  // Divide a duração igualmente
  }

  // Agora, diminui gradualmente de 100% a 0% de brilho
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(pwmPin, brightness);
    delay(fadeDuration / 255);  // Divide a duração igualmente
  }
}
