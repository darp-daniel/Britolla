#include <Arduino.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <RF24.h>

RF24 radio(9,8);
const byte endereco[6] = "00001";
int leituraAD = 5;
float lido, ang, angulo; 
long tempo;

/*
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, endereco);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  }

void loop() {
  tempo = millis();
  if (radio.available()){
    radio.read(&angulo, sizeof(angulo));
    Serial.print(angulo);
    Serial.print(", ");
    Serial.println(tempo);
  }
  delay(1000);
}
*/


void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(endereco);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  pinMode(leituraAD, INPUT);
  }

void loop() {
  lido = analogRead(leituraAD);
  ang = ((260*(float)lido)/1023)+0.5;
  angulo = map(lido,0,1023,0,260);
  if(lido <= 28){
    angulo = ang*5;
  }
  if(lido > 28){
    angulo = (ang+20);
  }
  radio.write(&angulo, sizeof(angulo));
  delay(1000);
}
