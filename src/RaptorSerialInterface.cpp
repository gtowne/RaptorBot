#include "RaptorSerialInterface.h"

/*   Public methods  */
RaptorSerialInterface::RaptorSerialInterface() {
	deviceFD = -1;
	bzero(&deviceName, 256);
	bzero(&receiveBuffer, RECEIVE_BUFFER_SIZE);
}

int RaptorSerialInterface::InitConnection(char* _deviceName, int _baudrate, int _delayMS) {
	int _fd = serialport_init(_deviceName, _baudrate);
	
	if (_fd > -1) {
		deviceFD = _fd;
		strcpy(_deviceName, deviceName);

		// sleep while connection is established
		usleep(_delayMS * 1000);
		return 1;
	} 

	return 0;
}


int RaptorSerialInterface::SendBuff(char* buff, int len) {
	serialport_write(deviceFD, buff, len);
	return 1;
}
	
int RaptorSerialInterface::Receive(char* buff, int max_len) {
	return 1;
}

int RaptorSerialInterface::CloseConnection() {
	close(deviceFD);
}


/*  -----------------------------------------------------------------
------------------------- Private methods  --------------------------
---------------------------------------------------------------------*/
int RaptorSerialInterface::serialport_write(int fd, const char* str, int len) {
    	int n = write(fd, str, len);
    	if( n!=len ) 
        	return -1;
    	return 0;
}
	
int RaptorSerialInterface::serialport_read_until(int fd, char* buff, int max_len) {
	char b[1];
    	int i=0;

	for (int i = 0; i < max_len; i++) {
		int n = read(fd, b, 1);
		if (n == -1)  {
			return -1;
		} else if (n == 0) {
			usleep(10000);
			continue;
		}
		buff[i] = b[0];
	}

    	return max_len;
}

// takes the string name of the serial port (e.g. "/dev/tty.usbserial","COM1")
// and a baud rate (bps) and connects to that port at that speed and 8N1.
// opens the port in fully raw mode so you can send binary data.
// returns valid fd, or -1 on error
int RaptorSerialInterface::serialport_init(const char* serialport, int baud)
{
    struct termios toptions;
    int fd;
    
    //fprintf(stderr,"init_serialport: opening port %s @ %d bps\n",
    //        serialport,baud);

    fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)  {
        perror("init_serialport: Unable to open port ");
        return -1;
    }
    
    if (tcgetattr(fd, &toptions) < 0) {
        perror("init_serialport: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baud; // let you override switch below if needed
    switch(baud) {
    case 4800:   brate=B4800;   break;
    case 9600:   brate=B9600;   break;
#ifdef B14400
    case 14400:  brate=B14400;  break;
#endif
    case 19200:  brate=B19200;  break;
#ifdef B28800
    case 28800:  brate=B28800;  break;
#endif
    case 38400:  brate=B38400;  break;
    case 57600:  brate=B57600;  break;
    case 115200: brate=B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);

    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw

    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;
    
    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }

    return fd;
}
