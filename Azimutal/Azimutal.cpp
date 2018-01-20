#include "Azimutal.h"
/*
 * Azimutal.h - Biblioteca de um azimutal  - Vers�o 1.1
 *
 * Funcionamento original	(0.0)   por Filipe Gabriel Santos e Henrique Martinez Rocamora.
 * Biblioteca para arduino	(0.1)	por Filipe Santos, David Engelstein, Gustavo Takashi, Paulo Yamabuchi, Anna Queiroz
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
 * pinRX[0] == alavanca de posição
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
	pinMode(this->pinNS, INPUT);
}
Azimutal::Azimutal(int pinRX0, int pinRX1, int pinRX2, int pinRX3, int pinSM0, int pinSM1, int pinSM2, int pinSM3, int nbrSteps, int pinNS) : Stepper(nbrSteps, pinSM0, pinSM1, pinSM2, pinSM3)
{
	this->pinRX0 = pinRX0;
	pinMode(this->pinRX0, INPUT);
	this->pinRX1 = pinRX1;
	pinMode(this->pinRX1, INPUT);
	this->pinRX2 = pinRX2;
	pinMode(this->pinRX, INPUT);
	this->pinRX3 = pinRX3;
	pinMode(this->pinRX3, INPUT);

	this->pinNS = pinNS;
	pinMode(this->pinNS, INPUT);
}
Azimutal::Azimutal(int pinRX0, int pinRX1, int pinRX2, int pinRX3, int pinSM0, int pinSM1, int pinSM2, int pinSM3, int pinSM4, int nbrSteps, int pinNS) : Stepper(nbrSteps, pinSM0, pinSM1, pinSM2, pinSM3, pinSM4)
{
	this->pinRX0 = pinRX0;
	pinMode(this->pinRX0, INPUT);
	this->pinRX1 = pinRX1;
	pinMode(this->pinRX1, INPUT);
	this->pinRX2 = pinRX2;
	pinMode(this->pinRX, INPUT);
	this->pinRX3 = pinRX3;
	pinMode(this->pinRX3, INPUT);

	this->pinNS = pinNS;
	pinMode(this->pinNS, INPUT);
}

//Configurando as constantes de calibração:
/* não lembro o que faz, tirar se não servir pra nada
bool Azimutal::setConstStep(float ctrlConf)
{
	this->ctrlConf = ctrlConf;
	return true;
}
*/
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

//Métodos privados
float Azimutal::readPWM(int pin)
{
	int x = (int)pulseIn(pin, HIGH);
	if (x > this->maxPWM) x = maxPWM;
	else if (x < this->minPWM) x = minPWM;

	return map_f((float)x, minPWM, maxPWM, 100.0, 0);
}
float Azimutal::map_f(float number, float minI, float maxI, float minF, float maxF)
{
	float p = ((number - minI) * (maxF - minF) / (maxI - minI)) + minF;
	if (p > maxF) p = maxF;
	if (p < minF) p = minF;
	return p;
}


bool Azimutal::filter(float x)
{	//RETORNA FALSO SE X' FOR TREMULAÇÃO DE X

	bool answer = true;
	//onde filtra com un unidades
	for (int i = 0; i < filterSize; i++)
	{
		if (x <= this->filtre[i] + this->un && x >= this->filtre[i] - this->un) answer = false; // un explicado no .h
	}

	//atualiza o filtro
	for (int i = 0; i < filterSize; i++)
	{
		if (i < 2)
		{
			this->filtre[i] = this->filtre[i + 1];
		}
		else this->filtre[i] = x;
	}
	return answer;
}
int	 Azimutal::readStep(void)
{
	//essa função le o pwm e o transforma em um passo entre o passo minimo e o passo maximo.
	float x = readPWM(pinRX0);
	x = map_f(x, 0.0, 100.0, -100.0, 100.0);
	//transformando de porcentagem pra passo
	return this->getNbrSteps() * this->driverConf * this->restrition * x / 100;
}

void Azimutal::moveToStep(int target)
{
	Stepper::step(target);
	return void;
}

//IDENTIFICAÇÃO DE PRIORIDADE
int Azimutal::idPriority(void)
{
	if (readPWM(pinRX3) < 20)			return 4; //alavanca panico
	else if (readPWM(pinRX2) > 50)	return 3; //alavanca adição 180
	else if (readPWM(pinRX1) > 50)	return 2; //alavanca adição 90
	else								return 1; //alavanca principal
}

void Azimutal::lookForZero(void)
{
	access = true;
	while (access)
	{
		if (digitalRead(pinNS) == HIGH)	break;
		else	Stepper::step(1);
	}
	return void;
}

bool Azimutal::routine(void)
{//retorna falso se ocorrer problema.
	int priority = idPriority();
	int add = 0;

	bool operation = true;
	switch (priority)
	{
	case 4:					//procurar pelo zero
		lookForZero();			//FALTA FAZER
		break;

	case 3:					//adicione 180
		add = (this->getNbrSteps() * this->driverConf) / 2;
		priority = 1;

	case 2:					//adicione 90
		if (readPWM(pinRX0) < 30)
			moveToStep(this->getNbrSteps() / 4);
		else moveToStep((3 * this->getNbrSteps()) / 4);
		break;

	case 1:					//normal
		moveToStep(readStep() + add);
		break;

	default:
		operation = false;
		break;
	}

	return operation;
}

float readStep(void)
{
	float x = this->readPWM(this->pinRX0);
	//cria o buraco do zero
	if (-(this->nullHole) < x || x < this->nullHole) x = 0;

	if (this->filter(x)) return x + this->shouldIReverse(); 
	else                 return getCurrentStep(void) + shouldIReverse
}
