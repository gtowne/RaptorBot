#ifndef BEHAVIOR_QUEUE_H
#define BEHAVIOR_QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <fstream>
#include <pthread.h>
#include <string.h>
#include <sstream>
#include <ctype.h>
#include <unistd.h>
#include <algorithm>
#include "ControlInterface.h"

using namespace std;

class BehaviorQueue {
private:

	queue<Maneuver*> maneuverQueue;
	ControlInterface controlInterface;

	pthread_t busyLoopThread;

	pthread_mutex_t queueMutex;

	bool isRunning;

	int parseInput(istream* input, vector<Maneuver*>& maneuverVec);
	
	int _enqueue(Maneuver* _maneuver);

public:
	BehaviorQueue();
	
	int run();
	int kill();

	int setNextManeuver(Maneuver* maneuver);
	int enqueue(Maneuver* maneuver);

	int busyLoopProcedure();

	int loadFromScriptText(char* scriptText, int len);

	int loadFromScriptFile(char* filename);
	

	Maneuver* getCurrentManeuver();
	Maneuver* getNextManeuver();
	int getMSToManeuverCompletion();

};


#endif
