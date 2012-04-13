#include <iostream>
#include "RaptorRemoteServer.h"

int main (int argc, char * const argv[]) {
	
	RaptorRemoteServer server;
	server.startServer();

	while(true);
	
	return 0;
}
