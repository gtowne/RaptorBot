/*
 *  RaptorRemoteProtocol.h
 *  RaptorRemoteInterface
 *
 *  Created by Gordon Towne on 1/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RAPTOR_REMOTE_SESSION_PROTOCOL_H
#define RAPTOR_REMOTE_SESSION_PROTOCOL_H

/*
 * Protocol Version ID
 */
#define RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID 0x7D

/*
 * Message Types
 */

#define PING_MSG 'p'
#define PING_RSP_MSG 'P'
#define INIT_MSG 'i'
#define INIT_RSP_MSG 'I'
#define QUIT_MSG 'q'
#define QUIT_RSP_MSG 'Q'
#define CMD_MSG 'c'
#define VID_MSG 'v'

/*
 * Header format definition
 */
typedef struct NetworkHeader_t {
	char protocolID;
	char messageType;
} NetworkHeader;



#endif 