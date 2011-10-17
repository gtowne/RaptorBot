#include "RaptorWireProtocol.h"

/*  INTERNAL FUNCTIONS */


SerialPacket* newPacket() {
	return (SerialPacket*)malloc(SERIAL_PACKET_BYTES);
}



/*  PUBLIC FUNCTIONS */
SerialPacket* setPinPacket(char val) {
	SerialPacket* packet = newPacket();
	packet->type = SET_PIN;
	packet->data1 = val;
}

