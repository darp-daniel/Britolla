float angle;
float volts;
#define pot1 A0;
#define pot2 A1;
int angMax = 270;

void setup(){
  Serial.begin(115200);
}

void loop(){
    float l1 = analogRead(pot1);
    float l2 = analogRead(pot2);
    float x = calcularPosX(pot1, pot2);
    float y = calcularPosY(pot1, pot2);
    Serial.print();
    Serial.print(",");
    Serial.println();
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
float calcularPosY(int pin1, int pin2){
    float ang1 = calcularAng(pin1);
    float ang2 = calcularAng(pin2);
    return 140*(sin(ang1) + sin(ang1+ang2));
}