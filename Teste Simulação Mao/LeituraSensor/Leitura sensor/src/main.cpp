#include <Arduino.h>

/* Potentiometer reading program */
// Constants
#define DELAY 500 // Delay between two measurements in ms
#define VIN 5 // V power voltage
#define R 10000 //ohm resistance value
// Parameters
/*
Pinos:
  1.OUT
  2.VCC
  3.GND
*/

// DEDOS 

#define INDICADOR = A1;
#define ANELAR = A2;
#define MEDIO = A3;
#define MINDINHO = A4;
#define DEDAO = A5;



//Variables
float vIndi;
float vMed;
float vAnel;
float vMindi;
float sensorRawToPhys(int pin){
 	float sensorVal = analogRead(pin)
	// Conversion rule
 	float Vout = sensorVal * (VIN / float(1023));// Conversion analog to voltage
 	return map(Vout,0,5,0,90);
}
void setup(void) {
 	Serial.begin(9600);
}
void loop(void) {
	Serial.print(millis());
	Serial.print(", ");
	vIndi = sensorRawToPhys(INDICADOR);
	Serial.print(vIndi);
	Serial.print(", ");
	vMed = sensorRawToPhys(MEDIO);
	Serial.print(vMed);
	Serial.print(", ");
	vAnel = sensorRawToPhys(ANELAR);
	Serial.print(vAnel);
	Serial.print(", ");
	vMindi = sensorRawToPhys(MINDINHO);
	Serial.println(vMindi);
	delay(DELAY);
}
