#include "ControlInterface.h"

ControlInterface::ControlInterface() {
	serialInterface = new RaptorSerialInterface();
	serialInterface->InitConnection(ARDUINO_SERIAL_DEVICE_NAME, ARDUINO_SERIAL_BAUDRATE, 2000);
}

ControlInterface::~ControlInterface() {

}

void ControlInterface::Move(char direction, float distance, float speed) {


}


void ControlInterface::Move(char direction, float distance) {


}


void ControlInterface::Move(char direction) {


}

void ControlInterface::PivotTurn(char direction, float degrees) {


}


void ControlInterface::PivotTurn(char direction) {



}

void ControlInterface::ArcTurn(char direction, float degrees, float radius) {


}


void ControlInterface::ArcTurn(char direction, float degrees) {



}

void ControlInterface::ArcTurn(char direction) {



}

void ControlInterface::Set(char type, float value) {
	switch (type) {
		case PIN_NUM:
			
			break;
		default:
			break;

	}

}


float ControlInterface::Get(char type) {

	return 1;
}
