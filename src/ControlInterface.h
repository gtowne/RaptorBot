#ifndef RAPTOR_BOT_CONTROL_INTERFACE_H
#define RAPTOR_BOT_CONTROL_INTERFACE_H

#include "RaptorSerialInterface.h"
#include "RaptorWireProtocol.h"

// Define constants for directions
#define FORWARDS 'f'
#define BACKWARD 'b'
#define LEFT 'l'
#define RIGHT 'r'
#define FORWARD_LEFT 'a'
#define FORWARD_RIGHT 'b'
#define BACKWARDS_LEFT 'c'
#define BACKWARDS RIGHT 'd'

// Define constants for parameters 
#define DEGREE 'e'
#define RADIUS 'f'
#define DISTANCE 'g'
#define SPEED 'h'
#define PIN_NUM 'i'

// Name for Arduino USB device in /dev. This is sytem dependent.
#define ARDUINO_SERIAL_DEVICE_NAME "/dev/ttyACM0"
#define ARDUINO_SERIAL_BAUDRATE 9600

class ControlInterface
{
  public:
    ControlInterface();
    ~ControlInterface();
    
    /*direction - either FORWARDS or BACKWARDS
	 *distance - the distance the robot should move in meters 
	 *speed - the speed the robot should move in meters/second
	 *This function moves the robot either forwards or backwards, over the distance and at a speed 
	 *specified as arguments.
	 */
    int Move(char direction, float distance, float speed);
    
    /*
     *direction - either FORWARDS or BACKWARDS 
	 *distance - the distance the robot should move in meters
	 *This function moves the robot either forwards or backwards, over the distance specified 
	 *as an argument at the speed previously specified with the Set() function.
     */
    int Move(char direction, float distance);
    
    /*
     *direction - either FORWARDS or BACKWARDS
	 * This function moves the robot either forwards or backwards, over the distance and at the speed
	 * previously specified with the Set() function.
     */
    int Move(char direction);
    
    /*
     * direction - either LEFT or RIGHT 
     * degrees - the number of degrees that the robot should turn (can be
	 *     greater than 360)
	 * This function makes the robot perform a pivot-turn in the direction and for the angle 
	 * specified as arguments.
     */
    int PivotTurn(char direction, float degrees);
    
    /*
     * direction - either LEFT or RIGHT (which are macros you must define)
	 * This function makes the robot perform a pivot-turn in the direction specified as an 
	 * argument, for the number of degrees previously specified with Set().
     */
    int PivotTurn(char direction);
    
    /*
     * direction - either FORWARD LEFT,	FORWARD RIGHT, BACKWARDS LEFT or BACKWARDS RIGHT
	 * degrees - the number of degrees that the arc turn should travel  
	 * radius - the radius that the arc turn should be taken at
     */
    int ArcTurn(char direction, float degrees, float radius);
    
    /*
     * direction - either	FORWARD_LEFT,	FORWARD_RIGHT, BACKWARDS_LEFT or BACKWARDS_RIGHT 
	 * degrees - the number of degrees that the arc turn should travel 
     */
    int ArcTurn(char direction, float degrees);
    
    /* 
     * direction - either FORWARD LEFT,	FORWARD RIGHT, BACKWARDS LEFT or BACKWARDS RIGHT
     */
    int ArcTurn(char direction);
    
    /*
     *type - either DEGREE, RADIUS, DISTANCE, SPEED or PIN_NUM 
	 *value - the value to set.
	 * This function is used to set either the degree, radius, distance or 
	 * speed properties for later movement. 
     */
    int Set(char type, float value);
    
    /*
     * type - either DEGREE, RADIUS, DISTANCE, SPEED or pin-number 
	 * return value - the value stored in the behaviors library that is used for movement 
	 *       functions, or an analog pin value
	 * This function returns the value stored in the behaviors object. For analog pins, the 
	 * return value is the value reported by analogRead.
     */
    float Get(char type);

    int SetPin(int pinNum, int pwmVal);

  private:
  	int curDegrees;
  	int curRadius;
  	char curDirection;
	RaptorSerialInterface* serialInterface;
};


#endif
