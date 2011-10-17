#ifndef RAPTOR_WIRE_PROTOCOL_H
#define RAPTOR_WIRE_PROTOCOL_H

#include <stdlib.h>

#define SERIAL_PACKET_BYTES 4


/* Packet types */
#define SET_PIN 'a'

typedef struct SerialPacket_t {
	char type;
	char data1;
	char data2;
	char data3;
} SerialPacket;


SerialPacket* setPinPacket(char val);





#endif
