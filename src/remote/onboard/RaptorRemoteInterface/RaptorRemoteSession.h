/*
 *  RaptorRemoteSession.h
 *  RaptorRemoteInterface
 *
 *  Created by Gordon Towne on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RAPTOR_REMOTE_SESSION_H
#define RAPTOR_REMOTE_SESSION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "RaptorRemoteSessionProtocol.h"
#include "RaptorVideoStream.h"


#define SEND_BUFFER_SIZE 256
#define RECEIVE_BUFFER_SIZE 256

// this macro is useful for determining the size of 
// a member of a struct
#define MEMBER_SIZE(type, member) sizeof(((type *)0)->member)

/*
 * Additional error message types for
 * RaptorSessionMessage, valid message types
 * are the same as those sent in the wire protocol
 * defined in RaptorRemoteSessionProtocol.h
 */
#define SOCKET_READ_ERROR '.'
#define UNKNOWN_MESSAGE_TYPE ','


typedef struct RaptorSessionMessage_t {
	char messageType;
} RaptorSessionMessage;




class RaptorRemoteSession {
private:
	int socketFD;
	char sendBuffer[SEND_BUFFER_SIZE];
	char receiveBuffer[RECEIVE_BUFFER_SIZE];
    
    // mutexes for the send and receive buffers
    pthread_mutex_t sendMutex;
    pthread_mutex_t receiveMutex;
    
    pthread_t listeningThread;
    
    RaptorVideoStream* videoStream;
    
    bool connected;
    bool everybodyDie;
    
    bool teardownSession();
    bool checkSessionIntegrity();
    
    int sendPingMessage();
    int sendInitResponseMessage(bool success);
    int sendQuitMessage();
    int sendQuitResponseMessage();
    int sendVideoStartRsp(bool success);
    int sendVideoEndRsp(bool success);
    
    int handleVideoStart();
    int handleVideoEnd();
    
	
public:
	RaptorRemoteSession(int socketFD);
	~RaptorRemoteSession();
	
    /*
     * getIncomingMessage() - block until we receive a message from the client,
     *   populate the fields of a RaptorSessionMessage struct based on the data
     *   we receive, and return that struct
     */
	RaptorSessionMessage* getIncomingMessage();
    
    /*
     * sendPingResponseMessage() - send a packet of type PING_RSP to the client,
     *    return 0 on success
     *           -1 on failure
     */
    int sendPingResponseMessage();
    int sendConnectResponseMessage();
    
    int startNewSession();
    int listeningThreadRoutine();
};




#endif