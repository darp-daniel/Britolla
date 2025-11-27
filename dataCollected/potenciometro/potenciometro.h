/**
 * @file potenciometro.h
 * @author Equipe All Blue / Seu Nome
 * @date 2025-11-27
 * @brief Definição da classe Pot para leitura de sensores angulares.
 * * Este arquivo contém a definição da classe Pot, responsável por abstrair
 * a leitura de potenciômetros, aplicar filtragem digital e converter
 * leituras analógicas em ângulos relativos.
 */

 #ifndef POT_H
 #define POT_H
 
 /**
  * @class Pot
  * @brief Classe para manuseio e filtragem de sinal de potenciômetros.
  * * A classe implementa um filtro digital para suavizar o ruído da leitura
  * analógica e métodos para calibração de referencial zero.
  */
 class Pot {
   private:
     /** @brief Pino analógico do Arduino conectado ao potenciômetro. */
     int pino;
     
     /** @brief Valor de ângulo de referência (offset) para calibração. */
     int angRef;
     
     /** @brief Intervalo de tempo entre amostras em microssegundos (Ts). */
     const int interAmostra = 2500;
     
     /** @brief Armazena a entrada anterior do filtro (x[n-1]). */
     float xn1;
     
     /** @brief Armazena a saída anterior do filtro (y[n-1]). */
     float yn1;
     
     /** @brief Timestamp da última iteração de leitura. */
     unsigned long ultInter;
     
     /** @brief Valor atual normalizado da entrada (x[n]). */
     float xn;
     
     /** @brief Valor atual filtrado da saída (y[n]). */
     float yn;
 
   public:
     /**
      * @brief Construtor da classe Pot.
      * @param pin O número do pino analógico (ex: A0, A1).
      */
     Pot(int pin);
 
     /**
      * @brief Realiza a leitura, filtragem e cálculo do ângulo.
      * * Implementa um filtro de equação de diferenças e controla a taxa de amostragem.
      * @return O ângulo calculado em graus relativo à referência, ou ajustado para 360 se houver estouro.
      */
     int leitura();
 
     /**
      * @brief Define o ângulo de referência (Zero).
      * * Lê o valor atual do potenciômetro e o define como o ponto zero (home)
      * para as leituras subsequentes.
      */
     void setAngRef();
 
     /**
      * @brief Converte o valor inteiro da leitura para radianos.
      * * Normaliza o ângulo para o intervalo [-PI, PI].
      * @param leitura O valor em graus retornado pelo método leitura().
      * @return O ângulo correspondente em radianos.
      */
     float angulo(int leitura);
 };
 
 #endif