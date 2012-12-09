//
//  AudioClock.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/10/16.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "AudioClock.h"

void AudioClock::render(PtTimestamp timestamp, void *userData)
{
	AudioClock *ac = (AudioClock *)userData;
    ac->sendAudio(ac->output, ac->numPackets*sizeof(short));
}

int AudioClock::data1(const char   *path, 
                      const char   *types, 
                      lo_arg       **argv, 
                      int          argc,
                      void         *data, 
                      void         *user_data)
{
    AudioClock *ac = (AudioClock *)user_data;
    int s = argv[0]->i;
    
    if (s > 0) {
        if (!Pt_Started()) Pt_Start(ac->interval, ac->render, ac);
    }else {
        if (Pt_Started()) Pt_Stop();
    }
    return 0;
}

int AudioClock::data2(const char   *path, 
                      const char   *types, 
                      lo_arg       **argv, 
                      int          argc,
                      void         *data, 
                      void         *user_data)
{
    AudioClock *ac = (AudioClock *)user_data;
    int i = argv[0]->i;
    int s = argv[3]->i;

    if (Pt_Started()) Pt_Stop();
    ac->numPackets = 64 * (i/4);
    ac->interval    = (double)ac->numPackets/ac->sampleRate;
    
    if (s > 0) {
        if (!Pt_Started()) Pt_Start(ac->interval, ac->render, ac);
    }else {
        if (Pt_Started()) Pt_Stop();
    }

    return 0;
}

AudioClock::AudioClock(Server *s, const char *osc) : Module(s, osc)
{
    addMethodToServer("/Data", "ii", AudioClock::data1, this);
    addMethodToServer("/Data", "iiii", AudioClock::data2, this);

	sampleRate	= SAMPLE_RATE;
	numPackets	= 256;
	interval	= (double)numPackets/sampleRate;
    
	output		= (short *)malloc(numPackets*sizeof(short));
    memset(output, 0, numPackets*sizeof(short));
    Pt_Start(interval, this->render, this);

}

AudioClock::~AudioClock()
{
    Pt_Stop();
	free(output);
    deleteMethodFromServer("/Data", "ii");
	deleteMethodFromServer("/Data", "iiii");

}
