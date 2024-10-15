#include <RF24.h> //Inclusão da biblioteca do módulo
#include <nRF24L01.h> //Inclusão da biblioteca do módulo
#include <printf.h>

//RECEPTOR
//verde CE 8
//laranja CSN 9

RF24 radio(8,9); //Definição dos pins 'CE' e 'CSN'
const byte address[6] = "00001"; //Definição do endereço a ser utilizado pelo os módulos
int valor; //Tipo do valor a ser recebido pelo arduino

void setup() {
  Serial.begin(115200);
  radio.begin(); //Início de comunicação
  radio.openReadingPipe(0,address); // Início da linha de recepção de nº 0 conforme o dado
  radio.setPALevel(RF24_PA_MIN); //Configuração do nível de potência de transmissão do módulo
  radio.startListening(); // Início da recepção

}

void loop() {
  // put your main code here, to run repeatedly:
  if (radio.available()) { //Checa se existe alguma transmissão disponível na linha de rádio endereçada
    radio.read(&valor, sizeof(valor)); // Ler o valor recebido e armazena na variável pedida
    Serial.println(valor);
  }
}
