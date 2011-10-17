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
    void Move(char direction, float distance, float speed);
    
    /*
     *direction - either FORWARDS or BACKWARDS 
	 *distance - the distance the robot should move in meters
	 *This function moves the robot either forwards or backwards, over the distance specified 
	 *as an argument at the speed previously specified with the Set() function.
     */
    void Move(char direction, float distance);
    
    /*
     *direction - either FORWARDS or BACKWARDS
	 * This function moves the robot either forwards or backwards, over the distance and at the speed
	 * previously specified with the Set() function.
     */
    void Move(char direction);
    
    /*
     * direction - either LEFT or RIGHT 
     * degrees - the number of degrees that the robot should turn (can be
	 *     greater than 360)
	 * This function makes the robot perform a pivot-turn in the direction and for the angle 
	 * specified as arguments.
     */
    void PivotTurn(char direction, float degrees);
    
    /*
     * direction - either LEFT or RIGHT (which are macros you must define)
	 * This function makes the robot perform a pivot-turn in the direction specified as an 
	 * argument, for the number of degrees previously specified with Set().
     */
    void PivotTurn(char direction);
    
    /*
     * direction - either FORWARD LEFT,	FORWARD RIGHT, BACKWARDS LEFT or BACKWARDS RIGHT
	 * degrees - the number of degrees that the arc turn should travel  
	 * radius - the radius that the arc turn should be taken at
     */
    void ArcTurn(char direction, float degrees, float radius);
    
    /*
     * direction - either	FORWARD_LEFT,	FORWARD_RIGHT, BACKWARDS_LEFT or BACKWARDS_RIGHT 
	 * degrees - the number of degrees that the arc turn should travel 
     */
    void ArcTurn(char direction, float degrees);
    
    /* 
     * direction - either FORWARD LEFT,	FORWARD RIGHT, BACKWARDS LEFT or BACKWARDS RIGHT
     */
    void ArcTurn(char direction);
    
    /*
     *type - either DEGREE, RADIUS, DISTANCE, SPEED or PIN_NUM 
	 *value - the value to set.
	 * This function is used to set either the degree, radius, distance or 
	 * speed properties for later movement. The pin-number refers to a digital pin number 
	 * from 0 to 13. For digital pins, you can set a value to be HIGH (5V), LOW (GND) or 
	 * oscillating according to a PWM signal controlled by analogWrite. In the latter case, 
	 * the value being set will be the duty cycle for the PWM signal.
     */
    void Set(char type, float value);
    
    /*
     * type - either DEGREE, RADIUS, DISTANCE, SPEED or pin-number 
	 * return value - the value stored in the behaviors library that is used for movement 
	 *       functions, or an analog pin value
	 * This function returns the value stored in the behaviors object. For analog pins, the 
	 * return value is the value reported by analogRead.
     */
    float Get(char type);
  private:
  	int curDegrees;
  	int curRadius;
  	char curDirection;
	RaptorSerialInterface* serialInterface;
};


#endif
