/*
 *  MySocketUtils.c
 *  RaptorRemoteInterface
 *
 *  Created by Gordon Towne on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "MySocketUtils.h"

/*
 * openListeningSocket() - take a port number,
 *    and open a server socket on that port, return
 *    the file descriptor to the new server socket on
 *    success, or a negative value on failure
 */
int openListeningSocket(int listeningPort) {
	int sockfd;
	struct sockaddr_in serv_addr;
	
	if (listeningPort < 0 ) {
		return -1;
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd < 0) {
        return -2;
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(listeningPort);
	
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
			 sizeof(serv_addr)) < 0) {
		return -3;
	}
	
	listen(sockfd,5);
	
	return sockfd;
}

/*
 * acceptConnection() - take a server socket file
 *    descriptor, block until we receive a new
 *    incoming connection, then return the file
 *    descriptor to the new socket
 */
int acceptConnection(int listeningSocketFD, struct sockaddr_in* hostAddr) {
	int newSocketFD;
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);
	
	newSocketFD = accept(listeningSocketFD, (struct sockaddr*)&clientAddress, &clientAddressLength);
	
    memcpy(hostAddr, &clientAddress, sizeof(sockaddr_in));
    
	return newSocketFD;
}

int openUDPSocket(sockaddr_in* remoteAddr, int* localPort) {
    
    int myFD = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (myFD < 0) {
        return myFD;
    }
    
    
    connect(myFD, (sockaddr*)remoteAddr, sizeof(sockaddr_in));
    
    int localPortNumber = -1;
    sockaddr_in localAddr;
    socklen_t addrlen = sizeof(localAddr);
    if(getsockname(myFD, (sockaddr*)&localAddr, &addrlen) == 0 &&
       localAddr.sin_family == AF_INET &&
       addrlen == sizeof(localAddr)) {
        localPortNumber = ntohs(localAddr.sin_port);
    }
    
    *localPort = localPortNumber;
    
    return myFD;
}

int openUDPSocket(char* remoteAddr, int remotePort) {
    int myFD = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (myFD < 0) {
        return myFD;
    }
    
    struct hostent *server = gethostbyname(remoteAddr);
    
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    
    char buffer[256];
    struct sockaddr_in serv_addr;
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(remotePort);
    
    if (connect(myFD,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        return -1;
    }
    
    return myFD;
    
}

/*
 * readFully - Keep reading from a socket into a buffer until we
 *    get all the data we want or there's an error
 */
int readFully(int socketFD, char* buff, int offset, int numBytes) {
    int totalBytesRead = 0;
    int bytesJustRead = 0;
    
    while (totalBytesRead < numBytes) {
        bytesJustRead = recv(socketFD, &(buff[offset + totalBytesRead]), numBytes - totalBytesRead, 0);
        
        if (bytesJustRead == 0) {
            return totalBytesRead;
        } else if (bytesJustRead < 1) {
            if (totalBytesRead > 0) {
                return totalBytesRead;
            }
            
            return bytesJustRead;
        }
        
        totalBytesRead += bytesJustRead;
    }
    
    return totalBytesRead;
}

