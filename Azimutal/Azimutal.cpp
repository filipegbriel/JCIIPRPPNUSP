#include "Azimutal.h"
/*
 * Azimutal.h - Biblioteca de um azimutal  - Vers�o 1.1
 *
 * Funcionamento original	(0.0)   por Filipe Gabriel Santos e Henrique Martinez Rocamora.
 * Biblioteca para arduino	(0.1)	por Filipe Santos, David Engelstein, Gustavo Takashi, Paulo Yamabuchi
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

Azimutal::~Azimutal()
{
}
