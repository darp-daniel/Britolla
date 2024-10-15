int leituraAD = 5;
int valor_lido, ang, angulo;
void setup() {
  // put your setup code here, to run once:
  pinMode(leituraAD, INPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  valor_lido = analogRead(leituraAD);
  ang = ((260*(float)valor_lido)/1023)+0.5;
  angulo = map(valor_lido,0,1023,0,260);
  if(valor_lido <= 28){
    angulo = ang*5;
  }
  if(valor_lido > 28){
    angulo = (ang+20);
  }
  Serial.print("O angulo atual é: ");
  Serial.println(angulo);
  Serial.print("O valor lido é: ");
  Serial.println(valor_lido);
  delay(100);
}
//26 ----- 1023
//angulo -- valor lido