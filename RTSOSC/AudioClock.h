//
//  AudioClock.h
//  
//
//  Created by NAKAJIMA Musashi on 12/10/16.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _AudioClock_h
#define _AudioClock_h

#include "Module.h"
#ifndef	_PT_
#include "porttime.h"
#define _PT_
#endif

class AudioClock : public Module {
    
public:
    double		interval;
    double		sampleRate;
    unsigned	numPackets;
	short		*output;
	
	AudioClock(Server *s, const char *osc);
	~AudioClock();
    
    static int data1(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data);
    
    static int data2(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data);

	static void render(PtTimestamp timestamp, void *userData);
    
};

#endif