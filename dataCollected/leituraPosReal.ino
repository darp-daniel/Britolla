float angle;
#define pot1 A0
#define pot2 A1
int angMax = 270;
float angref1;
float angref2;

void setup(){
  Serial.begin(115200);
  angref1 = referenceAng(pot1);
  angref2 = referenceAng(pot2);
}

void loop(){
    float ang1 = calcularAng(pot1, angref1);
    float ang2 = calcularAng(pot2, angref2);
    Serial.print(ang1);
    Serial.print(",");
    Serial.println(ang2);
}

float calcularAng(int pin, float refAng){
    float leitura = analogRead(pin);
    float angulo = angMax * (leitura/1024);
    if(angulo > refAng){
        360 - (angulo - refAng);
    }
    return angulo - refAng;
}

float referenceAng(int pin){
    float leitura = analogRead(pin);
    float prevAngulo = angMax * (leitura/1024);
    return prevAngulo;
}