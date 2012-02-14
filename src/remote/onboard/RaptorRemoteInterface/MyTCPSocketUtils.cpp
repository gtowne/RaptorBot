/*
 *  MyTCPSocketUtils.c
 *  RaptorRemoteInterface
 *
 *  Created by Gordon Towne on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "MyTCPSocketUtils.h"

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
int acceptConnection(int listeningSocketFD) {
	int newSocketFD;
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);
	
	newSocketFD = accept(listeningSocketFD, (struct sockaddr*)&clientAddress, &clientAddressLength);
	
	return newSocketFD;
}