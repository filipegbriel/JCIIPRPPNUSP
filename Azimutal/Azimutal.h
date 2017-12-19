﻿/*
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
// Garante que o arquivo s� seja inclu�do uma vez.
#ifndef Azimutal_h
#define Azimutal_h
#include "Stepper.h"
#define filterSize 3
class Azimutal : public Stepper
{
public:
	//construtures para todos os casos de motor de passo
	Azimutal(int pinRX[0], int pinRX[1], int pinRX[2], int pinRX[3],
		int pinSM0, int pinSM1, int nbrSteps);
	Azimutal(int pinRX[0], int pinRX[1], int pinRX[2], int pinRX[3],
				int pinSM0, int pinSM1, int pinSM2, int pinSM3, int nbrSteps);
	Azimutal(int pinRX[0], int pinRX[1], int pinRX[2], int pinRX[3],
				int pinSM0, int pinSM1, int pinSM2, int pinSM3, int pinSM4, int nbrSteps);
	
	//Constantes de calibração
	//bool setConstStep(float ctrlConf); //escala do Controle; deixar em 100
	bool setConstPWM(float minPWM, float maxPWM);
	bool setConstNH(float nullHole);
	bool setRestrition(float res);
	bool setDriverConf(int num); //configuração do driver

private:
	int pinRX[4]; //pinos do Receptor
	/*
	pinRX[] = alavanca de posição
	*/

	//constantes de calibração
	float minPWM, maxPWM;				//min e max leitura analogica do pwm.
	float restrition;				//no modo normal é a restrição do quanto pode ir ao lado
	int	  driverConf
										// float ctrlConf = 100;

	//metodos privados
	float map_f(float number, float minI, float maxI, float minF, float maxF);
	float readPWM(int pin);
	bool filter(float x);
	float readStep(void);

	float nullHole;
	const float un = 10;
	float filtre[filterSize] = { 0, 0, 0};
	
};
#endif
