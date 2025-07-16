#include "potenciometro.h"
#include <Arduino.h>

Pot::Pot(int pin) {
  this->pino = pin;
  this->xn1 = 0;
  this->yn1 = 0;
  this->ultInter = 0;

}

void Pot::setAngRef() {
  xn = analogRead(pino) / 1023.0; // normaliza para 0–1
  this->angRef = xn * 270; // depende da calibração real
  xn1 = xn;
  yn1 = angRef;
  Serial.print("Este é o ângulo de Referencia:");
  Serial.println(angRef);
}

 Pot::leitura() {
  unsigned long now = micros();
  if (now - ultInter >= interAmostra) {
    xn = analogRead(pino) / 1023.0; // normaliza
    yn = 0.072821 * xn + 0.072821 * xn1 + 0.854359 * yn1;
    yn1 = yn;
    xn1 = xn;
    ultInter = now;
    int angulo = 270 * yn;
    if(angulo > angRef){ 
        return 360 + (angRef - angulo);
    }
    return angRef-angulo;
  }
}

