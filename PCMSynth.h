/*
 *  PCMSynth.h
 *  
 *
 *  Created by kannolab on 12/08/03.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PCMSynth_h
#define _PCMSynth_h

#include "Module.h"
#include "portsf.h"
#include "AudioSource.h"
#include <map>

#ifndef	_PT_
#include "porttime.h"
#define _PT_
#endif

class PCMSynth : public Module {
	
public:
    std::map<int, AudioSource*>   sources;
	
	PCMSynth(lo_server_thread s, const char *osc);
	~PCMSynth();
    
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
	
	static int data3(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data);
	
	int			prepareAudioResources(const char *sound);
	void		initAudioInfo();
	static void render(PtTimestamp timestamp, void *userData);
};

#endif