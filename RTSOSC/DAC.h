//
//  DAC.h
//  
//
//  Created by NAKAJIMA Musashi on 12/05/27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _DAC_h
#define _DAC_h

#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include "Module.h"

#define PA_SAMPLE_TYPE      paInt16
#define FRAMES_PER_BUFFER   (64*2)
#define MAX_PACKET          44100

class DAC : public Module
{
public:
    PaStreamParameters inputParameters, outputParameters;
    PaStream *paStream;
    PaError err;
    
    bool            isPlaying;
    double          sampleRate;
    short           *buf;
    unsigned long   bs,rp,wp,loop;
    float           vol, vTable[128];
    int             vVal;
    
    DAC(lo_server_thread s, const char *osc);
    ~DAC();
    
private:
    int preparePa();
    int start();
    int stop();
    
    static int outputCallback(const void            *inputBuffer,
							  void                  *outputBuffer,
							  unsigned long         framesPerBuffer,
							  const                 PaStreamCallbackTimeInfo* timeInfo,
							  PaStreamCallbackFlags statusFlags,
							  void                  *userData);
    
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
	
	
};


#endif
