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
const int sensorPin = A0; // Pin connected to sensor
//Variables
int sensorVal; // Analog value from the sensor
float res; //resistance value
float sensorRawToPhys(int raw){
 	// Conversion rule
 	float Vout = float(raw) * (VIN / float(1023));// Conversion analog to voltage
 	return map(Vout,0,5,0,90);
}
void setup(void) {
 	Serial.begin(9600);
}
void loop(void) {
 	sensorVal = analogRead(sensorPin);
 	res=sensorRawToPhys(sensorVal);
  Serial.print(millis());
  Serial.print(", ");
 	Serial.println(res); // the analog reading
 	delay(DELAY);
}
