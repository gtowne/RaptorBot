//
//  RaptorVideoStreamProtocol.cpp
//  RaptorRemoteInterface
//
//  Created by Gordon Towne on 2/16/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "RaptorVideoStreamProtocol.h"

int newInitRsp(char* buff, int myDataPort) {
    InitRsp* message = (InitRsp*) buff;
    
    message->messageType = INIT_RSP;
    message->myDataPort = myDataPort;
    
    return sizeof(InitRsp);
}

char readFromDataSocket(int socket, char* buff) {
    return 0;
}

char readFromControlSocket(int socket, char* buff) {
    int bytesRead = read(socket, buff, 1);
    
    if (bytesRead != 1) {
        return -1;
    }
    
    char messageType = buff[0];
    
    switch (messageType) {
        case INIT_MSG:
            
            bytesRead += read(socket, &(buff[1]), sizeof(InitMsg) - bytesRead);
            
            if (bytesRead != sizeof(InitMsg)) {
                return -1;
            }
            
            return INIT_MSG;
            
            break;
            
        default:
            break;
    }
    
    return -1;
}