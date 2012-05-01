//
//  VideoStreamer.cpp
//  Streamer
//
//  Created by Gordon Towne on 4/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//makefile

#include "VideoStreamer.h"
using namespace std;

VideoStreamer::VideoStreamer(char* _hostname, int _hostport, int _camNum, int _targetFPS) {
    this->hostname = _hostname;
    this->hostport = _hostport;
    this->camNum = _camNum;
    this->targetFPS = _targetFPS;
	this->keepStreaming = false;
}

void* busyLoop(void* arg);

int VideoStreamer::start() {
    
    if (pthread_create(&busyLoopThread, NULL, busyLoop, (void*) this) == 0) {
		return 1;
    }
    
    
    return  0;
}

bool VideoStreamer::stop() {
	keepStreaming = false;
	return true;
}

bool VideoStreamer::isActive() {
	return keepStreaming;
}


/*
 * Writes an appropriately formatted header for the video data passed
 * in in the 'data' buffer and returns the number of header+payload
 * bytes written to the buffer. Assumes that payload data is already
 * in network byte order if necessary
 */
int newVidDataPacket(char* buff, char* data, int dataLen, int frameNum, int imWidth, int imHeight) {
    VidDataPacket* packet = (VidDataPacket*) buff;
    packet->messageType = htonl(VID_DATA_MSG);
    packet->dataLength = htonl(dataLen);
    packet->frameNum = htonl(frameNum);
    packet->imWidth = htonl(imWidth);
    packet->imHeight = htonl(imHeight);
    
    memcpy(&buff[sizeof(VidDataPacket)], data, dataLen);
    
    return sizeof(VidDataPacket) + dataLen;
}

int cvMatDataToBuff(cv::Mat* _mat, char* buff) {
    int matType = _mat->type();
    
    cv::Mat* mat = _mat;
    if (!matType == CV_8UC3) {
        printf("Input matrix is not 8UC3, converting...");
        mat = new cv::Mat();
        _mat->convertTo(*mat, CV_8UC3);
    }
    
    matType = mat->type();
    cv::Size matSize = mat->size();
    size_t pixelBytes = mat->elemSize();
    uchar* matData = mat->data;
    
    size_t bytesToCopy = pixelBytes * matSize.height * matSize.width;
    
    for (int i = 0; i < bytesToCopy; i++) {
        buff[i] = matData[i];
    }
    
    return bytesToCopy;
}

int cvImageDataToBuff(vector<uchar>& data, char* buff) {
    size_t len = data.size();
    for (int i = 0; i < len; i++) {
        buff[i] = data.at(i);
    }
    
    return (int)len;
}

int sendBigPacket(int socketFD, char* buff, int len, int maxPacketLen, int seqNo) {
    int numFrags = (int)ceil((double)len / (double)maxPacketLen);
    
    char sendBuff[sizeof(BigPacket) + maxPacketLen];
    //char recvBuff[];
    
    BigPacket* header = (BigPacket*) sendBuff;
    header->messageType = htonl(BIG_PACKET_MSG);
    header->seqNo = htonl(seqNo);
    header->totalPacketBytes = htonl(len);
    header->numFrags = htonl(numFrags);
    
    char* payloadPtr = &sendBuff[sizeof(BigPacket)];
    int payloadBytesRemaining = len;
    int offset = 0;
    int bytesToSendNow;
    
    for (int curFragNum = 1; curFragNum <= numFrags; curFragNum++) {
        header->fragNo = htonl(curFragNum);
        header->offset = htonl(offset);
        
        bytesToSendNow = payloadBytesRemaining < maxPacketLen ? payloadBytesRemaining : maxPacketLen;
        
        header->fragBytes = htonl(bytesToSendNow);
        
        memcpy(payloadPtr, &buff[offset], bytesToSendNow);
        
        usleep(50);
        
        send(socketFD, sendBuff, bytesToSendNow, 0);
        
        offset += bytesToSendNow;
        payloadBytesRemaining -= bytesToSendNow;
        
        
    }
    
    return 0;
}

void* busyLoop (void* arg) {
	printf("VideoStreamer::Started feed thread\n");

    VideoStreamer* _this = (VideoStreamer*) arg;
    
    cv::VideoCapture cap;
    bool openedSuccessfully = cap.open(_this->camNum);
    
    cv::Mat frame;
    cv::Mat scaledFrame;
    
    
    int socketFD = openUDPSocket(_this->hostname, _this->hostport);
    
    
    //setsockopt(socketFD, IPPROTO_IP, IP_, &val, sizeof(val));
    
    printf("My socket fd is %d\n", socketFD);
    
    int sendBuffLen = 640 * 480 * 5;
    char* sendBuff = (char*)malloc(sendBuffLen);
    char* vidDataBuff = (char*)malloc(sendBuffLen);
    cv::Size frameSize;
    cv::Size scaledSize(320,240);
    
    vector<uchar> jpgBuffer;
    //int _args[] = {CV_IMWRITE_PNG_COMPRESSION, 8};
    //vector<int> args(_args, _args + sizeof(_args);
    
    
    time_t frameGrabTime, loopEndTime;

	_this->keepStreaming = true;
    
    int i = 0;
    while(_this->keepStreaming) {
        frameGrabTime = time(NULL);
        cap >> frame;

		resize(frame, scaledFrame, scaledSize, 0, 0, cv::INTER_NEAREST);
	
        frameSize = frame.size();
        
        cv::imencode(".bmp", scaledFrame, jpgBuffer);
        
        //int vidDataLen = cvMatDataToBuff(&frame, vidDataBuff); 
        
        int vidDataLen = cvImageDataToBuff(jpgBuffer, vidDataBuff);
        
        int bytesWritten = newVidDataPacket(sendBuff, vidDataBuff, vidDataLen, i, scaledSize.width, scaledSize.height);
        
        //for (int j = 0; j < 100; j++) {
        //    printf(" %u\n", vidDataBuff[j]);
        //}
        
        sendBigPacket(socketFD, sendBuff, bytesWritten, 1400, i);
        
        bzero(sendBuff, sendBuffLen);
        loopEndTime = time(NULL);
        
        double targetDelaySeconds = 1.0 / _this->targetFPS;
        double secsForLoop = difftime(loopEndTime, frameGrabTime);
        double secsToWait = targetDelaySeconds - secsForLoop;
        
        if (secsToWait > 0) {
            int msToWait = (int)(secsToWait * 1000);
            //printf("Sleeping for %d MS\n", msToWait);
            usleep(msToWait);
        }
        i++;
    }

	frame.release();
	scaledFrame.release();
	cap.release();
    
    return 0;
    
}
