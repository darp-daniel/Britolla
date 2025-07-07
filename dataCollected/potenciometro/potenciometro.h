#ifndef POT_H
#define POT_H

class Pot {
  private:
    int pino;
    float angRef;
    const int interAmostra = 2500;
    float xn1;
    float yn1;
    unsigned long ultInter;
    float xn;
    float yn;

  public:
    Pot(int pin);
    float leitura();
    void setAngRef();  // ← Moveu para a seção pública
    float angulo();
};

#endif
