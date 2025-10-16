#include "potenciometro.h"
#include "math.h"

Pot pot1(A0);
Pot pot2(A1);

// Variáveis para guardar a última posição (x,y) que foi impressa.
// Iniciamos com um valor que provavelmente será diferente do primeiro cálculo.
float ultimoX = -999.0;
float ultimoY = -999.0;

int L = 14;

// (Opcional, mas recomendado) Define uma pequena tolerância.
// O valor só será impresso se a mudança for maior que este valor.
// Isso evita impressões desnecessárias por causa de ruído elétrico mínimo.
const float TOLERANCIA = 0.5;

void setup() {
  Serial.begin(115200);
  delay(100);
  pot1.setAngRef();
  pot2.setAngRef();
  delay(5000);
}

void loop() {
  // 1. A leitura e os cálculos continuam sendo feitos em toda iteração do loop.
  // Isso garante que você sempre tenha os dados mais recentes, sem alterar a lógica de leitura.
  int ang1 = pot1.leitura();
  int ang2 = pot2.leitura();

  float rad1 = pot1.angulo(ang1);
  float rad2 = pot2.angulo(ang2);

  float x = L * (cos(rad1) + cos(rad1 + rad2));
  float y = L * (sin(rad1) + sin(rad1 + rad2));

  //float yH = L * (cos(rad1));
  //float xH = L * (sin(rad1));

  // 2. A CONDIÇÃO é aplicada aqui, ANTES de imprimir.
  // Comparamos o valor recém-calculado de 'x' e 'y' com o último que foi impresso.
  // A função fabs() calcula o valor absoluto (módulo) da diferença.
  if (fabs(x - ultimoX) > TOLERANCIA || fabs(y - ultimoY) > TOLERANCIA) {

    // 3. Se a mudança for maior que a tolerância, imprimimos.
    Serial.print(x);
    Serial.print(",");
    Serial.println(y);
    //Serial.print(",");
    //Serial.print(xH);
    //Serial.print(",");
    //Serial.println(yH);

    // 4. E SÓ ENTÃO, atualizamos os últimos valores conhecidos.
    ultimoX = x;
    ultimoY = y;
  }
  
  delay(10);
}