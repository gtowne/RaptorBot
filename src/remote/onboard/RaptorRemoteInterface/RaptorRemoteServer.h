/*
 *  RaptorRemoteServer.h
 *  RaptorRemoteInterface
 *
 *  Created by Gordon Towne on 1/17/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RAPTOR_REMOTE_SERVER_H
#define RAPTOR_REMOTE_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "RaptorRemoteSessionProtocol.h"
#include "MyTCPSocketUtils.h"
#include "RaptorRemoteSession.h"

#define SERVER_LISTENING_PORT_NO 7234
#define MAX_SESSION_ID (1 << 30);

class RaptorRemoteServer {
	
private:
	int serverSocketFD;
	int lastSessionID;
	

	RaptorRemoteSession* acceptNewSession(); 
	int getNewSessionID();
	
	
public:
	RaptorRemoteServer();
	~RaptorRemoteServer();
	
	int startServer();
	
};

int startServer();

#endif