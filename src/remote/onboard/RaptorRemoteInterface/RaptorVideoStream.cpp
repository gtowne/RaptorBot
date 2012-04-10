//
//  RaptorVideoStream.cpp
//  RaptorRemoteInterface
//
//  Created by Gordon Towne on 2/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "RaptorVideoStream.h"


RaptorVideoStream::RaptorVideoStream(sockaddr_in* _remoteAddr, int _remoteDataPort, int _remoteServerPort) {
    this->remoteAddrData = (sockaddr_in*)malloc(sizeof(sockaddr_in));
    this->remoteAddrControl = (sockaddr_in*)malloc(sizeof(sockaddr_in));
    
    memcpy(remoteAddrData, _remoteAddr, sizeof(sockaddr_in));
    memcpy(remoteAddrControl, _remoteAddr, sizeof(sockaddr_in));
    
    remoteAddrData->sin_port = htons(_remoteDataPort);
    remoteAddrControl->sin_port = htons(_remoteServerPort);
}

RaptorVideoStream::~RaptorVideoStream() {
    free(this->remoteAddrData);
    free(this->remoteAddrControl);
}

int RaptorVideoStream::start() {
    
    
    
    return 0;
}