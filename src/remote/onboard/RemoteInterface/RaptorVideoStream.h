//
//  RaptorVideoStream.h
//  RaptorRemoteInterface
//
//  Created by Gordon Towne on 2/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef RAPTOR_VIDEO_STREAM_H
#define RAPTOR_VIDEO_STREAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "RaptorVideoStreamProtocol.h"


#define CONTROL_SEND_BUFF_SIZE 256
#define DATA_SEND_BUFF_SIZE 256
#define CONTROL_RECV_BUFF_SIZE 256
#define DATA_RECV_BUFF_SIZE 256

class RaptorVideoStream {
private:
    int controlSocket;
    int dataSocket;
    
    char controlSendBuff[CONTROL_SEND_BUFF_SIZE];
    char controlRecvBuff[CONTROL_RECV_BUFF_SIZE];
    char dataSendBuff[DATA_SEND_BUFF_SIZE];
    char dataRecvBuff[DATA_RECV_BUFF_SIZE];
    
    sockaddr_in* remoteAddrData;
    sockaddr_in* remoteAddrControl;
    
public:
    
    RaptorVideoStream(sockaddr_in* _remoteAddr, int _remoteDataPort, int _remoteServerPort);
    ~RaptorVideoStream();
    
    int start();
    
};

#endif
