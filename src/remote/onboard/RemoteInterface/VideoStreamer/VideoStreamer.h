//
//  VideoStreamer.h
//  Streamer
//
//  Created by Gordon Towne on 4/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Streamer_VideoStreamer_h
#define Streamer_VideoStreamer_h

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "../utils/MySocketUtils.h"

#define VID_DATA_MSG 0xABC123

typedef struct VidDataPacket_t {
    int32_t messageType;
    int32_t dataLength;
    int32_t frameNum;
    int32_t imWidth;
    int32_t imHeight;
} VidDataPacket;

#define BIG_PACKET_MSG 8
#define DATA_RECEIVE_BUFF_SIZE 256
typedef struct BigPacket_t {
    int32_t messageType;
    int32_t seqNo;
    int32_t totalPacketBytes;
    int32_t numFrags;
    int32_t offset;
    int32_t fragBytes;
    int32_t fragNo;
} BigPacket;

typedef struct AckPacket_t {
    int32_t messageType;
} AckPacket;


class VideoStreamer {
public:
    VideoStreamer(char* _hostname, int _hostport, int _camNum, int _targetFPS);
    
    pthread_t busyLoopThread;
    
    // start streaming, returns 1 if successful, 0 if error
    int start();
	bool isActive();
	bool stop();

    int targetFPS;
    int camNum;
    char* hostname;
    int hostport;
	bool keepStreaming;

	


};

#endif
