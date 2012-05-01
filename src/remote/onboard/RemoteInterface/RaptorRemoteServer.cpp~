/*
 *  RaptorRemoteServer.cpp
 *  RaptorRemoteInterface
 *
 *  Created by Gordon Towne on 1/17/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "RaptorRemoteServer.h"

void* startServerThread(void* arg) {
	RaptorRemoteServer* server = (RaptorRemoteServer*)arg;
	server->serverThreadRoutine();

	return NULL;
}

RaptorRemoteServer::RaptorRemoteServer() {
	activeSession = NULL;
}

RaptorRemoteServer::~RaptorRemoteServer() {

}


int RaptorRemoteServer::startServer() {
	serverSocketFD =  openListeningSocket(SERVER_LISTENING_PORT_NO);

	if (serverSocketFD < 0) {
		printf("RaptorRemoteServer::Error, could not open server socket\n");
		return -1;
	}
	

	return pthread_create(&serverThread, NULL, startServerThread, (void*) this);	

}



int RaptorRemoteServer::serverThreadRoutine() {
	printf("RaptorRemoteServer::Server started\n");

	while(true) {

		struct sockaddr_in hostAddr;

		int newSocketFD = acceptConnection(serverSocketFD, &hostAddr);

		InitPacket* newInitPacket = NULL;

		if (newSocketFD > 0) {
			printf("RaptorRemoteServer::New incoming conneciton received\n");

			recv(newSocketFD, recvBuff, sizeof(AbstractPacket), 0);
			AbstractPacket* newHeader = (AbstractPacket*)recvBuff;

			newHeader->messageType = ntohl(newHeader->messageType);

			printf("Message type: %x\n", newHeader->messageType);

			switch(newHeader->messageType) {
			case PING_MSG:
				printf("RaptorRemoteServer::Received Ping Message\n");

				sendPingResponse(newSocketFD, MACHINE_NAME);
				break;

			case INIT_SESSION_MSG:
				printf("RaptorRemoteServer::Received Init Session Message\n");
				
				readFully(newSocketFD, recvBuff, sizeof(AbstractPacket), sizeof(InitPacket) - sizeof(AbstractPacket));

				newInitPacket = (InitPacket*)recvBuff;

				newInitPacket->remoteFeedbackServerPort = ntohl(newInitPacket->remoteFeedbackServerPort);
				newInitPacket->remoteIPLen = ntohl(newInitPacket->remoteIPLen);

				char remoteIP[128];
				bzero (remoteIP, 128);
				memcpy(remoteIP, newInitPacket->remoteIP, newInitPacket->remoteIPLen);


				if (sessionIsActive()) {
					printf("RaptorRemoteServer::There is already an active session, replying to new Init request with failure\n");
					sendInitResponse(newSocketFD, false);
				} else {
					printf("RaptorRemoteServer::Opening feedback socket to %s at port %d\n", remoteIP, newInitPacket->remoteFeedbackServerPort);
					int feedbackSocketFD = openTCPSocket(remoteIP, newInitPacket->remoteFeedbackServerPort);

					if (feedbackSocketFD < 0) {
						sendInitResponse(newSocketFD, false);
					} else {

						activeSession = new RaptorRemoteSession(newSocketFD, feedbackSocketFD, &hostAddr);
						activeSession->startNewSession();
					}
				}

				break;

			default:
				printf("RaptorRemoteServer::Received unsupported message type\n");
				break;
			}
		} else {
			printf("RaptorRemoteServer::Error accepting new incoming connection\n");
		}
	}

	return 0;
}

bool RaptorRemoteServer::sessionIsActive() {
	if (activeSession!=NULL && activeSession->isActive()) {
		return true;
	}

	return false;
}

int RaptorRemoteServer::sendPingResponse(int socketFD, char* machineName) {
	printf("RaptorRemoteServer::Sending Ping Response Message\n");
	int bytesInBuffer = newPingRspMsg(sendBuff, machineName, strlen(machineName));
	int bytesWritten = write(socketFD, sendBuff, bytesInBuffer);

	if (bytesWritten != bytesInBuffer) {
		return -1;
	}

	return 0;
}

int RaptorRemoteServer::sendInitResponse(int socketFD, bool success) {
	printf("RaptorRemoteServer::Sending Init Response Message::");
	if (success) {
		printf("success\n");
	} else {
		printf("failure\n");
	}

	int bytesInBuffer = newInitRspMsg(sendBuff, success);
	int bytesWritten = write(socketFD, sendBuff, bytesInBuffer);

	if (bytesWritten != bytesInBuffer) {
		return -1;
	}

	return 0;
}




