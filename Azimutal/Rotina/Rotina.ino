#include <Stepper.h>
#include <Azimutal.h>


/*
 * 
 * pinRX0 == alavanca de posição
 * pinRX1 == alavanca de add90
 * pinRX2 == alavanca de add180
 * pinRX3 == alavanca de PANICO (busca zero)
 */
//  Azimutal(int pinRX0, int pinRX1, int pinRX2, int pinRX3,
//           int pinSM0, int pinSM1, int pinSM2, int pinSM3, int nbrSteps, int pinNS);
Azimutal azm(4, 7, 6, 5, 8, 9, 10, 11, 200, 13);

void setup() {
  azm.setSpeed(1200);
  azm.setConstPWM(1005.0, 1930.0);
  azm.setConstNH(400.0);
  azm.setRestrition(2);
  azm.setDriverConf(16);
  azm.setCalibrationsVars(0, 0, 0);
  
  Serial.begin(9600);
}

void loop() {
  bool c = azm.routine();
}

