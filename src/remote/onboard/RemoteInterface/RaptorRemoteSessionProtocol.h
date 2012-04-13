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
#define QUIT_RSP_MSG 0x06
#define VID_START 0x07
#define VID_START_RSP 0x08

#define MAX_PACKET_BYTES 256

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
} InitPacket;

typedef struct InitRspPacket_t {
    int32_t messageType;
    int32_t success;
} InitRspPacket;

typedef struct QuitPacket_t {
    char messageType;
} QuitPacket;

typedef struct QuitRspPacket_t {
    int32_t messageType;
    int32_t success;
} QuitRspPacket;

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

int newPingMsg(char* buff);
int newPingRspMsg(char* buff, char* machineName, int machineNameLen);
int newInitRspMsg(char* buff, bool success);
int newQuitMsg(char* buff);
int newQuitRspMsg(char* buff, bool success);
int newVidInitRspMsg(char* buff, bool success);



#endif 
