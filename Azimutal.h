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

// Garante que o arquivo só seja incluído uma vez.
#ifndef Azimutal_h
#define Azimutal_h

// Descrição interativa da classe
class Azimutal{
  public:
    // construtores (depende do motor de passo):
    Azimutal(int confPassos, int pinPasso1, int pinPasso2,
			 int pinRX1,     int pinRX2,    int pinRX3,
		     int pinRX4);
	Azimutal(int confPassos, int pinPasso1, int pinPasso2,
			 int pinPasso3,	 int pinPasso4,
			 int pinRX1,     int pinRX2,    int pinRX3,
			 int pinRX4);
	Azimutal(int confPassos, int pinPasso1, int pinPasso2,
			 int pinPasso3,  int pinPasso4, int pinPasso5,
			 int pinPasso6,
			 int pinRX1,	 int pinRX2,	int pinRX3,
			 int pinRX4);
    

    // Para fazer o Stepper funcionar
    void setSpeed(long whatSpeed);

    // mover method:
    void step(int number_of_steps);

    int version(void);

  private:
    void stepMotor(int this_step);

    int direction;            // Direction of rotation
    unsigned long step_delay; // delay between steps, in ms, based on speed
    int number_of_steps;      // total number of steps this motor can take
    int pin_count;            // how many pins are in use.
    int step_number;          // which step the motor is on

    // motor pin numbers:
    int motor_pin_1;
    int motor_pin_2;
    int motor_pin_3;
    int motor_pin_4;
    int motor_pin_5;          // Only 5 phase motor

    unsigned long last_step_time; // time stamp in us of when the last step was taken
};

#endif

