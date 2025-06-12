float prevAng = 0.00;
float angle;
float volts;
#define pin A0
int angMax = 270;

void setup(){
  Serial.begin(115200);
}

void loop(){
  float leitura = analogRead(pin);
  volts = leitura*(4.9/1023);
  angle = angMax * leitura/1023;

  if(abs(prevAng - angle) < 0.){
    Serial.print("Volts:");
    Serial.print(volts);
    Serial.print(", Ângulo:");
    Serial.println(angle);
    prevAng = angle;
  }
}