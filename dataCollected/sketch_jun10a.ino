#define pin A0
const int interAmostra = 2500;
unsigned long ultimoInt = 0;
int leitura = 0;

void setup(){
  Serial.begin(115200);
}

void loop(){
  unsigned long now = micros();
  if(now - ultimoInt >= interAmostra){

    leitura = analogRead(pin);
    Serial.println(leitura);
    ultimoInt = now;
  }
}