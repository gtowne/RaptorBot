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
#include "utils/MySocketUtils.h"
#include "VideoStreamer/VideoStreamer.h"
#include "ControlInterface/BehaviorQueue.h"


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
#define SOCKET_READ_ERROR 0x100
#define UNKNOWN_MESSAGE_TYPE 0x101
#define SOCKET_CLOSED 0x102



class RaptorRemoteSession {
private:
	int socketFD;
	int feedbackSocketFD;

	bool sessionIsActive;

    struct sockaddr_in remoteAddr;
	char sendBuffer[SEND_BUFFER_SIZE];
	char receiveBuffer[RECEIVE_BUFFER_SIZE];

	char feedbackSendBuffer[SEND_BUFFER_SIZE];
    
    // mutexes for the send and receive buffers
    pthread_mutex_t sendMutex;
    pthread_mutex_t receiveMutex;
    
    pthread_t listeningThread;
	pthread_t feedbackThread;

	VideoStreamer* videoStreamer;
    BehaviorQueue behaviorQueue;

	int sendInitResponseMessage(bool success);
	int sendVidInitResponseMessage(bool sucess);
	
	int teardownSession();

	int handleManeuver(int maneuver, int direction, int degrees, int speed, int distance, int radius);

	
public:
	RaptorRemoteSession(int _socketFD, int _feedbackSocketFD, sockaddr_in* _remoteAddr);
	~RaptorRemoteSession();
	
    /*
     * getIncomingMessage() - block until we receive a message from the client,
     *   populate the fields of a RaptorSessionMessage struct based on the data
     *   we receive, and return that struct
     */
	bool processIncomingMessage();
    
    int startNewSession();
    int listeningThreadRoutine();
	int feedbackThreadRoutine();
	bool isActive();
};

#endif
