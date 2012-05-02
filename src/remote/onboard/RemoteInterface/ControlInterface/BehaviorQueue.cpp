#include "BehaviorQueue.h"

BehaviorQueue::BehaviorQueue() {
	isRunning = false;
	pthread_mutex_init(&queueMutex, NULL);	
}

void* startBusyLoop(void* arg) {
	BehaviorQueue* _this = (BehaviorQueue*) arg;
	_this->busyLoopProcedure();	
}

int BehaviorQueue::run() {
	isRunning = true;

	pthread_create(&busyLoopThread, NULL, startBusyLoop, (void*)this);
}


int BehaviorQueue::kill() {
	isRunning = false;
}

int BehaviorQueue::busyLoopProcedure() {
	printf("BehaviorQueue:: Busy loop thread started\n");

	while(this->isRunning) {
		usleep(100000);

		// if there's not a maneuver currently running and we have
		// a next maneuver enqueued, pop the next one off the queue and
		// execute

		pthread_mutex_lock(&queueMutex);
		//printf("Locked 1\n");
		
		//printf("Queue size __ %d\n", maneuverQueue.size());
		
		if (controlInterface.GetMSToManeuverCompletion() == 0 && !maneuverQueue.empty()) {
			Maneuver* nextManeuver = maneuverQueue.front();
			
			printf("BehaviorQueue:: Dequeueing and executing next maneuver\n");

			switch(nextManeuver->maneuverType) {

			case BEHAVIOR_MOVE:
				controlInterface.Move(nextManeuver->direction, nextManeuver->distance, nextManeuver->speed);


				break;
			case BEHAVIOR_ARC:
				controlInterface.ArcTurn(nextManeuver->direction, nextManeuver->degrees, nextManeuver->radius);

				break;
			case BEHAVIOR_PIVOT:
				controlInterface.PivotTurn(nextManeuver->direction, nextManeuver->degrees);

				break;
			}
			maneuverQueue.pop();
		}
		pthread_mutex_unlock(&queueMutex);
		//printf("Unocked 1\n");
	}

	printf("BehaviorQueue:: Busy loop thread exited\n");
}

int mystrncmpi(char* str1, char* str2, int n) {

	if (str1 == NULL || str2 == NULL) {
		return 1;
	}

	if (strlen(str1) < n || strlen(str2) < n) {
		return 1;
	}
	int maxLen = n;

	char buff1[maxLen];
	char buff2[maxLen];

	memcpy(buff1, str1, maxLen);
	memcpy(buff2, str2, maxLen);

	for (int i = 0; i < maxLen; i++) {
		if (tolower(buff1[i]) != tolower(buff2[i])) {

			return 1;
		}
	}	

	return 0;
}

int BehaviorQueue::enqueue(Maneuver* _maneuver) {
	pthread_mutex_lock(&queueMutex);
	//printf("Locked 2\n");
	int returnVal = _enqueue(_maneuver);
	pthread_mutex_unlock(&queueMutex);
	printf("Unocked 2\n");
	return returnVal;
}

int BehaviorQueue::_enqueue(Maneuver* _maneuver) {
	Maneuver* maneuver = (Maneuver*)malloc(sizeof(Maneuver));

	printf("here1\n");

	memcpy(maneuver, _maneuver, sizeof(Maneuver));

	printf("here2\n");
	
	printf("BehaviorQueue::Enqueueing maneuver:");
	
	printf("here3\n");
	
	printManeuver(maneuver);
	
	printf("here4\n");
	
	printf("Queue size = %d\n", maneuverQueue.size());
		
	maneuverQueue.push(maneuver);
	
	printf("here5\n");

	return 0;
}

int BehaviorQueue::setNextManeuver(Maneuver* _maneuver) {
	Maneuver* maneuver = (Maneuver*)malloc(sizeof(Maneuver));
	
	memcpy(maneuver, _maneuver, sizeof(Maneuver));
	
	printf("BehaviorQueue::Setting next maneuver:");
	printManeuver(maneuver);
	
	pthread_mutex_lock(&queueMutex);
	//printf("Locked 3\n");
		
	if (maneuverQueue.empty()) {
		maneuverQueue.push(maneuver);
	} else {
		Maneuver* curFront = maneuverQueue.front();
		memcpy(curFront, maneuver, sizeof(Maneuver));
	}
	
	pthread_mutex_unlock(&queueMutex);
	printf("Unocked 3\n");
	
	return 0;
}


int BehaviorQueue::parseInput(istream* input, vector<Maneuver*>& maneuverVec) {
	char linebuff[1024];

	vector<Maneuver*> maneuverList;

	int j = 0;

	int lineNum = 1;

	input->getline(linebuff, 1024);

	bool didReadValidManeuver = false;

	while(input->good()) {

		Maneuver* newManeuver = (Maneuver*) malloc (sizeof(Maneuver));
		bzero(newManeuver, sizeof(Maneuver));
		didReadValidManeuver = false;


		char* maneuverTypeStr = strtok(linebuff, " ");

		if (mystrncmpi(maneuverTypeStr, "#", 1) == 0) { 
			didReadValidManeuver = false;

		} else if (mystrncmpi(maneuverTypeStr, "MOVE", 4) == 0) {
			
			newManeuver->maneuverType = BEHAVIOR_MOVE;

			char* direction = strtok(NULL, " ");

			if (direction == NULL) {
				newManeuver->direction = -1;
			} else if(mystrncmpi(direction, "FORWARDS", 7) == 0) {
				newManeuver->direction = FORWARDS;

			} else if (mystrncmpi(direction, "BACKWARDS", 8) == 0) {
				newManeuver->direction = BACKWARDS;
			} else {
				return lineNum;
			}				

			char* distanceStr = strtok(NULL, " ");
			if (distanceStr == NULL) {
				newManeuver->distance = -1.0;
			} else {
				float distance = (float)atoi(distanceStr);
				if (distance < 0.0) {
					return lineNum;
				}

				newManeuver->distance = distance;		
			} 

			char* speedStr = strtok(NULL, " ");
			if (speedStr == NULL) {
				newManeuver->speed = -1.0;
			} else {
				float speed = (float)atoi(speedStr);
				if (speed < -0.1) {
					return lineNum;
				}

				newManeuver->speed = speed;		
			} 

			didReadValidManeuver = true;

		} else if (mystrncmpi(maneuverTypeStr, "ARC", 3) == 0) {

			char* direction = strtok(NULL, " ");

			if (direction == NULL) {
				newManeuver->direction = -1;
			} else if(mystrncmpi(direction, "FORWARDS-LEFT", 13) == 0) {
				newManeuver->direction = FORWARDS_LEFT;
			} else if (mystrncmpi(direction, "BACKWARDS-LEFT", 14) == 0) {
				newManeuver->direction = BACKWARDS_LEFT;
			} else if(mystrncmpi(direction, "FORWARDS-RIGHT", 14) == 0) {
				newManeuver->direction = FORWARDS_RIGHT;
			} else if (mystrncmpi(direction, "BACKWARDS-RIGHT", 15) == 0) {
				newManeuver->direction = BACKWARDS_RIGHT;
			} else {
				return lineNum;
			}				

			char* degreesStr = strtok(NULL, " ");
			if (degreesStr == NULL) {
				newManeuver->degrees = -1.0;
			} else {
				float degrees = (float)atoi(degreesStr);
				if (degrees < -0.1 || degrees > 90.1) {
					return lineNum;
				}

				newManeuver->degrees = degrees;		
			} 

			char* speedStr = strtok(NULL, " ");
			if (speedStr == NULL) {
				newManeuver->speed = -1.0;
			} else {
				float speed = (float)atoi(speedStr);
				if (speed < -0.1) {
					return lineNum;
				}

				newManeuver->speed = speed;		
			} 

			didReadValidManeuver = true;

			newManeuver->maneuverType = BEHAVIOR_ARC;

			didReadValidManeuver = true;

		} else if (mystrncmpi(maneuverTypeStr, "PIVOT", 5) == 0) {

			char* direction = strtok(NULL, " ");

			if (direction == NULL) {
				newManeuver->direction = -1;
			} else if(mystrncmpi(direction, "RIGHT", 5) == 0) {
				newManeuver->direction = RIGHT;
			} else if (mystrncmpi(direction, "LEFT", 4) == 0) {
				newManeuver->direction = LEFT;
			} else {
				return lineNum;
			}				

			char* degreesStr = strtok(NULL, " ");
			if (degreesStr == NULL) {
				newManeuver->degrees = -1.0;
			} else {
				float degrees = (float)atoi(degreesStr);
				if (degrees < -0.1 || degrees > 90.1) {
					return lineNum;
				}

				newManeuver->degrees = degrees;		
			}

			newManeuver->maneuverType = BEHAVIOR_PIVOT;

			didReadValidManeuver = true;
		} 


		if (didReadValidManeuver) {

			maneuverVec.push_back(newManeuver);
			
			printf("Inserting new maneuver \n");
		}

		bzero(linebuff, 1024);
		input->getline(linebuff, 1024);
		lineNum++;
	}

	return 0;
}

int BehaviorQueue::loadFromScriptText(char* scriptText, int len) {
	string scriptString(scriptText, len);
	istringstream stringStream(scriptString);

	vector<Maneuver*> maneuverVec;

	int parseRetVal = parseInput(&stringStream, maneuverVec);
	
	printf("BehaviorQueue:: Loading script from string\n");

	if (parseRetVal > 0) {
		printf("BehaviorQueue:: Error at line %d of script, could not parse input", parseRetVal);

		return parseRetVal;
	} 

	pthread_mutex_lock(&queueMutex);
	//printf("Locked 4\n");
	for (int i = 0; i < maneuverVec.size(); i++) {
		//this->_enqueue(maneuverVec[i]);
		printf("Here***\n");
		printManeuver(maneuverVec[i]);
	}
	pthread_mutex_unlock(&queueMutex);
	printf("Unocked 4\n");

	return 0;
}

int BehaviorQueue::loadFromScriptFile(char* filename) {
	ifstream infile;

	infile.open(filename, ifstream::in);

	if (!infile.is_open()) {
		printf("BehaviorQueue:: Error, could not open file %s\n", filename);
		return -1;
	}

	printf("BehaviorQueue:: Loading script file %s\n", filename);

	vector<Maneuver*> maneuverVec;

	int parseRetVal = parseInput(&infile, maneuverVec);

	if (parseRetVal > 0) {
		printf("BehaviorQueue:: Error at line %d of script, could not parse input", parseRetVal);

		return parseRetVal;
	} 

	pthread_mutex_lock(&queueMutex);
	//printf("Locked 5\n");
	for (int i = 0; i < maneuverVec.size(); i++) {
		this->_enqueue(maneuverVec[i]);
	}
	pthread_mutex_unlock(&queueMutex);
	printf("Unocked 5\n");


	return 0;
}

int printManeuver(Maneuver* m) {
	printf("   Maneuver: t:");

	switch(m->maneuverType) {
	case BEHAVIOR_NONE:
		printf("None ");
		break;
	case BEHAVIOR_MOVE:
		printf("Move ");
		break;
	case BEHAVIOR_ARC:
		printf("Arc Turn ");
		break;
	case BEHAVIOR_PIVOT:
		printf("Pivot Turn ");
		break;
	default:
		printf("Unknown Behavior Type ");
	}

	switch(m->direction) {
	case FORWARDS:
		printf("Forwards ");
		break;
	case BACKWARDS:
		printf("Backwards ");
		break;
	case LEFT:
		printf("Left ");
		break;
	case RIGHT:
		printf("Right ");
		break;
	case FORWARDS_RIGHT:
		printf("Forwards Right ");
		break;
	case BACKWARDS_RIGHT:
		printf("Backwards Right ");
		break;
	case FORWARDS_LEFT:
		printf("Forwards Left ");
		break;
	case BACKWARDS_LEFT:
		printf("Forwards Left ");
		break;
	default:
		printf("Uknown Direction ");
	}

	printf("deg:%f spd:%f dst:%f rad:%f\n", m->degrees, m->speed, m->distance, m->radius);

	return 0;
}

Maneuver* BehaviorQueue::getCurrentManeuver() {
	return controlInterface.GetCurrentManeuver();
}

Maneuver* BehaviorQueue::getNextManeuver() {
	pthread_mutex_lock(&queueMutex);
	//printf("Locked 6\n");
	Maneuver* maneuver = (Maneuver*) malloc(sizeof(Maneuver));	
	if (maneuverQueue.empty()) {
		maneuver->maneuverType = BEHAVIOR_NONE;
		pthread_mutex_unlock(&queueMutex);
		return maneuver;
	}

	memcpy(maneuver, maneuverQueue.front(), sizeof(Maneuver));
	pthread_mutex_unlock(&queueMutex);
	printf("Unocked 6\n");

	return maneuver;
}

int BehaviorQueue::getMSToManeuverCompletion() {
	return controlInterface.GetMSToManeuverCompletion();
}

