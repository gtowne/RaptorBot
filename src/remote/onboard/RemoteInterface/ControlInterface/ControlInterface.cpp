
#include "ControlInterface.h"

char* getCurTimeString() {
	time_t curtime;
	time(&curtime);
	struct tm* timeinfo;
	timeinfo = localtime ( &curtime );
	return asctime (timeinfo);

}

ControlInterface::ControlInterface() {
	time(&timeLastManeuverStart);

	char filenamebuff[256];
	sprintf(filenamebuff, "ControlLog-%s.txt", getCurTimeString());

	outfile.open(filenamebuff);
	outfile << getCurTimeString << " --- Initializing\n";
	outfile.flush();

	curManeuver.maneuverType = BEHAVIOR_NONE;
	curManeuver.degrees = DEFAULT_DEGREES;
	curManeuver.radius = DEFAULT_RADIUS;
	curManeuver.speed = DEFAULT_SPEED;
	curManeuver.direction = DEFAULT_DIRECTION;
}

ControlInterface::~ControlInterface() {
}


/*direction - either FORWARDS or BACKWARDS
 *distance - the distance the robot should move in meters 
 *speed - the speed the robot should move in meters/second
 *This function moves the robot either forwards or backwards, over the distance and at a speed 
 *specified as arguments.
 */
int ControlInterface::Move(char direction, float distance, float speed) {
	printf("ControlInterface:: Beginning Move\n");

	outfile << getCurTimeString() << " --- Begin Move ";
	time(&timeLastManeuverStart);

	this->curManeuver.maneuverType = BEHAVIOR_MOVE;
	this->curManeuver.direction = direction;

	switch (direction) {
	case FORWARDS:
		outfile << "Forward ";

		break;
	case BACKWARDS:
		outfile << "Backward ";

		break;
	}

	if (distance > 0) {
		outfile << distance << " Meters ";
		this->curManeuver.distance = distance;
	}

	if (speed > 0) {
		this->curManeuver.speed = speed;
	}

	outfile << "at " << this->curManeuver.speed << " m/s\n";

	outfile.flush();
	return 0;
}

/*
 *direction - either FORWARDS or BACKWARDS 
 *distance - the distance the robot should move in meters
 *This function moves the robot either forwards or backwards, over the distance specified 
 *as an argument at the speed previously specified with the Set() function.
 */
int ControlInterface::Move(char direction, float distance) {
	return this->Move(direction, distance, this->curManeuver.speed);
}

/*
 *direction - either FORWARDS or BACKWARDS
 * This function moves the robot either forwards or backwards, over the distance and at the speed
 * previously specified with the Set() function.
 */
int ControlInterface::Move(char direction) {
	return this->Move(direction, -1, this->curManeuver.speed);
	return 0;
}

/*
 * direction - either LEFT or RIGHT 
 * degrees - the number of degrees that the robot should turn (can be
 *     greater than 360)
 * This function makes the robot perform a pivot-turn in the direction and for the angle 
 * specified as arguments.
 */
int ControlInterface::PivotTurn(char direction, float degrees) {
	printf("ControlInterface:: Beginning Pivot\n");

	outfile << getCurTimeString() << " --- Begin Pivot Turn ";
	time(&timeLastManeuverStart);

	this->curManeuver.maneuverType = BEHAVIOR_PIVOT;
	this->curManeuver.direction = direction;

	switch (direction) {
	case LEFT:
		outfile << "Left ";

		break;
	case RIGHT:
		outfile << "Right ";

		break;
	}

	if (degrees > 0) {
		this->curManeuver.degrees = degrees;
	}

	outfile << this->curManeuver.degrees << " degrees\n";

	outfile.flush();

	return 0;
}

/*
 * direction - either LEFT or RIGHT (which are macros you must define)
 * This function makes the robot perform a pi	printf("Initialized serial connection to Arduino\n");vot-turn in the direction specified as an 
 * argument, for the number of degrees previously specified with Set().
 */
int ControlInterface::PivotTurn(char direction) {
	return this->PivotTurn(direction, this->curManeuver.direction);
}

/*
 * direction - either FORWARD LEFT,	FORWARD RIGHT, BACKWARDS LEFT or BACKWARDS RIGHT
 * degrees - the number of degrees that the arc turn should travel  
 * radius - the radius that the arc turn should be taken at
 */
int ControlInterface::ArcTurn(char direction, float degrees, float radius) {
	printf("ControlInterface:: Beginning Arc\n");

	outfile << getCurTimeString() << " --- Begin Arc Turn ";
	time(&timeLastManeuverStart);

	this->curManeuver.maneuverType = BEHAVIOR_ARC;
	this->curManeuver.direction = direction;

	switch (direction) {
	case FORWARDS_LEFT:
		outfile << "Forward Left ";

		break;
	case FORWARDS_RIGHT:
		outfile << "Forward Right ";

		break;
	case BACKWARDS_LEFT:
		outfile << "Backward Left ";

		break;
	case BACKWARDS_RIGHT:
		outfile << "Backward Right ";

		break;
	}

	if (degrees > -0.1 && degrees < 90.1) {
		this->curManeuver.degrees = degrees;
	}

	if (radius > -0.1) {
		this->curManeuver.radius = radius;
	}

	outfile << this->curManeuver.degrees << " degrees ";
	outfile << " in radius " << this->curManeuver.radius << "\n";
	

	outfile.flush();

	return 0;
}

/*
 * direction - either	FORWARD_LEFT,	FORWARD_RIGHT, BACKWARDS_LEFT or BACKWARDS_RIGHT 
 * degrees - the number of degrees that the arc turn should travel 
 */
int ControlInterface::ArcTurn(char direction, float degrees) {
	return 0;
}

/* 
 * direction - either FORWARD LEFT,	FORWARD RIGHT, BACKWARDS LEFT or BACKWARDS RIGHT
 */
int ControlInterface::ArcTurn(char direction) {
	return 0;
}

int ControlInterface::GetMSToManeuverCompletion() {
	time_t curTime;

	time(&curTime);	

	double secsSinceLast = difftime(curTime, timeLastManeuverStart);

	if (secsSinceLast > 10) {
		return 0;
	}

	double secsToComplete = 10 - secsSinceLast;

	int msToComplete = (int)(secsToComplete * 1000);

	return msToComplete;
}

Maneuver* ControlInterface::GetCurrentManeuver() {
	if (this->GetMSToManeuverCompletion() <= 0) {
		this->curManeuver.maneuverType = BEHAVIOR_NONE;
	}

	Maneuver* maneuver = (Maneuver*) malloc(sizeof(Maneuver));
	memcpy(maneuver, &curManeuver, sizeof(Maneuver));

	return maneuver;
}




