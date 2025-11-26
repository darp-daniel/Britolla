#include "potenciometro.h"
#include "math.h"

Pot pot1(A0);
Pot pot2(A1);
Pot pot3(A2); // Verificar se esse pino tá livre no arduino

// Variáveis para guardar a última posição (x,y,z) que foi impressa.
// Iniciamos com um valor que provavelmente será diferente do primeiro cálculo.
float ultimoX = -999.0;
float ultimoY = -999.0;
float ultimoZ = -999.0;

int L = 14;
short d = 0;

// (Opcional, mas recomendado) Define uma pequena tolerância.
// O valor só será impresso se a mudança for maior que este valor.
// Isso evita impressões desnecessárias por causa de ruído elétrico mínimo.
const float TOLERANCIA = 0.5;

void setup() {
  Serial.begin(115200);
  delay(100);
  pot1.setAngRef();
  pot2.setAngRef();
  pot3.setAngRef();
  delay(5000);
}

void loop() {
  // 1. A leitura e os cálculos continuam sendo feitos em toda iteração do loop.
  // Isso garante que você sempre tenha os dados mais recentes, sem alterar a lógica de leitura.
  int ang1 = pot1.leitura();
  int ang2 = pot2.leitura();
  int ang3 = pot3.leitura();

  float rad1 = pot1.angulo(ang1);
  float rad2 = pot2.angulo(ang2);
  float rad3 = pot3.angulo(ang3);

  //float rad2 = 0.0;
  //float rad3 = 0.0;

  // (Cálculo original)
  //float x = L * ((cos(rad1)*cos(rad2)) + (cos(rad1)*cos(rad2)*cos(rad3)-cos(rad1)*sin(rad2)*sin(rad3)));
  //float y = L * ((sin(rad1)*cos(rad2)) + (sin(rad1)*cos(rad2)*cos(rad3)-sin(rad1)*sin(rad2)*sin(rad3)));
  //float z = L * ((sin(rad2))+(sin(rad2)*cos(rad3)+cos(rad2)*sin(rad3))) + d;

  // (Sugestão do deepseek) Usando identidades trigonométricas
  float x = cos(rad1) * L *(cos(rad2) + cos(rad2+rad3));
  float y = sin(rad1) * L * (cos(rad2) + cos(rad2+rad3));
  float z = L * (sin(rad2) + sin(rad2 + rad3)) + d;

  // 2. A CONDIÇÃO é aplicada aqui, ANTES de imprimir.
  // Comparamos o valor recém-calculado de 'x', 'y' e 'z' com o último que foi impresso.
  // A função fabs() calcula o valor absoluto (módulo) da diferença.
  if (fabs(x - ultimoX) > TOLERANCIA || fabs(y - ultimoY) > TOLERANCIA || fabs(z - ultimoZ) > TOLERANCIA) {

    // 3. Se a mudança for maior que a tolerância, imprimimos.
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.print(",");
    Serial.println(z);

    // 4. E SÓ ENTÃO, atualizamos os últimos valores conhecidos.
    ultimoX = x;
    ultimoY = y;
    ultimoZ = z;
  }
  
  delay(10);
}