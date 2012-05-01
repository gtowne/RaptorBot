/*
 *  MySocketUtils.h
 *  RaptorRemoteInterface
 *
 *  Created by Gordon Towne on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MY_SOCKET_UTILS_H
#define MY_SOCKET_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

/*
 * openListeningSocket() - take a port number,
 *    and open a server socket on that port, return
 *    the file descriptor to the new server socket on
 *    success, or a negative value on failure
 */
int openListeningSocket(int listeningPort);


/*
 * acceptConnection() - take a server socket file
 *    descriptor, block until we receive a new
 *    incoming connection, then return the file
 *    descriptor to the new socket
 */
int acceptConnection(int listeningSocketFD, struct sockaddr_in* hostAddr);

int openUDPSocket(sockaddr_in* remoteAddr, int localPort);

int openUDPSocket(char* remoteAddr, int remotePort);

int readFully(int socketFD, char* buff, int offset, int numBytes);

#endif
