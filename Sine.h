/*
 *  Sine.h
 *  
 *
 *  Created by kannolab on 12/07/13.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _Sine_h
#define _Sine_h

#include "Module.h"
#include "porttime.h"

#define SAMPLE_RATE	44100.0

typedef struct WaveInfo {
	float			*sample;
	unsigned long	packetCount;
}WaveInfo;

class Sine : public Module {

public:
	int			freq;
	float		fVal;
	float		*sample;
	double		location;
	long		packetCount;
	double		interval;
    double		sampleRate;
    unsigned	numPackets;
	float		fTable[128];
	float		*buf;
	short		*output;
	WaveInfo	pcm;
	
	Sine(lo_server_thread s, const char *osc);
	~Sine();

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
	
	void prepareAudioResources();
	void initWave();
	static void render(PtTimestamp timestamp, void *userData);
};

#endif