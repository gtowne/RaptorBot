#include "ControlInterface.h"

ControlInterface::ControlInterface() {
	serialInterface = new RaptorSerialInterface();
	serialInterface->InitConnection(ARDUINO_SERIAL_DEVICE_NAME, ARDUINO_SERIAL_BAUDRATE, 2000);
	printf("Initialized serial connection to Arduino\n");
}

ControlInterface::~ControlInterface() {

}

int ControlInterface::Move(char direction, float distance, float speed) {
	return 1;

}


int ControlInterface::Move(char direction, float distance) {
	return 1;

}


int ControlInterface::Move(char direction) {
	return 1;

}

int ControlInterface::PivotTurn(char direction, float degrees) {
	return 1;

}


int ControlInterface::PivotTurn(char direction) {
	return 1;


}

int ControlInterface::ArcTurn(char direction, float degrees, float radius) {
	return 1;

}


int ControlInterface::ArcTurn(char direction, float degrees) {
	return 1;


}

int ControlInterface::ArcTurn(char direction) {
	return 1;


}

int ControlInterface::Set(char type, float value) {
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

int ControlInterface::SetPin(int pinNum, int pwmVal) {
	if (!serialInterface) {
		return 0;
	}

	SerialPacket* packet = setPinPacket(pinNum, pwmVal);

	if (!packet) {
		return 0;
	}

	serialInterface->SendBuff(packet, SERIAL_PACKET_BYTES);

	return 1;
}
