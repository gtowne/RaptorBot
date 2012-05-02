/*
 *  RaptorRemoteProtocol.h
 *  RaptorRemoteInterface
 *
 *  Created by Gordon Towne on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RAPTOR_REMOTE_SESSION_PROTOCOL_H
#define RAPTOR_REMOTE_SESSION_PROTOCOL_H

#include <stdint.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

/*
 * Message Types
 */

#define PING_MSG 0x01
#define PING_RSP_MSG 0x02
#define INIT_SESSION_MSG 0x03
#define INIT_SESSION_RSP_MSG 0x04
#define QUIT_MSG 0x05
#define VID_START 0x07
#define VID_START_RSP 0x08
#define VID_STOP 0x09
#define FEEDBACK_MSG 0x0A
#define MANEUVER_MSG 0x0B
#define SCRIPT_MSG 0x0C
#define SCRIPT_RSP_MSG 0x0D

/*
 * Maneuver Types
 */

#define MAN_NONE 0x01
#define MAN_MOVE 0x02
#define MAN_PIVOT 0x03
#define MAN_ARC 0x04

/*
 * Maneuver Parameters
 */

/*
 *  --- Direction
 */
#define DIR_FORWARD 0x01
#define DIR_BACKWARD 0x02
#define DIR_LEFT 0x03
#define DIR_RIGHT 0x04
#define DIR_FORWARD_LEFT 0x05
#define DIR_FORWARD_RIGHT 0x06
#define DIR_BACKWARD_LEFT 0x07
#define DIR_BACKWARD_RIGHT 0x08

#define MAN_QUEUE_SET_NEXT 0x01
#define MAN_QUEUE_ENQUEUE 0x02

#define FLOAT_SCALE_FACTOR 10000.0

#define NUM_PAYLOAD_BYTES(packetDef) (sizeof(packetDef) - sizeof(AbstractPacket))



/*
 * Header format definclean:
        rm -f *.oition
 */

typedef struct AbstractPacket_t {
    int32_t messageType;
} AbstractPacket;

typedef struct PingPacket_t {
    int32_t messageType;
} PingPacket;

typedef struct PingRspPacket_t {
    int32_t messageType;
    int32_t machineNameLen;
    char machineName[128];
} PingRspPacket;

typedef struct InitPacket_t {
    int32_t messageType;
	int32_t remoteFeedbackServerPort;
	int32_t remoteIPLen;
	char remoteIP[128];
} InitPacket;

typedef struct InitRspPacket_t {
    int32_t messageType;
    int32_t success;
} InitRspPacket;

typedef struct QuitPacket_t {
    int32_t messageType;
} QuitPacket;

typedef struct FeedbackPacket_t {
	int32_t messageType;
	int32_t currentManeuver;
	int32_t currentDirection;
	int32_t currentDegrees;
	int32_t currentSpeed;
	int32_t currentDistance;
	int32_t currentRadius;
	int32_t msToNext;
	int32_t nextManeuver;
	int32_t nextDirection;
	int32_t nextDegrees;
	int32_t nextSpeed;
	int32_t nextDistance;
	int32_t nextRadius;
} FeedbackPacket;

typedef struct VideoInitPacket_t {
    int32_t messageType;
    int32_t remoteDataPort;
	int32_t camNum;
	int32_t targetFPS;
	int32_t remoteIPLen;
	char remoteIP[128];
} VideoInitPacket;

typedef struct VideoInitRspPacket_t {
	int32_t messageType;
	int32_t success;
} VideoInitRspPacket;

typedef struct VideoStopPacket_t {
	int32_t messageType;
} VideoStopPacket;

typedef struct ManeuverPacket_t {
	int32_t messageType;
	int32_t maneuver;
	int32_t direction;
	int32_t degrees;
	int32_t speed;
	int32_t distance;
	int32_t radius;
	int32_t updateMethod;
} ManeuverPacket;

typedef struct ScriptPacket_t {
	int32_t messageType;
	int32_t scriptLen;
	char script[2048];
} ScriptPacket;

typedef struct ScriptRspPacket_t {
	int32_t messageType;
	int32_t errorLine;
} ScriptRspPacket;

int newPingMsg(char* buff);
int newPingRspMsg(char* buff, char* machineName, int machineNameLen);
int newInitRspMsg(char* buff, bool success);
int newQuitMsg(char* buff);
int newQuitRspMsg(char* buff, bool success);
int newVidInitRspMsg(char* buff, bool success);
int newFeedbackMsg(char* buff);
int newScriptRspMsg(char* buff, int errorLine);


#endif 
