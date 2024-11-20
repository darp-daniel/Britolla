#include <Arduino.h>

/* Potentiometer reading program */
// Constants

#define DELAY 500 // Delay between two measurements in ms
#define VIN 5 // V power voltage
#define R 10000 //ohm resistance value
// Parameters
const int indicador = A0;
const int anelar = A1;
const int meio = A2;
const int mindinho = A3; 
const int botao = A4;// Pin connected to sensor
const int contador = 0;
//Variables
int sensorVal; // Analog value from the sensor
float voltage; //resistance value
float angIndicador;
float angMeio;
float angAnelar;
float angMindinho;
float mindmin;
float minmax;
float indmax;
float indmin;
float meiomax;
float meiomin;
float anelmin;
float anelmax;
float pinomin;
float pinomax;
float pino ;

float converte(void){

  angIndicador = constrain(analogRead(indicador),610,995);
  angMeio = constrain(analogRead(meio),241,600);
  angAnelar = constrain(analogRead(anelar),370,980);
  angMindinho = constrain(analogRead(mindinho),540,710);
  angIndicador = map(angIndicador,610,995,0,95);
  angMeio = map(angMeio,241,600,0,105);
  angAnelar = map(angAnelar,370,980,0,112.5);
  angMindinho = map(angMindinho,540,710,0,122.5);
 	Serial.print(angIndicador); // the analog reading
    Serial.print(", ");
 	Serial.print(angMeio);
    Serial.print(", ");
 	Serial.print(angAnelar);
    Serial.print(", ");
 	Serial.println(angMindinho);
}

void setup(void) {
 	Serial.begin(9600);
}
void loop(void) {


 	delay(DELAY);

    converte();
}
float sensor(int sensorPin){
 	sensorVal = analogRead(sensorPin);
  voltage = sensorVal * (VIN/1023);
 	return voltage;

}