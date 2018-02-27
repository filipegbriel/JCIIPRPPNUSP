#include "Azimutal.h"
/*
 * Azimutal.h - Biblioteca de um azimutal  - Vers�o 1.1
 *
 * Funcionamento original	(0.0)   por Filipe Gabriel Santos e Henrique Martinez Rocamora.
 * Biblioteca para arduino	(0.1)	por Filipe Santos, Anna Queiroz
 *
 * Esta biblioteca � para o uso privado da institui��o hoje (2017) nomeada por PoliN�utico - grupo de
 * extens�o acad�mica da Escola Politecninca da Universidade de S�o Paulo. Seu uso n�o est� autorizado 
 * para qualquer outra pessoa que n�o seja, ou n�o tenha sido, associada ao grupo.
 *
 * Universidade de S�o Paulo, S�o Paulo, S�o Paulo, Brasil. Novembro de 2017
 * 
 * Esta biblioteca controla um motor de passo ligado a uma propuls�o azimutal do barco Jo�o C�ndido II.
 * Sua comunica��o com o receptor usa 3 canais e um sensor de fim de curso, o qual verifica se o barco est�
 * em zero com a normal.
 * 
 * pinRX0 == alavanca de posição
 * pinRX1 == alavanca de add90
 * pinRX2 == alavanca de add180
 * pinRX3 == alavanca de PANICO (busca zero)
 */


Azimutal::Azimutal(int pinRX0, int pinRX1, int pinRX2, int pinRX3, int pinSM0, int pinSM1, int nbrSteps, int pinNS) : Stepper(nbrSteps, pinSM0, pinSM1)
{
	this->pinRX0 = pinRX0;
	pinMode(this->pinRX0, INPUT);
	this->pinRX1 = pinRX1;
	pinMode(this->pinRX1, INPUT);
	this->pinRX2 = pinRX2;
	pinMode(this->pinRX2, INPUT);
	this->pinRX3 = pinRX3;
	pinMode(this->pinRX3, INPUT);
	this->pinNS = pinNS;
	pinMode(this->pinNS, INPUT_PULLUP);

	this->lastStep = 0;
	this->Stepsum = 0;
}
Azimutal::Azimutal(int pinRX0, int pinRX1, int pinRX2, int pinRX3, int pinSM0, int pinSM1, int pinSM2, int pinSM3, int nbrSteps, int pinNS) : Stepper(nbrSteps, pinSM0, pinSM1, pinSM2, pinSM3)
{
	this->pinRX0 = pinRX0;
	pinMode(this->pinRX0, INPUT);
	this->pinRX1 = pinRX1;
	pinMode(this->pinRX1, INPUT);
	this->pinRX2 = pinRX2;
	pinMode(this->pinRX2, INPUT);
	this->pinRX3 = pinRX3;
	pinMode(this->pinRX3, INPUT);
	this->pinNS = pinNS;
	pinMode(this->pinNS, INPUT_PULLUP);

	this->lastStep = 0;
	this->Stepsum = 0;
}
Azimutal::Azimutal(int pinRX0, int pinRX1, int pinRX2, int pinRX3, int pinSM0, int pinSM1, int pinSM2, int pinSM3, int pinSM4, int nbrSteps, int pinNS) : Stepper(nbrSteps, pinSM0, pinSM1, pinSM2, pinSM3, pinSM4)
{
	this->pinRX0 = pinRX0;
	pinMode(this->pinRX0, INPUT);
	this->pinRX1 = pinRX1;
	pinMode(this->pinRX1, INPUT);
	this->pinRX2 = pinRX2;
	pinMode(this->pinRX2, INPUT);
	this->pinRX3 = pinRX3;
	pinMode(this->pinRX3, INPUT);
	this->pinNS = pinNS;
	pinMode(this->pinNS, INPUT_PULLUP);

	this->lastStep = 0;
	this->Stepsum = 0;
}


bool Azimutal::setConstPWM(float minPWM, float maxPWM)
{
	this->minPWM = minPWM;
	this->maxPWM = maxPWM;

	return true;
}
bool Azimutal::setConstNH(float nullHole)
{
		this->nullHole = nullHole;
		return true;	
}
bool Azimutal::setRestrition(float res)
{
	this->restrition = res;
	return true;
}
bool Azimutal::setDriverConf(int num)
{
	this->driverConf = num;
	return true;
}
bool Azimutal::setCalibrationsVars(int eins, int zwei, int drei)
{
	this->calibration1 = eins;
	this->calibration2 = zwei;
	this->calibration3 = drei;

	return true;
}

//Métodos privados
float Azimutal::readPWM(int pin)
{
	int x = (int)pulseIn(pin, HIGH);
	if (x > this->maxPWM) x = maxPWM;
	else if (x < this->minPWM) x = minPWM;

	return map_f((float)x, minPWM, maxPWM, 0.0, 100.0);
}
float Azimutal::map_f(float number, float minI, float maxI, float minF, float maxF)
{
	float p = ((number - minI) * (maxF - minF) / (maxI - minI)) + minF;
	if (p > maxF) p = maxF;
	if (p < minF) p = minF;
	return p;
}

bool Azimutal::filter(int x)
{	//RETORNA FALSO SE X' FOR TREMULAÇÃO DE X
	
	//Discretização do intervalo
	bool answer = true;
	if (x <= this->lastStep + this->un && x >= this->lastStep - this->un) answer = false;
	else															  this->lastStep = x;
	
	return answer;
}
int	 Azimutal::readStep(void)
{
	//essa função le o pwm e o transforma em um passo entre o passo minimo e o passo maximo.
	float x = readPWM(pinRX0);
	x = map_f(x, 0.0, 100.0, -100.0, 100.0);
	//transformando de porcentagem pra passo
	return this->getNbrSteps() * this->driverConf * this->restrition * x / 100; //o resultado eh truncado pelo casting automatico do compilador
		
	/**************************************************
	Este programa trata sempre de variacoes. 
	Ele vai porcentagem do total pra um lado ou pro outro, 
	nunca x passos. O sinal indica horario ou anti-horario.
	**************************************************/
}
int Azimutal::getCurrentStep(void)
{
	return this->Stepsum;
}
void Azimutal::putStep(int target)
{
	this->Stepsum += target;
}
void Azimutal::moveToStep(int target)
{
	if (!filter(target)) return;
	
	//caso queiramos ir ao zero
	int delta = (target - getCurrentStep());
	Serial.print(getCurrentStep());
	
	if (abs(target) < abs(this->nullHole))
	{
		/***************************************************************
		Tres casos:
		(1) Se o sensor do zero estiver ativo, usa-lo para chegar no zero
		(2) Se eu estou no zero e quero me manter no zero sem o sensor
		(3) Se eu nao estou no zero e quero ir ao zero sem o sensor
		*****************************************************************/
		if (this->nullVerification) lookForZero();
		else
		{
			if (abs(getCurrentStep()) < abs(this->nullHole))
				return; //aqui ja estamos no zero, e queremos continuar
			else {
				Stepper::step(-1 * (getCurrentStep()) );
			}

		}
	}
	else	Stepper::step(delta);
	
	putStep(delta);
	return;
}

int Azimutal::idPriority(void)
{
	if (readPWM(pinRX3) < 20.0)		     return 4; 	//alavanca panico
	else if (readPWM(pinRX2) > 50.0)	 return 3;	//alavanca adição 180
	else if (readPWM(pinRX1) > 50.0)	 return 2;  //alavanca adição 90
	else								 return 1;	//alavanca principal
}

void Azimutal::lookForZero(void)
{
	if (!nullVerification) return;

	while (true)
	{
		if (digitalRead(pinNS) == LOW)	break; //esta no zero, ta check
		else //para encontrar o melhor caminho pro zero
		{
			if(this->step_number <  this->getNbrSteps() * this->driverConf) //se tiver antes da metade
				Stepper::step(1 * rotation);
			else
				Stepper::step(-1 * rotation);
		}
	}
	return;
	
}

//Métodos públicos
bool Azimutal::routine(void)
{//retorna falso se ocorrer problema.
	int priority = idPriority();
	int add = 0;
	int movement = 0;

	bool operation = true; //verifica se algo ocorreu mesmo
	switch (priority)
	{
	case 4:					//procurar pelo zero
		if(this->nullVerification)	lookForZero();
		//soh funciona se estiver ativa
		break;

	case 3:					//adicione 180
		add = ((this->getNbrSteps() * this->driverConf) / 2) + this->calibration1;
		moveToStep(readStep() + add);
		break;

	case 2:					//adicione 90
		if (readPWM(pinRX0) < 30)
			moveToStep((this->getNbrSteps() / 4) + this->calibration2);
		else moveToStep( ( (3 * this->getNbrSteps()) / 4) + this->calibration3);
		break;

	case 1:					//normal
		movement = readStep();
		moveToStep(movement);
		
		break;

	default:
		operation = false;
		break;
	}

	return operation;
}
