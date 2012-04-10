//
//  RaptorVideoStreamProtocol.h
//  RaptorRemoteInterface
//
//  Created by Gordon Towne on 2/16/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef RAPTOR_VIDEO_STREAM_PROTOCOL_H
#define RAPTOR_VIDEO_STREAM_PROTOCOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

/*
 * Message types
 */

#define INIT_MSG 'i'
#define INIT_RSP 'I'

typedef struct InitMsg_t {
    char messageType;
    int serverSocketPort;
    int remoteDataPort;
} InitMsg;

typedef struct InitRsp_t {
    char messageType;
    int myDataPort;
} InitRsp;

int newInitRsp(char* buff, int myDataPort);

char readFromDataSocket(int socket, char* buff);

char readFromControlSocket(int socket, char* buff);


#endif