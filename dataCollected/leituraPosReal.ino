#define pot1 A0
#define pot2 A1
float xn1 = 0;
float yn1 = 0;
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
    delay(10);
}

float filter(int pin){
    float xn = analogRead(pin);
    float output = 0.939*yn1 + 0.0305*xn + 0.305*xn1;

    xn1 = xn;
    yn1 = output;

    return output
}

float calcularAng(int pin, float refAng){
    float leitura = filter(pin);
    float angulo = angMax * (leitura/1024);
    if(angulo > refAng){
        return 360 + (refAng - angulo);
    }
    return refAng-angulo;
}

float referenceAng(int pin){
    float leitura = analogRead(pin);
    float prevAngulo = angMax * (leitura/1024);
    return prevAngulo;
}