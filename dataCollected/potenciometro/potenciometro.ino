/**
 * @file potenciometro.ino
 * @author Equipe All Blue / Seu Nome
 * @brief Controlador Principal para Cinemática Direta via Potenciômetros.
 * @version 1.0
 * * @details Este programa lê três potenciômetros, converte seus valores para radianos
 * e calcula a posição cartesiana (X, Y, Z) da ponta de um manipulador robótico
 * utilizando equações de Cinemática Direta. Os dados são enviados via Serial
 * apenas se houver variação significativa (Tolerância).
 */

 #include "potenciometro.h"
 #include "math.h"
 
 /// Instância do potenciômetro para a Junta 1 (Base).
 Pot pot1(A0);
 /// Instância do potenciômetro para a Junta 2 (Ombro).
 Pot pot2(A1);
 /// Instância do potenciômetro para a Junta 3 (Cotovelo).
 Pot pot3(A2); 
 
 // Variáveis para guardar a última posição (x,y,z) impressa.
 float ultimoX = -999.0; /**< Última coordenada X enviada via serial. */
 float ultimoY = -999.0; /**< Última coordenada Y enviada via serial. */
 float ultimoZ = -999.0; /**< Última coordenada Z enviada via serial. */
 
 int L = 14; /**< Comprimento do elo do manipulador (unidade arbitrária, ex: cm). */
 short d = 0; /**< Deslocamento vertical ou offset da base. */
 
 /** * @brief Tolerância para envio de dados.
  * Define a variação mínima necessária nas coordenadas para que um novo pacote
  * serial seja enviado, evitando saturação do canal com ruído.
  */
 const float TOLERANCIA = 0.5;
 
 /**
  * @brief Configuração inicial do sistema.
  * Inicializa a comunicação serial e define os ângulos de referência (homing)
  * dos potenciômetros.
  */
 void setup() {
   Serial.begin(115200);
   delay(100);
   
   // Rotina de calibração inicial
   pot1.setAngRef();
   pot2.setAngRef();
   pot3.setAngRef();
   delay(5000);
 }
 
 /**
  * @brief Loop principal de controle.
  * * Executa ciclicamente:
  * 1. Leitura dos sensores (ângulos brutos).
  * 2. Conversão para radianos.
  * 3. Cálculo da Cinemática Direta.
  * 4. Verificação de tolerância e envio Serial.
  */
 void loop() {
   // 1. Leitura e Filtragem
   int ang1 = pot1.leitura();
   int ang2 = pot2.leitura();
   int ang3 = pot3.leitura();
 
   // 2. Conversão para Radianos (\theta_1, \theta_2, \theta_3)
   float rad1 = pot1.angulo(ang1);
   float rad2 = pot2.angulo(ang2);
   float rad3 = pot3.angulo(ang3);
 
   /**
    * @brief Cálculo da Cinemática Direta.
    * * As posições cartesianas são calculadas baseadas nas identidades trigonométricas:
    * * \f$ x = L \cdot \cos(\theta_1) \cdot (\cos(\theta_2) + \cos(\theta_2 + \theta_3)) \f$
    * * \f$ y = L \cdot \sin(\theta_1) \cdot (\cos(\theta_2) + \cos(\theta_2 + \theta_3)) \f$
    * * \f$ z = L \cdot (\sin(\theta_2) + \sin(\theta_2 + \theta_3)) + d \f$
    */
   float x = cos(rad1) * L *(cos(rad2) + cos(rad2+rad3));
   float y = sin(rad1) * L * (cos(rad2) + cos(rad2+rad3));
   float z = L * (sin(rad2) + sin(rad2 + rad3)) + d;
 
   // 3. Verificação de variação (Deadband)
   if (fabs(x - ultimoX) > TOLERANCIA || fabs(y - ultimoY) > TOLERANCIA || fabs(z - ultimoZ) > TOLERANCIA) {
 
     // Envio no formato CSV: x,y,z
     Serial.print(x);
     Serial.print(",");
     Serial.print(y);
     Serial.print(",");
     Serial.println(z);
 
     // Atualiza estado anterior
     ultimoX = x;
     ultimoY = y;
     ultimoZ = z;
   }
   
   delay(10);
 }