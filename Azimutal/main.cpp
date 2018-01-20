#include <iostream>

#include "Azimutal.h"
#include "Stepper.h"

using namespace std;

int main(void)
{
	Azimutal* azm = new Azimutal(1, 2, 3, 4, 5, 6, 200, 7);
	
	azm->filtre[0] = 200;

	azm->filtre[1] = 200;

	azm->filtre[2] = 200;

	if (azm->filter(205))
		cout << "X EH ACEITAVEL" << endl;
	else cout << "X NAO E ACEITAVEL" << endl;

	return 0;
}