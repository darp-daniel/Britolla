#ifndef POT_H
#define POT_H


class Pot {
  private:
    int pino;
    int angRef;
    const int interAmostra = 2500;
    float xn1;
    float yn1;
    unsigned long ultInter;
    float xn;
    float yn;

  public:
    Pot(int pin);
    int leitura();
    void setAngRef();  // ← Moveu para a seção pública
    float angulo(int leitura);
};

#endif
