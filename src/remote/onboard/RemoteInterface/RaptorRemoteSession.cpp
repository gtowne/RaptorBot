/*
 *  RaptorRemoteSession.cpp
 *  RaptorRemoteInterface
 *
 *  Created by Gordon Towne on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "RaptorRemoteSession.h"

RaptorRemoteSession::RaptorRemoteSession(int _socketFD, int _feedbackSocketFD, sockaddr_in* _remoteAddr) {
	sessionIsActive = false;
	this->socketFD = _socketFD;
	this->feedbackSocketFD = _feedbackSocketFD;
	memcpy(&remoteAddr, _remoteAddr, sizeof(sockaddr_in));

	this->videoStreamer = NULL;

	pthread_mutex_init(&sendMutex, NULL);
	pthread_mutex_init(&receiveMutex, NULL);
}

int RaptorRemoteSession::teardownSession() {
	this->sessionIsActive = false;
	if (videoStreamer) {
		videoStreamer->stop();
	}
	close(this->socketFD);

	pthread_mutex_destroy(&sendMutex);
	pthread_mutex_destroy(&receiveMutex);
}

int RaptorRemoteSession::listeningThreadRoutine() {
	sessionIsActive = true;
	sendInitResponseMessage(true);

	printf("RaptorRemoteSession::Listening thread for session started\n");

	while(this->processIncomingMessage());

	return 0;
}

void* startFeedbackThread(void* arg) {
	RaptorRemoteSession* _this = (RaptorRemoteSession*)arg;
	_this->feedbackThreadRoutine();

	printf("RaptorRemoteSession::Feedback thread for session exited\n");

	return NULL;
}

int RaptorRemoteSession::feedbackThreadRoutine() {
	sessionIsActive = true;
	printf("RaptorRemoteSession::Feedback thread started\n");

	Maneuver* curManeuver;
	Maneuver* nextManeuver;
	int msToCompletion;
	FeedbackPacket* thisPacket;

	while(this->sessionIsActive) {
		usleep(500000);

		curManeuver = behaviorQueue.getCurrentManeuver();
		nextManeuver = behaviorQueue.getNextManeuver();
		msToCompletion = behaviorQueue.getMSToManeuverCompletion();
		

		bzero(feedbackSendBuffer, sizeof(FeedbackPacket));

		thisPacket = (FeedbackPacket*)feedbackSendBuffer;

		thisPacket->messageType = htonl(FEEDBACK_MSG);
		
		switch(curManeuver->maneuverType) {
		case BEHAVIOR_NONE:
			thisPacket->currentManeuver = htonl(MAN_NONE);
			break;
		case BEHAVIOR_MOVE:
			thisPacket->currentManeuver = htonl(MAN_MOVE);
			break;
		case BEHAVIOR_ARC:
			thisPacket->currentManeuver = htonl(MAN_ARC);
			break;
		case BEHAVIOR_PIVOT:
			thisPacket->currentManeuver = htonl(MAN_PIVOT);
			break;
		}

		switch(curManeuver->direction) {
		case FORWARDS:
			thisPacket->currentDirection = htonl(DIR_FORWARD);
			break;
		case BACKWARDS:
			thisPacket->currentDirection = htonl(DIR_BACKWARD);
			break;
		case LEFT:
			thisPacket->currentDirection = htonl(DIR_LEFT);
			break;
		case RIGHT:
			thisPacket->currentDirection = htonl(DIR_RIGHT);
			break;
		case FORWARDS_LEFT:
			thisPacket->currentDirection = htonl(DIR_FORWARD_LEFT);
			break;
		case FORWARDS_RIGHT:
			thisPacket->currentDirection = htonl(DIR_FORWARD_RIGHT);			
			break;
		case BACKWARDS_LEFT:
			thisPacket->currentDirection = htonl(DIR_BACKWARD_LEFT);
			break;
		case BACKWARDS_RIGHT:
			thisPacket->currentDirection = htonl(DIR_BACKWARD_RIGHT);
			break;
		}

		thisPacket->currentDegrees = htonl((int)(curManeuver->degrees * FLOAT_SCALE_FACTOR));
		thisPacket->currentSpeed = htonl((int)(curManeuver->speed * FLOAT_SCALE_FACTOR));
		thisPacket->currentDistance = htonl((int)(curManeuver->distance * FLOAT_SCALE_FACTOR));
		thisPacket->currentRadius = htonl((int)(curManeuver->radius * FLOAT_SCALE_FACTOR));
		thisPacket->msToNext = htonl(msToCompletion);

		switch(nextManeuver->maneuverType) {
		case BEHAVIOR_NONE:
			thisPacket->nextManeuver = htonl(MAN_NONE);
			break;
		case BEHAVIOR_MOVE:
			thisPacket->nextManeuver = htonl(MAN_MOVE);
			break;
		case BEHAVIOR_ARC:
			thisPacket->nextManeuver = htonl(MAN_ARC);
			break;
		case BEHAVIOR_PIVOT:
			thisPacket->nextManeuver = htonl(MAN_PIVOT);
			break;
		}

		switch(nextManeuver->direction) {
		case FORWARDS:
			thisPacket->nextDirection = htonl(DIR_FORWARD);
			break;
		case BACKWARDS:
			thisPacket->nextDirection = htonl(DIR_BACKWARD);
			break;
		case LEFT:
			thisPacket->nextDirection = htonl(DIR_LEFT);
			break;
		case RIGHT:
			thisPacket->nextDirection = htonl(DIR_RIGHT);
			break;
		case FORWARDS_LEFT:
			thisPacket->nextDirection = htonl(DIR_FORWARD_LEFT);
			break;
		case FORWARDS_RIGHT:
			thisPacket->nextDirection = htonl(DIR_FORWARD_RIGHT);			
			break;
		case BACKWARDS_LEFT:
			thisPacket->nextDirection = htonl(DIR_BACKWARD_LEFT);
			break;
		case BACKWARDS_RIGHT:
			thisPacket->nextManeuver = htonl(DIR_BACKWARD_RIGHT);
			break;
		}	

		thisPacket->nextDegrees = htonl((int)(nextManeuver->degrees * FLOAT_SCALE_FACTOR));
		thisPacket->nextSpeed = htonl((int)(nextManeuver->speed * FLOAT_SCALE_FACTOR));	
		thisPacket->nextDistance = htonl((int)(nextManeuver->distance * FLOAT_SCALE_FACTOR));
		thisPacket->nextRadius = htonl((int)(nextManeuver->radius * FLOAT_SCALE_FACTOR));

		printf("RaptorRemoteSession:: Sending feedback message\n");
		printf("RaptorRemoteSession:: Current Maneuver complete in %d MS:\n", msToCompletion);
		printManeuver(curManeuver);
		printManeuver(nextManeuver);

		write(this->feedbackSocketFD, this->feedbackSendBuffer, sizeof(FeedbackPacket));
	}

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
    if (pthread_create(&this->listeningThread, NULL, startListeningThread, (void*)this) == 0 && pthread_create(&this->feedbackThread, NULL, startFeedbackThread, (void*)this) == 0 ) { 
        
        behaviorQueue.run();
        
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


	VideoInitPacket* vidInitPacket;
	ManeuverPacket* manPacket;
	ScriptPacket* scriptPacket;

    // switch on the message type received
    switch (this->receiveBuffer[0]) {
    case VID_START:
		printf("RaptorRemoteSession::Vid Init message received\n");

		bytesRead = readFully(this->socketFD, this->receiveBuffer, sizeof(AbstractPacket), sizeof(VideoInitPacket) - sizeof(AbstractPacket));

		vidInitPacket = (VideoInitPacket*)this->receiveBuffer;

		vidInitPacket->remoteDataPort = ntohl(vidInitPacket->remoteDataPort);
		vidInitPacket->camNum = ntohl(vidInitPacket->camNum);
		vidInitPacket->targetFPS = ntohl(vidInitPacket->targetFPS);
		vidInitPacket->remoteIPLen = ntohl(vidInitPacket->remoteIPLen);

		char remoteIP[128];
		bzero (remoteIP, 128);
		memcpy(remoteIP, vidInitPacket->remoteIP, vidInitPacket->remoteIPLen);


		// if we can construct a new video stream
		if (!this->videoStreamer || (this->videoStreamer && !this->videoStreamer->isActive())) {
			printf("RaptorRemoteSession::Initializing new video stream remoteIP:%s port:%d camNum:%d targetFPS:%d\n", remoteIP, vidInitPacket->remoteDataPort, vidInitPacket->camNum, vidInitPacket->targetFPS);
			this->videoStreamer = new VideoStreamer(remoteIP, vidInitPacket->remoteDataPort, vidInitPacket->camNum, vidInitPacket->targetFPS);
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

	case VID_STOP:
		printf("RaptorRemoteSession::Vid Stop message received\n");
		
		if (!this->videoStreamer || (this->videoStreamer && !this->videoStreamer->isActive())) {
			printf("RaptorRemoteSession::No active video stream to stop\n");	
		} else {
			videoStreamer->stop();
		}

		break;

	case QUIT_MSG:
		printf("RaptorRemoteSession::Quit Message received\n");

		teardownSession();

		return false;

		break;

	case MANEUVER_MSG:

		printf("RaptorRemoteSession:: Maneuver Message received\n");

		bytesRead = readFully(this->socketFD, this->receiveBuffer, sizeof(AbstractPacket), sizeof(ManeuverPacket) - sizeof(AbstractPacket));

		manPacket = (ManeuverPacket*) this->receiveBuffer;

		manPacket->maneuver = ntohl(manPacket->maneuver);
		manPacket->direction =  ntohl(manPacket->direction);
		manPacket->degrees = ntohl(manPacket->degrees) / FLOAT_SCALE_FACTOR;
		manPacket->speed = ntohl(manPacket->speed) / FLOAT_SCALE_FACTOR;
		manPacket->distance = ntohl(manPacket->distance) / FLOAT_SCALE_FACTOR;
		manPacket->radius = ntohl(manPacket->radius) / FLOAT_SCALE_FACTOR;
		manPacket->updateMethod = ntohl(manPacket->updateMethod);

		printf("RaptorRemoteSession:: Maneuver received \n  maneuver:%d\n  direction:%d\n  degrees:%d\n  speed:%d\n  distance:%d\n  radius:%d\n", manPacket->maneuver, manPacket->direction, manPacket->degrees, manPacket->speed, manPacket->distance, manPacket->radius);

		handleManeuverUpdate(manPacket->maneuver, manPacket->direction, manPacket->degrees, manPacket->speed, manPacket->distance, manPacket->radius, manPacket->updateMethod);
		

		break;
		
	case SCRIPT_MSG:
	
		printf("RaptorRemoteSession:: Script Message received\n");
		
		bytesRead = readFully(this->socketFD, this->receiveBuffer, sizeof(AbstractPacket), sizeof(ScriptPacket) - sizeof(AbstractPacket));
		
		scriptPacket = (ScriptPacket*) this->receiveBuffer;
		scriptPacket->scriptLen = ntohl(scriptPacket->scriptLen);
		
		printf("String size received %d\n", scriptPacket->scriptLen);
		for (int i = 0; i < scriptPacket->scriptLen; i++) {
			printf("%c", scriptPacket->script[i]);
		}
		
		char* scriptCopy = (char*) malloc(scriptPacket->scriptLen + 1);
		bzero(scriptCopy, scriptPacket->scriptLen + 1);
		memcpy(scriptCopy, scriptPacket->script, scriptPacket->scriptLen);
		//int errorLine = 0;
		int errorLine = behaviorQueue.loadFromScriptText(scriptCopy, scriptPacket->scriptLen);
		
		//printf("RaptorRemoteSession:: Return value from load script: %d\n", errorLine);
		
		//sendScriptResponseMessage(errorLine);
				
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

int RaptorRemoteSession::sendScriptResponseMessage(int errorLine) {
	pthread_mutex_lock(&sendMutex);
	
	bzero(this->sendBuffer, SEND_BUFFER_SIZE);
	
	int packetBytes = newScriptRspMsg(this->sendBuffer, errorLine);
	
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

int RaptorRemoteSession::handleManeuverUpdate(int maneuver, int direction, float degrees, float speed, float distance, float radius, int updateMethod) {
	Maneuver* newManeuver = (Maneuver*) malloc(sizeof(Maneuver));
	bzero(newManeuver, sizeof(Maneuver));
	
	switch (maneuver) {
	case MAN_NONE:
		newManeuver->maneuverType = BEHAVIOR_NONE;
		break;

	case MAN_MOVE:
		newManeuver->maneuverType = BEHAVIOR_MOVE;
		if (direction == DIR_FORWARD) {
			newManeuver->direction = FORWARDS;
		} else if (direction == DIR_BACKWARD) {
			newManeuver->direction = BACKWARDS;
		}
		newManeuver->speed = speed;
		newManeuver->distance = distance;
		break;

	case MAN_PIVOT:
		newManeuver->maneuverType = BEHAVIOR_PIVOT;
		if (direction == DIR_LEFT) {
			newManeuver->direction = LEFT;
		} else if (direction == DIR_RIGHT) {
			newManeuver->direction = RIGHT;
		}
		newManeuver->degrees = degrees;
		
		break;

	case MAN_ARC:
		newManeuver->maneuverType = BEHAVIOR_ARC;
		if (direction == DIR_FORWARD_LEFT) {
			newManeuver->direction = FORWARDS_LEFT;
		} else if (direction == DIR_FORWARD_RIGHT) {
			newManeuver->direction = FORWARDS_RIGHT;
		} else if (direction == DIR_BACKWARD_LEFT) {
			newManeuver->direction = BACKWARDS_LEFT;
		} else if (direction == DIR_BACKWARD_RIGHT) {
			newManeuver->direction = BACKWARDS_RIGHT;
		}
		newManeuver->degrees = degrees;
		newManeuver->radius = radius;
		break;
	}
	
	if (updateMethod == MAN_QUEUE_SET_NEXT) {
		behaviorQueue.setNextManeuver(newManeuver);
	} else if (updateMethod == MAN_QUEUE_ENQUEUE) {
		behaviorQueue.enqueue(newManeuver);
	}

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




