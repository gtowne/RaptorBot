#ifndef RAPTOR_SERIAL_INTERFACE_H
#define RAPTOR_SERIAL_INTERFACE_H

#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h> 
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <getopt.h>

#include "RaptorWireProtocol.h"

#define RECEIVE_BUFFER_SIZE 256

class RaptorSerialInterface {
	public:
		RaptorSerialInterface();

		int InitConnection(char* _deviceName, int _baudrate, int _delayMS);

		int SendBuff(void* buff, int bytes);

		int Receive(void* buff, int max_bytes);

		int CloseConnection();

	private:
		int deviceFD;
		int baudRate;
		char deviceName[256];
		char receiveBuffer[RECEIVE_BUFFER_SIZE];

		int serialport_write(int fd, const char* str, int len);
		int serialport_read_bytes(int fd, char* buf, int max_len);
		int serialport_init(const char* serialport, int baud);
};



#endif
