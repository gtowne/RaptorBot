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
	this->socketFD = _socketFD;
    
    struct timeval timeout;
    timeout.tv_sec = 10; // set timeout to 10 seconds
    setsockopt(this->socketFD, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    
    remoteAddr = (sockaddr_in*) malloc(sizeof(sockaddr_in));
    memcpy(this->remoteAddr, _remoteAddr, sizeof(sockaddr_in));
    
    this->connected = false;
    this->everybodyDie = false;
    
    pthread_mutex_init(&sendMutex, NULL);
    pthread_mutex_init(&receiveMutex, NULL);
}

RaptorRemoteSession::~RaptorRemoteSession() {
	teardownSession();
}

bool RaptorRemoteSession::teardownSession() {
    
    // let any threads trying to send finish sending,
    // then close the socket
    pthread_mutex_lock(&this->sendMutex);
    close(this->socketFD);
    pthread_mutex_unlock(&this->sendMutex);
    
    free(this->remoteAddr);
    
    this->connected = false;
    this->everybodyDie = true;
    
    pthread_mutex_destroy(&this->sendMutex);
    pthread_mutex_destroy(&this->receiveMutex);
    
    return true;
}

bool RaptorRemoteSession::checkSessionIntegrity() {
    if (connected) {
        int success = sendPingMessage();
        
        if (success == -1) {
            return false;
        }
    }
    
    return true;
}


int RaptorRemoteSession::listeningThreadRoutine() {
    
    printf("Listening thread started\n");
    
    this->sendInitResponseMessage(true);
    
    this->connected = true;
    
    AbstractPacket* newMessage;
    while(true) {
        
        if (everybodyDie) {
            return 0;
        }
        
        printf("Waiting for incoming message\n");
        newMessage = this->getIncomingMessage();
        printf("Got new incoming message\n");
        
        
        bool allIsWell;
        int remoteDataPort;
        int remoteServerPort;
        VideoInitPacket* videoInitMessage;
        
        switch (newMessage->messageType) {
            case PING_MSG:
                printf("processing ping message\n");
                
                this->sendPingResponseMessage();
                
                break;
                
            case QUIT_MSG:
                
                printf("processing quit message\n");
                
                this->sendQuitResponseMessage();
                
                this->teardownSession();
                
                break;
                
            case VID_START:
                printf("processing vid start message\n");
                
                // @TODO - get the return value from handleVideo
                // for success of video and and populate field
                // in response packet appropriately
                
                
                videoInitMessage = (VideoInitPacket*)newMessage;
                
                
                printf("Data port %d, Server port %d\n", remoteDataPort, remoteServerPort);
                
                //this->sendVideoStartRsp(true);
                
                break;
                
                /*               
                 case VID_END:
                 
                 printf("processing vid end message\n");
                 
                 
                 // @TODO - get the return value from handleVideo
                 // for success of video and and populate field
                 // in response packet appropriately
                 this->handleVideoEnd();
                 
                 this->sendVideoEndRsp(true);
                 
                 break;*/
                
            case SOCKET_READ_ERROR:
                allIsWell = checkSessionIntegrity();
                
                if (!allIsWell) {
                    this->sendQuitMessage();
                    this->teardownSession();
                }
                
                break;
                
            case SOCKET_CLOSED:
                this->teardownSession();
                
                break;
                
            case UNKNOWN_MESSAGE_TYPE:
            default:
                printf("encountered unexpected message type\n");
                break;
        }
    }
    
    return 0;
}

void* startListeningThread(void* _thisSession) {
    RaptorRemoteSession* thisSession = (RaptorRemoteSession*) _thisSession;
    
    thisSession->listeningThreadRoutine();
    
    printf("Listening thread for session exited\n");
    
    return NULL;
}

/*
 * startNewSession() - launches a thread to handle interactinos with
 *   the client connected by sockFD
 *   returns 0 on success
 *           -1 on failure
 */
int RaptorRemoteSession::startNewSession() {
    if (pthread_create(&this->listeningThread, NULL, startListeningThread, (void*)this) == 0) { 
        return 0;
    }
    
    this->sendInitResponseMessage(false);
    
    return -1;
}

/*
 * getIncomingMessage() - block until we receive a message from the client,
 *   populate the fields of a RaptorSessionMessage struct based on the data
 *   we receive, and return that struct
 */
AbstractPacket* RaptorRemoteSession::getIncomingMessage() {
	
	bool messageReceived = false;
    
    int bytesRead;
    
    AbstractPacket* returnMessage = (AbstractPacket*)malloc(MAX_PACKET_BYTES);
    VideoInitPacket* _videoInitMessage;
    
    
    pthread_mutex_lock(&receiveMutex);
    
    bzero(this->receiveBuffer, RECEIVE_BUFFER_SIZE);
    
    
    // read the message type
    bytesRead = readFully(this->socketFD, this->receiveBuffer, 0, sizeof(AbstractPacket));
    
    if (bytesRead < 0) {
        
        printf("Error reading message type from new message\n");
        
        pthread_mutex_unlock(&receiveMutex);
        
        returnMessage->messageType = SOCKET_READ_ERROR;
        
        return returnMessage;
        
    } else if (bytesRead == 0) {
        printf("Connection closed by remote client");
        
        pthread_mutex_unlock(&receiveMutex);
        
        returnMessage->messageType = SOCKET_CLOSED;
        
        return returnMessage;
    }
    
    // switch on the message type received
    switch (this->receiveBuffer[0]) {
        case PING_MSG:
            printf("ping message received\n");
                        
            bytesRead = readFully(this->socketFD, this->receiveBuffer, bytesRead, sizeof(PingPacket));
            
            returnMessage->messageType = PING_MSG;
            
            messageReceived = true;
            
            break;
            
        case INIT_MSG:
            printf("init message received\n");
                        
            bytesRead = recv(this->socketFD, (this->receiveBuffer), bytesRead, sizeof(InitPacket));
            
            returnMessage->messageType = INIT_MSG;
            
            messageReceived = true;
            
            break;
            
        case QUIT_MSG:
            printf("quit message received\n");
            
            bytesRead = readFully(this->socketFD, this->receiveBuffer, bytesRead, sizeof(QuitPacket));
            
            returnMessage->messageType = QUIT_MSG;
            
            break;
            
        case VID_START:
            printf("vid start message received\n");
            
            returnMessage->messageType = VID_START;
            
            bytesRead = readFully(this->socketFD, this->receiveBuffer, bytesRead, sizeof(VideoInitPacket));
            
            if (bytesRead < sizeof(VideoInitPacket)) {
                
                pthread_mutex_unlock(&receiveMutex);
                
                returnMessage->messageType = SOCKET_READ_ERROR;
                return returnMessage;
            }
            
            memcpy(returnMessage, this->receiveBuffer, sizeof(VideoInitPacket));
            
            _videoInitMessage = (VideoInitPacket*) returnMessage;
            _videoInitMessage->remoteDataPort = ntohl(_videoInitMessage->remoteDataPort);
            _videoInitMessage->remoteServerPort = ntohl(_videoInitMessage->remoteServerPort);
            
            break;
            
            /*
             
             case VID_END:
             printf("vid end message received\n");
             
             returnMessage->messageType = VID_END;
             
             messageReceived = true;
             
             break;*/
            
        default:
            printf("unknown message type received\n");
            
            returnMessage->messageType = UNKNOWN_MESSAGE_TYPE;
            
            messageReceived = true;
            
            break;
    }
    
    pthread_mutex_unlock(&receiveMutex);
    
    return returnMessage;
    
}

/*
 * sendPingResponseMessage() - send a packet of type PING_RSP to the client,
 *    return 0 on success
 *           -1 on failure
 */
int RaptorRemoteSession::sendPingResponseMessage() {
    pthread_mutex_lock(&sendMutex);
    
    bzero(this->sendBuffer, SEND_BUFFER_SIZE);
    
    int packetBytes = newPingRspMsg(this->sendBuffer);
    
    int bytesWritten = write(this->socketFD, this->sendBuffer, packetBytes);
    
    if (bytesWritten < packetBytes) {
        pthread_mutex_unlock(&sendMutex);
        return -1;
    }
    
    pthread_mutex_unlock(&sendMutex);
    
    return 0;
}

int RaptorRemoteSession::sendPingMessage() {
    pthread_mutex_lock(&sendMutex);
    
    bzero(this->sendBuffer, SEND_BUFFER_SIZE);
    
    int packetBytes = newPingMsg(this->sendBuffer);
    
    int bytesWritten = write(this->socketFD, this->sendBuffer, packetBytes);
    
    if (bytesWritten < packetBytes) {
        pthread_mutex_unlock(&sendMutex);
        return -1;
    }
    
    pthread_mutex_unlock(&sendMutex);
    
    return 0;
}

int RaptorRemoteSession::sendInitResponseMessage(bool success) {
    pthread_mutex_lock(&this->sendMutex);
    
    bzero(this->sendBuffer, SEND_BUFFER_SIZE);
    
    int bytesInBuffer = newInitRspMsg(this->sendBuffer, success);
    
    int bytesSent = write(this->socketFD, this->sendBuffer, bytesInBuffer);
    
    if (bytesSent != bytesInBuffer) {
        pthread_mutex_unlock(&this->sendMutex);
        return -1;
    }
    
    pthread_mutex_unlock(&this->sendMutex);
    
    return 0;
}

int RaptorRemoteSession::sendQuitResponseMessage() {
    pthread_mutex_lock(&this->sendMutex);
    
    bzero(this->sendBuffer, SEND_BUFFER_SIZE);
    
    sendBuffer[0] = RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID;
    sendBuffer[1] = QUIT_RSP_MSG;
    
    int packetBytes = 2;
    
    int bytesWritten = write(this->socketFD, this->sendBuffer, packetBytes);
    
    if (bytesWritten < packetBytes) {
        pthread_mutex_unlock(&this->sendMutex);
        return -1;
    }
    
    pthread_mutex_unlock(&this->sendMutex);
    
    return 0;
}

int RaptorRemoteSession::sendQuitMessage() {
    pthread_mutex_lock(&this->sendMutex);
    
    bzero(this->sendBuffer, SEND_BUFFER_SIZE);
    
    int packetBytes = newQuitMsg(this->sendBuffer);
    
    int bytesWritten = write(this->socketFD, this->sendBuffer, packetBytes);
    
    if (bytesWritten != packetBytes) {
        pthread_mutex_unlock(&this->sendMutex);
        return -1;
    }
    
    pthread_mutex_unlock(&this->sendMutex);
    
    return 0;
}

