/*
 *  RaptorRemoteProtocol.cpp
 *  RaptorRemoteInterface
 *
 *  Created by Gordon Towne on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "RaptorRemoteSessionProtocol.h"

int newPingMsg(char* buff) {
    PingPacket* packet = (PingPacket*)buff;
    packet->messageType = htonl(PING_MSG);
    return sizeof(PingPacket);
}

int newPingRspMsg(char* buff, char* machineName, int machineNameLen) {
    PingRspPacket* packet = (PingRspPacket*) buff;
    packet->messageType = htonl(PING_RSP_MSG);
    packet->machineNameLen = htonl(machineNameLen);
    memcpy(packet->machineName, machineName, machineNameLen);
    return sizeof(PingRspPacket);
}

int newInitRspMsg(char* buff, bool success) {
    InitRspPacket* packet = (InitRspPacket*) buff;
    packet->messageType = htonl(INIT_SESSION_RSP_MSG);
    
    if (success) {
        packet->success = htonl(1);
    } else {
        packet-> success = htonl(0);
    }
    
    return sizeof(InitRspPacket);
}

int newQuitMsg(char* buff) {
    QuitPacket* packet = (QuitPacket*) buff;
    packet->messageType = htonl(QUIT_MSG);
    return sizeof(QuitPacket);
}

int newQuitRspMsg(char* buff, bool success) {
    QuitRspPacket* packet = (QuitRspPacket*) buff;
    packet->messageType = htonl(QUIT_RSP_MSG);
    
    if (success) {
        packet->success = htonl(1);
    } else {
        packet-> success = htonl(0);
    }
    
    return sizeof(QuitRspPacket);
}

int newVidInitRspMsg(char* buff, bool success) {
	VideoInitRspPacket* packet = (VideoInitRspPacket*) buff;

	packet->messageType = htonl(VID_START_RSP);
    
    if (success) {
        packet->success = htonl(1);
    } else {
        packet-> success = htonl(0);
    }

	return sizeof(VideoInitRspPacket);
}

