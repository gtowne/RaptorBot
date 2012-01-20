/*
 *  RaptorRemoteSession.cpp
 *  RaptorRemoteInterface
 *
 *  Created by Gordon Towne on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "RaptorRemoteSession.h"

RaptorRemoteSession::RaptorRemoteSession(int _socketFD) {
	this->socketFD = _socketFD;
    
    pthread_mutex_init(&sendMutex, NULL);
    pthread_mutex_init(&receiveMutex, NULL);
}

RaptorRemoteSession::~RaptorRemoteSession() {
	close(this->socketFD);
    pthread_mutex_destroy(&this->sendMutex);
    pthread_mutex_destroy(&this->receiveMutex);
}

/*
 * getIncomingMessage() - block until we receive a message from the client,
 *   populate the fields of a RaptorSessionMessage struct based on the data
 *   we receive, and return that struct
 */
RaptorSessionMessage* RaptorRemoteSession::getIncomingMessage() {
	
	bool messageReceived = false;
    
    size_t bytesRead;
    
    RaptorSessionMessage* returnMessage = (RaptorSessionMessage*)malloc(sizeof(RaptorSessionMessage));
    
	while (!messageReceived) {
	
        pthread_mutex_lock(&receiveMutex);
        
        // read the protocol id
		bzero(this->receiveBuffer, RECEIVE_BUFFER_SIZE);
		bytesRead = recv(this->socketFD, (this->receiveBuffer), MEMBER_SIZE(NetworkHeader, protocolID), 0);
		
        if (bytesRead < 1) {
            
            pthread_mutex_unlock(&receiveMutex);
            
            returnMessage->messageType = SOCKET_READ_ERROR;
            return returnMessage;
        }
		
        // continue if sender's protocol version matches ours
		if (this->receiveBuffer[0] == RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID) {
			
            
            // read the message type
			bytesRead = recv(this->socketFD, (this->receiveBuffer), MEMBER_SIZE(NetworkHeader, messageType), 0);
		
            if (bytesRead < 1) {
                
                pthread_mutex_unlock(&receiveMutex);
                
                returnMessage->messageType = SOCKET_READ_ERROR;
                return returnMessage;
            }
            
			// switch on the message type received
			switch (this->receiveBuffer[0]) {
				case PING_MSG:
					printf("ping message received\n");
					
                    returnMessage->messageType = PING_MSG;
                    
					messageReceived = true;
                    
					break;
					
					
				default:
					printf("unknown message type received\n");
                    
                    returnMessage->messageType = UNKNOWN_MESSAGE_TYPE;
                    
                    messageReceived = true;
                    
					break;
			}
            
            pthread_mutex_unlock(&receiveMutex);
            
            return returnMessage;
		}
	}
	
	return NULL;
}

/*
 * sendPingResponseMessage() - send a packet of type PING_RSP to the client,
 *    return 0 on success
 *           -1 on failure
 */
int RaptorRemoteSession::sendPingResponseMessage() {
    pthread_mutex_lock(&sendMutex);
    
    bzero(this->sendBuffer, SEND_BUFFER_SIZE);
    
    sendBuffer[0] = RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID;
    sendBuffer[1] = PING_RSP_MSG;
    int packetBytes = 2;
    
    int bytesWritten = write(this->socketFD, this->sendBuffer, packetBytes);
        
    if (bytesWritten)
    
    pthread_mutex_unlock(&sendMutex);
    
    return 0;
}