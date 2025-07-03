#define pin A0
const int interAmostra = 2500
unsigned long ultimoInt = 0;
float leitura = 0;

void setup(){
  Serial.begin(115200);
}

void loop(){
  unsigned long now = micros();
  if(now - ultimoInt >= interAmostra){
    Serial.print(now - ultmoInt);
    Serial.print(", ")
    ultimoInt = now;

    leitura = analogRead(pin);
    Serial.println(leitura);
  }
}