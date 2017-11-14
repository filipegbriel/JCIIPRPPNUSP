/*
* Azimutal.h - Biblioteca de um azimutal  - Versão 1.1
*
* Funcionamento original	(0.0)   por Filipe Gabriel Santos e Henrique Martinez Rocamora.
* Biblioteca para arduino	(0.1)	por Filipe Santos, David Engelstein, Gustavo Takashi, Paulo Yamabuchi
*
* Esta biblioteca é para o uso privado da instituição hoje (2017) nomeada por PoliNáutico - grupo de
* extensão acadêmica da Escola Politecninca da Universidade de São Paulo. Seu uso não está autorizado
* para qualquer outra pessoa que não seja, ou não tenha sido, associada ao grupo.
*
* Universidade de São Paulo, São Paulo, São Paulo, Brasil. Novembro de 2017
*
* Esta biblioteca controla um motor de passo ligado a uma propulsão azimutal do barco João Cãndido II.
* Sua comunicação com o receptor usa 3 canais e um sensor de fim de curso, o qual verifica se o barco está
* em zero com a normal.
*/


#include "Arduino.h"
#include "Stepper.h"

//Construtor de dois fios
Azimutal::Azimutal(int confPassos, int pinPasso1, int pinPasso2,
	int pinRX1, int pinRX2, int pinRX3,
	int pinRX4);
{
}
