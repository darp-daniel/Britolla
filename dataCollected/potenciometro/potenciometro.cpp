/**
 * @file potenciometro.cpp
 * @brief Implementação dos métodos da classe Pot.
 * * Contém a lógica de filtro digital (IIR) e manipulação trigonométrica
 * para conversão de unidades.
 */

 #include "potenciometro.h"
 #include <Arduino.h>
 #include "math.h"
 
 Pot::Pot(int pin) {
   this->pino = pin;
   this->xn1 = 0;
   this->yn1 = 0;
   this->ultInter = 0;
 }
 
 void Pot::setAngRef() {
   xn = analogRead(pino) / 1023.0; // normaliza para 0–1
   this->angRef = xn * 270; // depende da calibração real (curso do pot)
   xn1 = xn;
   yn1 = angRef;
   Serial.print("Este é o ângulo de Referencia:");
   Serial.println(angRef);
 }
 
 int Pot::leitura() {
   unsigned long now = micros();
   // Verifica se o tempo de amostragem foi atingido
   if (now - ultInter >= interAmostra) {
     xn = analogRead(pino) / 1023.0; 
     
     /**
      * @brief Equação de Diferenças do Filtro Digital.
      * * A filtragem segue a estrutura IIR (Infinite Impulse Response):
      * \f$ y[n] = 0.072821 \cdot x[n] + 0.072821 \cdot x[n-1] + 0.854359 \cdot y[n-1] \f$
      * Onde:
      * - \f$ x[n] \f$: Leitura atual normalizada.
      * - \f$ y[n] \f$: Saída filtrada atual.
      */
     yn = 0.072821 * xn + 0.072821 * xn1 + 0.854359 * yn1;
     
     // Atualização das variáveis de estado (delay)
     yn1 = yn;
     xn1 = xn;
     ultInter = now;
     
     int angulo = 270 * yn;
     
     // Lógica para tratamento de wrap-around (passagem pelo zero/referência)
     if(angulo > angRef){ 
         return 360 + (angRef - angulo);
     }
     return angRef - angulo;
   }
   // Retorno padrão caso não seja tempo de amostragem (pode ser melhorado para retornar último valor válido)
   return 0; 
 }
 
 float Pot::angulo(int leitura) {
   int l = leitura;
   // Ajuste para manter o ângulo entre -180 e 180 graus
   if (l > 180)
     l -= 360;
   else if (l < -180)
     l += 360;
   
   return (l * M_PI) / 180.0;
 }