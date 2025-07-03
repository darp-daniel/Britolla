#define pin A0
const int interAmostra = 2500;
float yn1 = 0;
float xn1 = 0;
unsigned long ultimoInt = 0;
int leitura = 0;

void setup(){
  Serial.begin(115200);
}

void loop(){
  unsigned long now = micros();
  if(now - ultimoInt >= interAmostra){
    float xn = analogRead(pin);
    float yn = 0.072821 * xn + 0.072821 * xn1 + 0.854359 * yn1;
    yn1 = yn;
    xn1 = xn;
    ultimoInt = now;
    Serial.println(yn);
  }
}