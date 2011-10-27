#include "RaptorBot.h"

void spinAWhile();

int main() {
	ControlInterface* control = new ControlInterface();
	printf("Created control interface\n");
	

		/*control->SetPin(11, 255);
		control->SetPin(10, 0);


		control->SetPin(10, 255);
		control->SetPin(11, 0);*/

	//for(int ii=0; ii<1000; ii++){
	control->PivotTurn(LEFT, 10);
	control->PivotTurn(RIGHT, 10);

	control->PivotTurn(LEFT, 30);
	control->PivotTurn(RIGHT, 30);

	control->PivotTurn(LEFT, 50);
	control->PivotTurn(RIGHT, 50);

	control->PivotTurn(LEFT, 90);
	control->PivotTurn(RIGHT, 90);

	//}
}

void spinAWhile() {
	double x = 1.29094375;	
	for (int i = 0 ; i < 100000; i++) {
		x = x * x;
		x = x / x;
	}
}
