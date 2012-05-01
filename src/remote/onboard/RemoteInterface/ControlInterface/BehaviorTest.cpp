#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "BehaviorQueue.h"
#include "ControlInterface.h"


/*
typedef struct Maneuver_t {
	char maneuverType;
	int direction;
	float degrees;
	float speed;
	float distance;
	float radius;
} Maneuver;
*/

int main (int argc, char **argv){

	BehaviorQueue behaviorQueue;

	behaviorQueue.run();

	behaviorQueue.loadFromScriptFile("behavior_script.txt");


	while(true);

}
