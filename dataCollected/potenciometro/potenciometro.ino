#include "potenciometro.h"

Pot pot1(A0);
Pot pot2(A1);
void setup() {
  Serial.begin(115200);

  pot1.setAngRef();
}

void loop() {
  float leitura = analogRead(A0);
  Serial.println(leitura);
  delay(100);
}
