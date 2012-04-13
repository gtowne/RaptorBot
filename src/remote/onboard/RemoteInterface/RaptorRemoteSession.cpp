/*
 *  RaptorRemoteSession.cpp
 *  RaptorRemoteInterface
 *
 *  Created by Gordon Towne on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "RaptorRemoteSession.h"

RaptorRemoteSession::RaptorRemoteSession(int _socketFD, sockaddr_in* _remoteAddr) {
	sessionIsActive = false;
	this->socketFD = _socketFD;
	memcpy(&remoteAddr, _remoteAddr, sizeof(sockaddr_in));

	pthread_mutex_init(&sendMutex, NULL);
	pthread_mutex_init(&receiveMutex, NULL);
}

int RaptorRemoteSession::teardownSession() {
	this->sessionIsActive = false;
	close(this->socketFD);

	pthread_mutex_destroy(&sendMutex);
	pthread_mutex_destroy(&receiveMutex);
}

int RaptorRemoteSession::listeningThreadRoutine() {
	sendInitResponseMessage(true);
	sessionIsActive = true;

	printf("RaptorRemoteSession::Listening thread for session started\n");

	while(this->processIncomingMessage());

	return 0;
}

void* startListeningThread(void* _thisSession) {
    RaptorRemoteSession* thisSession = (RaptorRemoteSession*) _thisSession;
    
    thisSession->listeningThreadRoutine();
    
    printf("RaptorRemoteSession::Listening thread for session exited\n");
    
    return NULL;
}

/*
 * startNewSession() - launches a thread to handle interactions with
 *   the client connected by  error:   crosses initialization ofsockFD
 *   returns 0 on success
 *           -1 on failure
 */
int RaptorRemoteSession::startNewSession() {
    if (pthread_create(&this->listeningThread, NULL, startListeningThread, (void*)this) == 0) { 
        return 0;
    }
    
    return -1;
}

bool RaptorRemoteSession::processIncomingMessage() {

    int bytesRead = 0;
    
    pthread_mutex_lock(&receiveMutex);
    
    bzero(this->receiveBuffer, RECEIVE_BUFFER_SIZE);
    
    // read the message type
    bytesRead = readFully(this->socketFD, this->receiveBuffer, 0, sizeof(AbstractPacket));

    if (bytesRead < 0) {
        
        printf("RaptorRemoteSession::Error reading message type from new message\n");
        
        pthread_mutex_unlock(&receiveMutex);
        
        return true;
        
    } else if (bytesRead == 0) {
        printf("RaptorRemoteSession::Connection closed by remote client\n");
        
        teardownSession();

		return false;
    }

	AbstractPacket* newPacket = (AbstractPacket*)this->receiveBuffer;
	newPacket->messageType = ntohl(newPacket->messageType);

    // switch on the message type received
    switch (this->receiveBuffer[0]) {
    case VID_START:
		printf("RaptorRemoteSession::Vid Init message received\n");

		bytesRead = readFully(this->socketFD, this->receiveBuffer, sizeof(AbstractPacket), sizeof(VideoInitPacket) - sizeof(AbstractPacket));

		VideoInitPacket* packet = (VideoInitPacket*)this->receiveBuffer;

		packet->remoteDataPort = ntohl(packet->remoteDataPort);
		packet->camNum = ntohl(packet->remoteDataPort);
		packet->targetFPS = ntohl(packet->targetFPS);
		packet->remoteIPLen = ntohl(packet->remoteIPLen);

		char remoteIP[128];
		sendVidInitResponseMessage(streamerStarted);
		bzero (remoteIP, 128);
		memcpy(remoteIP, packet->remoteIP, packet->remoteIPLen);


		printf("Connecting to %s\n", remoteIP);

		// if we can construct a new video stream
		if (!this->videoStreamer || (this->videoStreamer && this->videoStreamer->isActive())) {
			printf("RaptorRemoteSession::Initializing new video stream\n");
			this->videoStreamer = new VideoStreamer(remoteIP, packet->remoteDataPort, packet->camNum, packet->targetFPS);
			int streamerStarted = videoStreamer->start();
			bool _streamerStarted = false;
			if (streamerStarted = 1) {
				_streamerStarted = true;			
			}

			sendVidInitResponseMessage(streamerStarted);
		} 

		else { // we can't construct a new video stream
			printf("RaptorRemoteSession::Can't initialize new stream, responding with error\n");
			sendVidInitResponseMessage(false);
		}


		break;

	}
    
    pthread_mutex_unlock(&receiveMutex);
    
    return true;
    
}

bool RaptorRemoteSession::isActive() {
	return sessionIsActive;
}

int RaptorRemoteSession::sendInitResponseMessage(bool success) {
	pthread_mutex_lock(&sendMutex);
    
    bzero(this->sendBuffer, SEND_BUFFER_SIZE);
    
    int packetBytes = newInitRspMsg(this->sendBuffer, success);
    
    int bytesWritten = write(this->socketFD, this->sendBuffer, packetBytes);
    
    if (bytesWritten < packetBytes) {
        pthread_mutex_unlock(&sendMutex);
        return -1;
    }
    
    pthread_mutex_unlock(&sendMutex);
    
    return 0;
}

int RaptorRemoteSession::sendVidInitResponseMessage(bool success) {
	pthread_mutex_lock(&sendMutex);

	bzero(this->sendBuffer, SEND_BUFFER_SIZE);
    
    int packetBytes =  newVidInitRspMsg(this->sendBuffer, success);
    
    int bytesWritten = write(this->socketFD, this->sendBuffer, packetBytes);
    
    if (bytesWritten < packetBytes) {
        pthread_mutex_unlock(&sendMutex);
        return -1;
    }
	pthread_mutex_unlock(&sendMutex);

	return 0;
}

/*
 * sendPingResponseMessage() - send a packet of type PING_RSP to the client,
 *    return 0 on success
 *           -1 on failure
 *
int RaptorRemoteSession::sendPingResponseMessage() {
    pthread_mutex_lock(&sendMutex);
    
    bzero(this->sendBuffer, SEND_BUFFER_SIZE);
    
    int packetBytes = newPingRspMsg(this->sendBuffer, MACHINE_NAME, strlen(MACHINE_NAME));
    
    int bytesWritten = write(this->socketFD, this->sendBuffer, packetBytes);
    
    if (bytesWritten < packetBytes) {
        pthread_mutex_unlock(&sendMutex);
        return -1;
    }
    
    pthread_mutex_unlock(&sendMutex);
    
    return 0;
}*/




