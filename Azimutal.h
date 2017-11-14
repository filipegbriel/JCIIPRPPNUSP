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

// Garante que o arquivo s� seja inclu�do uma vez.
#ifndef Azimutal_h
#define Azimutal_h

// Descri��o interativa da classe
class Azimutal{
  public:
    // construtores (depende do motor de passo):
    Azimutal(int confPassos, int pinPasso1, int pinPasso2,
			 int velGiro,
			 int pinRX1,     int pinRX2,    int pinRX3,
		     int pinRX4);
	Azimutal(int confPassos, int pinPasso1, int pinPasso2,
			 int pinPasso3,	 int pinPasso4, int velGiro,
			 int pinRX1,     int pinRX2,    int pinRX3,
			 int pinRX4);
	Azimutal(int confPassos, int pinPasso1, int pinPasso2,
			 int pinPasso3,  int pinPasso4, int pinPasso5,
			 int pinPasso6,  int velGiro,
			 int pinRX1,	 int pinRX2,	int pinRX3,
			 int pinRX4);
    

    // Para fazer o Stepper funcionar
    void setSpeed(long whatSpeed);

    // mover method:
    void step(int number_of_steps);

    int version(void);

  private:
    Stepper mtrPasso;
};

	  pepeta

#endif

