#include <TimerOne.h>

const int potPin = A0;   // Pino do potenciômetro (conectado ao A0)
const int pwmPin = 9;    // Pino de saída PWM (conectado ao D9)

void setup() {
  pinMode(pwmPin, OUTPUT);
  Timer1.initialize(50);  // Inicializa o timer com uma frequência de 20 kHz (50 us de período)
  Timer1.pwm(pwmPin, 0);  // Inicializa o PWM com ciclo de trabalho de 0 (desligado)
}

void loop() {
  // Lê o valor do potenciômetro (de 0 a 1023)
  int potValue = analogRead(potPin);
  
  // Mapeia o valor do potenciômetro para a faixa de 0 a 1023 (valor do PWM)
  int pwmValue = map(potValue, 0, 1023, 0, 1023);
  
  // Define o valor do PWM
  Timer1.setPwmDuty(pwmPin, pwmValue);
}
