float angle;
float volts;
#define pot1 A0
#define pot2 A1
int angMax = 4.712388;

void setup(){
  Serial.begin(115200);
}

void loop(){
    float l1 = calcularAng(pot1);
    float l2 = analogRead(pot2);
    float x = calcularPosX(pot1, 0);
    float y = calcularPosY(pot1, 0);
    Serial.print(x);
    Serial.print(",");
    Serial.println(y);
}

float calcularAng(int pin){
    float leitura = analogRead(pin);
    float prevAngulo = angMax * (leitura/1024);
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