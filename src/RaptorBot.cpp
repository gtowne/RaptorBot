#include "RaptorBot.h"

void spinAWhile();

int main() {
	ControlInterface* control = new ControlInterface();
	printf("Created control interface\n");
	

	for (int i = 0; i < 100; i++) {

		control->SetPin(11, 255);
		control->SetPin(10, 0);

		//spinAWhile();

		control->SetPin(10, 255);
		control->SetPin(11, 0);

		//spinAWhile();

	}
}

void spinAWhile() {
	double x = 1.29094375;	
	for (int i = 0 ; i < 100000; i++) {
		x = x * x;
		x = x / x;
	}
}
