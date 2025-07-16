#include "potenciometro.h"

Pot pot1(A0);
Pot pot2(A1);
void setup() {
  Serial.begin(115200);
  delay(100);
  pot1.setAngRef();
  pot2.setAngRef();
}

void loop() {
  int ang1 = pot1.leitura();
  int ang2 = pot2.leitura();
  float x = 14 * (cos(ang1) + cos(ang1 + ang2));
  float y = 14 * (sin(ang1) + sin(ang1 + ang2));
  Serial.print(x);
  Serial.print(",");
  Serial.println(y);
}
