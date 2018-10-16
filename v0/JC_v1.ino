
/*
 Este programa controla o motor de passo NEMA 23 usado para rotacionar o azimutal do 
 barco João Candido 2, em agosto de 2017.

 Feito por Henrique Martinez Rocamora
           Filipe Gabriel Santos
           Equipe PoliNautico
           Escola Politécnica da USP 2017
 */

//CONFIGURAÇÕES FÍSICAS
#define   stepsPerRevolution  200.0       //Número de passos fixo do motor.
#define   confDriver          0.1         //Configuração do driver, (ex Giro de 1/4: 0.25).
#define   turnVelocity        500.0       //Velocidade de giro em RPM.

//CONFIGURAÇÕES DE PWM
#define   maxPulse            1880.0    
#define   minPulse            1030.0

//CONFIGURAÇÕES DO ARDUINO
#define   pinoPWM_1           3         //Pino PWM da alavanca de controle de posição.
#define   pinoPWM_2           5         //Pino PWM do switch de prisão angular.
#define   pinoPWM_3           6         //Pino PWM do switch de adição 180°.
#define   pinEOC              12
#define   pinonPWM            4

//CONFIGURAÇÕES DE CONTROLE
#define   maxAngle            155.0       //Ângulo máximo de controle.
#define   minAngle           -157.0       //Ãngulo mínimo de controle.
#define   secInt              0.15        //Filtra o movimento em x%
#define   filterSize          3.0         //Tamanho do filtro: MUDAR INDICE DE FILTER
#define   delayMin            100.0       //Tempo mínimo de espera
#define   tremblingFilter     250.0        //Quantização maior do deltaStep
#define   straigthPos         300.0        //Posição de rotação no próprio eixo
#define   backwardsPos        535.0        //Posição de rotação no próprio eixo

const float nullHole = abs((maxAngle - minAngle) * secInt);
float filter[3];
int currentStep  = 0;


#include <Stepper.h>
Stepper myStepper(stepsPerRevolution / confDriver, 8, 9, 10, 11); //Usamos os pinos 8 a 11 na placa UNO.

//Função map dedicada a lógica float:
float map_f(float x, float in_min, float in_max, float out_min, float out_max)
{
  float p = ((x - in_min) * (out_max - out_min) / (in_max - in_min)) + out_min;
  if (p > out_max) p = out_max;
  if (p < out_min) p = out_min;
  return p;
}


//Função que move o azimutal para o angulo requerido
int moveToAngle(float targetAngle)                   
{ 
  
    //Devemos transformar o ângulo devido a quantização do movimento:
    //O movimento do motor de passo está quantizado pela quantidade de passos.
    //realSteps = stepsPerRevolution / confDriver;
    //Temos aqui o número de passos. Os angulos plausíveis são x = realSteps*n/360, sendo n inteiro até 360/realSteps
    
    int targetStep = floor ((targetAngle * stepsPerRevolution) / (360 * confDriver));

    int deltaStep = targetStep - currentStep;

  //Filtro de tamanho treblingFilter
   if(abs(deltaStep) < tremblingFilter)  return currentStep;


    for(int i = 0; i < abs(deltaStep); i++)
    {
      if(deltaStep < 0) myStepper.step(1);
      else myStepper.step(-1);
      
      delay(1);
    }

    return targetStep;
}



bool amIReversed = false;

//verifica se o azimutal está em zero de acordo com sensor de fim de curso colocado no pino pin
bool isAtZero()
{  

  if(digitalRead(pinEOC) == LOW)       return true;
  else   return false;
}

void gimmeNULL()
{
   while(!isAtZero())
  {

    if(currentStep > 0){
      myStepper.step(1);
    }
    else{
      myStepper.step(-1);
    }
    delay(1);
  }
  return;
}

int shouldIReverse(void)
{
  if(amIReversed) return backwardsPos;
  else            return 0;
}

float reading[2] = {0 , 0};

float readsPWM(int pin, float mxPulse, float mnPulse)
{
  unsigned long x_long = pulseIn(pin, HIGH);
  int x = (int) x_long;
  return map_f((float)x, minPulse, maxPulse, mnPulse, mxPulse);
}

float readPWM(int pin, float mxPulse, float mnPulse)
{ 
  unsigned long x_long = pulseIn(pin, HIGH);
  int x = (int) x_long;

  while(x > maxPulse || x < minPulse)  x = (int) pulseIn(pin, HIGH);

  return map_f((float)x, mnPulse, mxPulse, minAngle, maxAngle);
}

float readAngle()
{
  float x = readPWM(pinoPWM_1, maxPulse, minPulse);
  bool enable = true;

    //cria o buraco do zero
    if(-(nullHole) < x  || x < nullHole) x = 0;

    //Filtra leitura de acordo com filterSize
    while(enable)
    {
      for(int i = 0; i < filterSize; i++)
      {
        if( x <= filter[i] - filter[i]*secInt || x >= filter[i] + filter[i]*secInt ) enable = false;

        x = readPWM(pinoPWM_1, maxPulse, minPulse);
      }
      
    }
  return x + shouldIReverse();
}


bool reading2[2] = {false, false};

bool ctrlConf_3(void)
{
  if(readsPWM(pinonPWM, 100, 0) < 20)    reading2[0] = true;
  else                                   reading2[0] = false;

  bool action = false;
  
  if(reading2[0] != reading2[1])           
  {
   
    gimmeNULL();
    return true;
  }

  reading2[1] = reading2[0];
  return false;
}



//Adição de 180
bool ctrlConf_2(bool dontactivate)
{
  if(dontactivate) return true;
  
  if(readsPWM(pinoPWM_2, 100, 0) < 20)    reading[0] = 0;
  else                                    reading[0] = 1;
  
  bool action = false;
  
  if(reading[0] != reading[1])            action = true;

  if(action)  amIReversed = true;
  else amIReversed = false;
  return action;
}

//Prisão angular
bool ctrlConf_1(bool dontactivate)
{ 
  if(dontactivate)  return false;

  if(readsPWM(pinoPWM_3, 100, 0) > 50)
  {
    if(readsPWM(pinoPWM_1, 100, 0)  < 20) currentStep = moveToAngle(-straigthPos + 50 );
    else                                  currentStep = moveToAngle(straigthPos - 10);
      
    return true;
  }
  else return false;
}


/*
bool digitalFilter[3] = {0, 0, 0};

void refreshDigitalFilter(bool x)
{
    for(int i = 0; i < filterSize; i++)
  {
    if (i != filterSize - 1) digitalFilter[i] = filter[i + 1];
    else        digitalFilter[i] = x;
  }
}
*/



//Atualiza o filtro
void refreshFilter(int x)
{
  for(int i = 0; i < filterSize; i++)
  {
    if (i != filterSize - 1)
    {
      filter[i] = filter[i + 1];
    }
    else
    {
      filter[i] = x;
    }
  }
}

void supposedNull(void)
{
  if(abs(currentStep) < tremblingFilter)   
  {
     delay(10);
    
    moveToAngle(0);
    delay(10);
    moveToAngle(0);
   
  }

  
return;
}

bool priority = false;  

void setup() {

  myStepper.setSpeed(turnVelocity); 
  pinMode(pinEOC, INPUT_PULLUP);
  //gimmeNULL();
}

void loop() {
  priority = ctrlConf_1(ctrlConf_2(ctrlConf_3()));
  if (!priority)
  {
      currentStep = moveToAngle(readAngle());
      refreshFilter(currentStep);
  }

  if(abs(currentStep) < abs(tremblingFilter)) gimmeNULL();
  
}



