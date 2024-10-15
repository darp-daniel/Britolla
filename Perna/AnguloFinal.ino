volatile boolean TurnCLK; 

volatile boolean TurnDT;   

long tempoRuido = 0;
float delayRuido = 0.001; 


const int PinCLK=2;    

const int PinDT=3; 

int dt; 

int clk; 

int senRead = 0;     

 

void rotaryCLK(){ 
  if(millis() - tempoRuido > delayRuido){
  TurnCLK = true;
  tempoRuido = millis(); 
} 
}
void rotaryDT(){ 
if(millis() - tempoRuido > delayRuido){
  TurnDT = true;
  tempoRuido = millis(); 
  }
} 

 

void setup()  { 

  Serial.begin(2000000);   

  attachInterrupt(digitalPinToInterrupt(PinCLK),rotaryCLK,CHANGE); 

  attachInterrupt(digitalPinToInterrupt(PinDT),rotaryDT,CHANGE); 

} 

 

void loop ()  { 

 

  if (TurnCLK)  { 
    clk = digitalRead(PinCLK);  

    TurnCLK = false; 

    if(clk == 1){ 

      if(digitalRead(PinDT) == 0){ 

        senRead++; 

         

      }else{ 

        senRead--; 

         

      } 

    }else{ 

      if(digitalRead(PinDT) == 0){ 

        senRead--; 

         

      }else{ 

        senRead++; 

         

      } 

    } 

  } 

  if(TurnDT){ 

    dt = digitalRead(PinDT); 

    TurnDT = false; 

    if(dt == 1){ 

      if(digitalRead(PinCLK) == 0){ 

        senRead--; 

         

      }else{ 

        senRead++; 

         

      } 

    }else{ 

      if(digitalRead(PinCLK) == 0){ 

        senRead++; 

           

      }else{ 

        senRead--; 

         

      } 

    } 

  }
  Serial.println(senRead);
}