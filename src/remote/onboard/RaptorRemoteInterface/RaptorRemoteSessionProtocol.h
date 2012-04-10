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

/*
 * Protocol Version ID
 */
#define RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID 0x7D

/*
 * Message Types
 */

#define PING_MSG 'p'
#define PING_RSP_MSG 'P'
#define INIT_MSG 'i'
#define INIT_RSP_MSG 'I'
#define QUIT_MSG 'q'
#define QUIT_RSP_MSG 'Q'
#define CMD_MSG 'c'
#define VID_START 'v'

#define MAX_PACKET_BYTES 64

#define NUM_PAYLOAD_BYTES(packetDef) (sizeof(packetDef) - sizeof(AbstractPacket))



/*
 * Header format definition
 */

typedef struct AbstractPacket_t {
    char messageType;
} AbstractPacket;

typedef struct PingPacket_t {
    char messageType;
} PingPacket;

typedef struct PingRspPacket_t {
    char messageType;
} PingRspPacket;

typedef struct InitPacket_t {
    char messageType;
} InitPacket;

typedef struct InitRspPacket_t {
    char messageType;
    char success;
} InitRspPacket;

typedef struct QuitPacket_t {
    char messageType;
} QuitPacket;

typedef struct QuitRspPacket_t {
    char messageType;
    char success;
} QuitRspPacket;

typedef struct VideoInitPacket_t {
    char messageType;
    int remoteServerPort;
    int remoteDataPort;
} VideoInitPacket;

int newPingMsg(char* buff);
int newPingRspMsg(char* buff);
int newInitRspMsg(char* buff, bool success);
int newQuitMsg(char* buff);
int newQuitRspMsg(char* buff, bool success);



#endif 