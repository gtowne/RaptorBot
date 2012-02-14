/*
 *  RaptorRemoteServer.cpp
 *  RaptorRemoteInterface
 *
 *  Created by Gordon Towne on 1/17/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "RaptorRemoteServer.h"


/*---------------------------------------------------------------------------
 *------------------------------ PUBLIC FUNCTIONS ---------------------------
 *---------------------------------------------------------------------------
 */

RaptorRemoteServer::RaptorRemoteServer() {
	this->lastSessionID = 1;
}

RaptorRemoteServer::~RaptorRemoteServer() {
	
}

int RaptorRemoteServer::startServer() {
    
    
    this->serverSocketFD = openListeningSocket(SERVER_LISTENING_PORT_NO);
    
    if (this->serverSocketFD < 0) {
        printf("Failed opening server socket");
        return -1;
    }
    
    printf("Listening socket opened\n");
   
    while (true) {
        
        RaptorRemoteSession* newSession = acceptNewSession();
        
        printf("New connection received\n");
        
        RaptorSessionMessage* message = newSession->getIncomingMessage();
        
        switch (message->messageType) {
            case PING_MSG:
                
                newSession->sendPingResponseMessage();
                delete newSession;
                
                break;
                
            case INIT_MSG:
                
                newSession->startNewSession();
                
                break;
                
            default:
                
                printf("Received unsupported message type from session\n");
                break;
        }
        
        
    }
	
	return 0;
}


/*---------------------------------------------------------------------------
 *------------------------------ PRIVATE FUNCTIONS --------------------------
 *---------------------------------------------------------------------------
 */



RaptorRemoteSession* RaptorRemoteServer::acceptNewSession() {
	int newSocketFD = acceptConnection(this->serverSocketFD);
	
	RaptorRemoteSession* newSession = new RaptorRemoteSession(newSocketFD);
	
	return newSession;
}

int RaptorRemoteServer::getNewSessionID() {
	int newID = (lastSessionID + 1) % MAX_SESSION_ID;
	this->lastSessionID = newID;
	
	return newID;
}
