#ifndef RAPTOR_WIRE_PROTOCOL_H
#define RAPTOR_WIRE_PROTOCOL_H

#include <stdlib.h>
#include <stdio.h>

#define SERIAL_PACKET_BYTES 4
#define ACK_CHAR 'k'

/* Packet types */
#define SET_PIN 'a'

typedef unsigned char uint8;

typedef struct SerialPacket_t {
	char type;
	uint8 data1;
	uint8 data2;
	uint8 data3;
} SerialPacket;


SerialPacket* setPinPacket(int pin, int val);
void printPacket(SerialPacket* packet);




#endif
