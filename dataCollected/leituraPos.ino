float angle;
float volts;
#define pot1 A0;
#define pot2 A1;
int angMax = 270;

void setup(){
  Serial.begin(115200);
}

void loop(){
    
}

float calcularAng(int pin){
    unsigned long leitura = analogRead(pin);
    float prevAngulo = angMax * leitura*(4.9/1023);
    float angulo = angMax - prevAngulo;
    return angulo;
}
float calcularPosX(int pin1, int pin2){
    float ang1 = calcularAng(pin1);
    float ang2 = calcularAng(pin2);
    return 140*(cos(ang1) + cos(ang1+ang2));
}
float calcularPosX(int pin1, int pin2){
    float ang1 = calcularAng(pin1);
    float ang2 = calcularAng(pin2);
    return 140*(sin(ang1) + sin(ang1+ang2));
}