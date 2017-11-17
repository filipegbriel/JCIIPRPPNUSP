﻿#include "Azimutal.h"
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
bool Azimutal::setConstStep(float ctrlConf)
{
	this->ctrlConf = ctrlConf;
	return bool;
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

//Métodos privados
float Azimutal::readPWM(int pin)
{
	int x = (int)pulseIn(pin, HIGH);
	if (x > this->maxPWM) x = maxPWM;
	else if (x < this->minPWM) x = minPWM;

	return map_f((float)x, minPWM, maxPWM, -(this->getNbrSteps*this->ctrlConf)/2, this->getNbrSteps*this->ctrlConf)/2;
}
float Azimutal::map_f(float number, float minI, float maxI, float minF, float maxF)
{
	float p = ((number - minI) * (maxF - minF) / (maxI - minI)) + minF;
	if (p > maxF) p = maxF;
	if (p < minF) p = minF;
	return p;
}

float readStep(void)
{
	float x = this->readPWM(this->pinRX[0]);
	bool enable = true;

	//cria o buraco do zero
	if (-(this->nullHole) < x || x < this->nullHole) x = 0;

	//Filtra leitura de acordo com filterSize
	while (enable)
	{
		for (int i = 0; i < filterSize; i++)
		{
			if (x <= filter[i] - filter[i] * secInt || x >= filter[i] + filter[i] * secInt) enable = false;

			x = readPWM(pinoPWM_1, maxPulse, minPulse);
		}

	}
	return x + shouldIReverse();
}
