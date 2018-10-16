﻿/*
 * Azimutal.h - Biblioteca de um azimutal  - Vers�o 1.2
 *
 * Funcionamento original	(0.0)   por Filipe Gabriel Santos Valentim e Henrique Martinez Rocamora.
 * Biblioteca para arduino	(1.0)	por Filipe Valentim, Anna Queiroz
 * Funcionamento incial completo (1.1) 	por Filipe Valentim, Anna Queiroz
 * Atualiza��o para calibra��es e busca inteligente do zero (1.2) por Filipe Valentim
 *
*
* Esta biblioteca � para o uso privado da institui��o hoje (2017) nomeada por PoliN�utico - grupo de
* extens�o acad�mica da Escola Politecninca da Universidade de S�o Paulo. Seu uso n�o est� autorizado
* para qualquer outra pessoa que n�o seja, ou n�o tenha sido, associada ao grupo.
*
* Universidade de S�o Paulo, S�o Paulo, S�o Paulo, Brasil. Novembro de 2017
*
* Esta biblioteca controla um motor de passo ligado a uma propuls�o azimutal do barco Jo�o C�ndido II.
* Sua comunica��o com o receptor usa 3 canais e um sensor de fim de curso, o qual verifica se o barco est�
* em zero com a normal
*
* pinRX0 == alavanca de posição
* pinRX1 == alavanca de add90
* pinRX2 == alavanca de add180
* pinRX3 == alavanca de PANICO (busca zero)
* pinNS1 == sensor de fim de curso 1 ligado com PULLUP
* pinNS2 == sensor de fim de curso 2 ligado com PULLUP
* pinSM0 == pino motor de passo
* pinSM1 == pino motor de passo 
* pinSM2 == pino motor de passo
* pinSM3 == pino motor de passo
* nbrSteps == numero de passos do motor
*
* ****************************************************************************
* IMPORTANTE PARA UM K7: PARA A BIBLIOTECA FUNCIONAR, TEM QUE ESTAR JUNTO COM
* A BIBLIOTECA STEPPER
* ****************************************************************************
*
* Futuras implementações: botão ou alavanca para determinar nullVerification
*/

// Garante que o arquivo s� seja inclu�do uma vez.
#ifndef		Azimutal_h
#define		Azimutal_h
#include	"Stepper.h"
#include	"Arduino.h"


#define		un					100		//precisão do filtro, NAO PODE SER PORCENTAGEM DE X (é a discreitzação dos passos)
#define     COMUNICACAO			false

class Azimutal : public Stepper
{
public:
	//construtures para todos os casos de motor de passo
	Azimutal(int pinRX0, int pinRX1, int pinRX2, int pinRX3,
		int pinSM0, int pinSM1, int nbrSteps, int pinNS1, int pinNS2);
	Azimutal(int pinRX0, int pinRX1, int pinRX2, int pinRX3,
				int pinSM0, int pinSM1, int pinSM2, int pinSM3, int nbrSteps, int pinNS1, int pinNS2);
	Azimutal(int pinRX0, int pinRX1, int pinRX2, int pinRX3,
				int pinSM0, int pinSM1, int pinSM2, int pinSM3, int pinSM4, int nbrSteps, int pinNS1, int pinNS2);
		
	
	//Constantes de calibração
	//bool setConstStep(float ctrlConf); //escala do Controle; deixar em 100
	bool	setConstPWM(float minPWM, float maxPWM);
	bool	setConstNH(float nullHole);
	bool	setRestrition(float res);
	bool	setDriverConf(int num); //configuração do driver
	bool	setCalibrationsVars(int eins, int zwei, int drei);
	bool    setNullVerif(bool set);
	bool    setNullSteps(int number);
	bool 	setNullPosSteps(int number);
	bool    setNullTimeSearch(int time); 	//delay entre passos do zero em busca

	//FUNCAO ROTINA: COMO TDO FUNCIONA
	bool	routine(void);
	int		getCurrentStep(void);

private:

	int		pinRX0;
	int		pinRX1;
	int		pinRX2;
	int		pinRX3;//pinos do Receptor
	int		pinNS1; //fim de curso 1
	int		pinNS2; //fim de curso 2

	bool 	nullVerification;  	//determina se deve usar o sensor do zero ou nao
		
		//constantes de calibração
	float	minPWM, maxPWM;				//min e max leitura analogica do pwm.
	float	restrition;					//no modo normal é a restrição do quanto pode ir ao lado
	int		driverConf;					//conf de multiplicacao de passos do driver				
	int		calibration1;				//conf de add de 180
	int		calibration2;				//conf de add de 90 pra esquerda
	int		calibration3;				//conf de add de 90 pra direita (pode ser que esteja trocado)
	int 	nullStep;					//passos que da pra acertar o zero na busca inteligente
	int		PASSOS_EXTRA_ZERO;			//acertar posi��o ap�s busca do zero.
	int     TEMPO_DE_DELAY;				//delay entre passos do zero em busca


	//metodos privados
	float	map_f(float number, float minI, float maxI, float minF, float maxF);
	float	readPWM(int pin);
	int		readStep(void);
	void	moveToStep(int target);
	int		idPriority(void);
	void    putStep(int target);

	int		inteligentSearch(int ZeroType);
	void	lookForZero(int ZeroType);
	bool	AmIatZero(int zeroType);
		
	float	nullHole;

	bool	filter(int x);
	int		lastStep;    //usada no filtro
	int		Stepsum;	 //conta o passo atual
	int     effectiveZero;
	
	
};
#endif
