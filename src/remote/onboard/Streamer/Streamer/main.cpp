//
//  main.cpp
//  Streamer
//
//  Created by Gordon Towne on 3/31/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "VideoStreamer.h"

int main() {
    VideoStreamer vidStreamer("localhost", 7777, 1, 20);
    int retVal = vidStreamer.start();
    
    printf("Started busy thread with return value %d\n", retVal);
    
    while(true) {
        
    }
}



