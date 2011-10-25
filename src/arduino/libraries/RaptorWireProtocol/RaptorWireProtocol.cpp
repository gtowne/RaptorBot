#include "RaptorWireProtocol.h"

/*  INTERNAL FUNCTIONS */


SerialPacket* newPacket() {
	return (SerialPacket*)malloc(SERIAL_PACKET_BYTES);
}



/*  PUBLIC FUNCTIONS */
SerialPacket* setPinPacket(int pin, int val) {
	if (pin > 255 || val > 255) {
		return NULL;
	}

	SerialPacket* packet = newPacket();

	packet->type = SET_PIN;
	packet->data1 = (uint8)pin;
	packet->data2 = (uint8)val;
	packet->data3 = (uint8)'r';

	return packet;
}

void printPacket(SerialPacket* packet) {
	printf("Packet fields %c, %u, %u, %u\n", packet->type, packet->data1, packet->data2, packet->data3);
}

