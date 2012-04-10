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
    packet->messageType = PING_MSG;
    return sizeof(PingPacket);
}

int newPingRspMsg(char* buff) {
    PingRspPacket* packet = (PingRspPacket*) buff;
    packet->messageType = PING_RSP_MSG;
    return sizeof(PingRspPacket);
}

int newInitRspMsg(char* buff, bool success) {
    InitRspPacket* packet = (InitRspPacket*) buff;
    packet->messageType = INIT_RSP_MSG;
    
    if (success) {
        packet->success = 1;
    } else {
        packet-> success = 0;
    }
    
    return sizeof(InitRspPacket);
}

int newQuitMsg(char* buff) {
    QuitPacket* packet = (QuitPacket*) buff;
    packet->messageType = QUIT_MSG;
    return sizeof(QuitPacket);
}

int newQuitRspMsg(char* buff, bool success) {
    QuitRspPacket* packet = (QuitRspPacket*) buff;
    packet->messageType = QUIT_RSP_MSG;
    
    if (success) {
        packet->success = 1;
    } else {
        packet-> success = 0;
    }
    
    return sizeof(QuitRspPacket);
}

