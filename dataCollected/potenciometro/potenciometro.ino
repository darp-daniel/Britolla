#include "potenciometro.h"
#include "math.h"

Pot pot1(A0);
Pot pot2(A1);
void setup() {
  Serial.begin(115200);
  delay(100);
  pot1.setAngRef();
  pot2.setAngRef();
  delay(5000);
}

void loop() {
  int ang1 = pot1.leitura();
  int ang2 = pot2.leitura();
  float rad1 = pot1.angulo(ang1);
  float rad2 = pot2.angulo(ang2);
  float x = 14 * (cos(rad1) + cos(rad1 + rad2));
  float y = 14 * (sin(rad1) + sin(rad1 + rad2));
  Serial.print(x);
  Serial.print(",");
  Serial.println(y);
  delay(10);
}
