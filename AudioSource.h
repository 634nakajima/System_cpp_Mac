/*
 *  AudioSource.h
 *  
 *
 *  Created by kannolab on 12/07/16.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _AudioSource_h
#define _AudioSource_h

#include "Module.h"
#include "portsf.h"

#ifndef	_PT_
#include "porttime.h"
#define _PT_
#endif

class AudioSource : public Module {
	
public:
	float		rate;
	float		*sample;
	double		location;
	long		packetCount;
	double		interval;
    double		sampleRate;
    unsigned	numPackets;
	float		rTable[128];
	float		*buf;
	short		*output;
    int         ifd;
	bool		isPlaying;
	
	AudioSource(lo_server_thread s, const char *osc);
	~AudioSource();
	
private:
    
    static int stream(const char   *path, 
                      const char   *types, 
                      lo_arg       **argv, 
                      int          argc,
                      void         *data, 
                      void         *user_data);
    
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
	
	int			prepareAudioResources();
	void		initAudioInfo();
	static void render(PtTimestamp timestamp, void *userData);
};

#endif