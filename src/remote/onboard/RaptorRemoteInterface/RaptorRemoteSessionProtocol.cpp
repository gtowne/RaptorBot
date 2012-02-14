/*
 *  RaptorRemoteProtocol.cpp
 *  RaptorRemoteInterface
 *
 *  Created by Gordon Towne on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "RaptorRemoteSessionProtocol.h"

int writeNetworkHeader(char* buff, char messageType) {
    NetworkHeader* header = (NetworkHeader*)buff;
    
    header->protocolID = RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID;
    header->messageType = messageType;
    
    return sizeof(NetworkHeader);
}

int newPingRspMsg(char* buff) {
    return writeNetworkHeader(buff, PING_RSP_MSG);
}

int newInitRspMsg(char* buff, bool success) {
    int headerSize = writeNetworkHeader(buff, INIT_RSP_MSG);
    
    InitRspPacket* payload = (InitRspPacket*)(&buff[headerSize]);
    
    if (success) {
        payload->success = 1;
    } else {
        payload->success = 0;
    }
    
    return sizeof(NetworkHeader) + sizeof(InitRspPacket);
}

int newQuitRspMsg(char* buff, bool success) {
    int headerSize = writeNetworkHeader(buff, QUIT_RSP_MSG);
    
    QuitRspPacket* payload = (QuitRspPacket*)(&buff[headerSize]);
    
    if (success) {
        payload->success = 1;
    } else {
        payload->success = 0;
    }
    
    return sizeof(NetworkHeader) + sizeof(QuitRspPacket);
}

