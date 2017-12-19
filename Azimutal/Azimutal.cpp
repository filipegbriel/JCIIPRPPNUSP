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


Azimutal::Azimutal(int pinRX[0], int pinRX[1], int pinRX[2], int pinRX[3], int pinSM0, int pinSM1, int nbrSteps) : Stepper(nbrSteps, pinSM0, pinSM1)
{
	for (int i = 0, i < 4, i++)
	{
		this->pinRX[i] = pinRX[i];
		pinMode(this->pinRX[i], INPUT);
	}
}
Azimutal::Azimutal(int pinRX[0], int pinRX[1], int pinRX[2], int pinRX[3], int pinSM0, int pinSM1, int pinSM2, int pinSM3, int nbrSteps) : Stepper(nbrSteps, pinSM0, pinSM1, pinSM2, pinSM3)
{
	for (int i = 0, i < 4, i++)
	{
		this->pinRX[i] = pinRX[i];
		pinMode(this->pinRX[i], INPUT);
	}
}
Azimutal::Azimutal(int pinRX[0], int pinRX[1], int pinRX[2], int pinRX[3], int pinSM0, int pinSM1, int pinSM2, int pinSM3, int pinSM4, int nbrSteps) : Stepper(nbrSteps, pinSM0, pinSM1, pinSM2, pinSM3, pinSM4)
{
	for (int i = 0, i < 4, i++)
	{
		this->pinRX[i] = pinRX[i];
		pinMode(this->pinRX[i], INPUT);
	}
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
bool  Azimutal::filter(float x)
{
	bool answer = true;
	//onde filtra com un unidades
	for (int i = 0; i < filterSize; i++)
	{
		if (x <= this->filtre[i] + this->un && x >= this->filtre[i] - this->un) answer = false;
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
float Azimutal::readStep(void)
{
	//essa função le o pwm e o transforma em um passo entre o passo minimo e o passo maximo.
	float x = readPWM(pinRX[0]);
	x = map_f(x, 0.0, 100.0, -100.0, 100.0);
	return this->getNbrSteps() * this->driverConf * this->restrition * x / 100;
}

float readStep(void)
{
	float x = this->readPWM(this->pinRX[0]);
	//cria o buraco do zero
	if (-(this->nullHole) < x || x < this->nullHole) x = 0;

	if (this->filter(x)) return x + this->shouldIReverse(); 
	else                 return getCurrentStep(void) + shouldIReverse
}
