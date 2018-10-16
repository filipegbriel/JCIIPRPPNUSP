#include <Stepper.h>
#include <Azimutal.h>

/*
 * pinRX0 == alavanca de posição
 * pinRX1 == alavanca de add90
 * pinRX2 == alavanca de add180
 * pinRX3 == alavanca de PANICO (busca zero)
 */
//  Azimutal(int pinRX0, int pinRX1, int pinRX2, int pinRX3,
//           int pinSM0, int pinSM1, int pinSM2, int pinSM3, int nbrSteps, int pinNS1, int pinNS2);
Azimutal azm(4, 7, 6, 5, 8, 9, 10, 11, 200, 13, 12);

void setup() {
  azm.setSpeed(2300);                               //Velocidade de giro
  azm.setConstPWM(1005.0, 1930.0);                  //Minimo e maximo do pwm
  azm.setConstNH(800.0);                            //buraco do zero pra não ficar tremulo em torno dele
  azm.setRestrition(8.2);                           //restrição de movimento, quanto maior, mais anda (float)
  azm.setDriverConf(16);                            //configuração de passos do driver
  azm.setCalibrationsVars(20000,-11500,11500);      //soma final pra acertar posição limite
  azm.setNullVerif(true);                           //Usar sensores do zero ou nao
  azm.setNullSteps(150);                            //Passos que vai dando ao chegar perto do zero
  azm.setNullPosSteps(800);                         //Passos que dá ao final da operação de busca, pra acertar a posição no meio dos sensores
  azm.setNullTimeSearch(5);                         //delay entre passos do zero em busca


  /*
    Ao aumentar restrition precisa aumentar nullhole, a velocidade e passos do zero.
    As calibration vars são:
    1ª == conf de add de 180
    2ª == conf de add de 90 pra esquerda
    3ª == conf de add de 90 pra direita (pode ser que esteja trocado)
    Elas somam ao final da operação visando acertar a posição do azm
  */
  
  /*
  CONFIGURAÇÕES DUNA 2018
  azm.setSpeed(1200);                               //Velocidade de giro
  azm.setConstPWM(1005.0, 1930.0);                  //Minimo e maximo do pwm
  azm.setConstNH(400.0);                            //buraco do zero pra não ficar tremulo em torno dele
  azm.setRestrition(2);                             //restrição de movimento, quanto maior, mais anda (float)
  azm.setDriverConf(16);                            //confgração de passos do driver
  azm.setCalibrationsVars(20000,-11500,11500);      //soma final pra acertar posição limite
  azm.setNullVerif(true);                           //Usar sensores do zero ou nao
  azm.setNullSteps(150);                            //Passos que vai dando ao chegar perto do zero
  azm.setNullPosSteps(1000);                        //Passos que dá ao final da operação de busca, pra acertar a posição no meio dos sensores
  azm.setNullTimeSearch(10);                        //delay entre passos do zero em busca
  */
  
  Serial.begin(9600);
}

void loop() {
  Serial.print("\n-- Rotina Completa --\n");
  bool c = azm.routine();
}

