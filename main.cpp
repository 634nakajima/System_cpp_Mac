//  main.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/05/28.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "DAC.h"
#include "ADC.h"
#include "Coordinator.h"
#include "XBeeController.h"
#include "Envelope.h"
#include "Sine.h"
#include "AudioSource.h"
#include "AudioClock.h"
#include "lo.h"

int main()
{
    AudioClock *ac;
    DAC *dac;
	ADC *adc;
    Envelope *env;
    Coordinator *co;
	Sine *sine1,*sine2,*sine3;
	AudioSource *a1, *a2, *a3;
    lo_server_thread st;
	
    st = lo_server_thread_new("6340", NULL);
    ac = new AudioClock(st, "/GN/AudioClock");
    dac = new DAC(st, "/DAC/1");
	//adc = new ADC(st, "/ADC/1");
    env = new Envelope(st, "/EF/Envelope/1");
	sine1 = new Sine(st, "/GN/Sine/1");
    sine2 = new Sine(st, "/GN/Sine/2");
	sine3 = new Sine(st, "/GN/Sine/3");

    //co = new Coordinator(st, "/Coordinator");
	a1 = new AudioSource(st, "/GN/AudioSource/1");
	a2 = new AudioSource(st, "/GN/AudioSource/2");
	a3 = new AudioSource(st, "/GN/AudioSource/3");

	a1->prepareAudioResources("sound_c.wav");
	a2->prepareAudioResources("1_1_0_Intro_l_4.wav");
	a3->prepareAudioResources("digital.wav");

    a2->isPlaying = true;
    a2->isLooping = true;
    
    sine1->freq = 45;
    sine2->freq = 61;
    sine3->freq = 52;

    lo_server_thread_start(st);

    ac->connectTo(sine1, "/Stream");
    sine1->connectTo(sine2, "/Stream");
    sine2->connectTo(sine3, "/Stream");
    sine3->connectTo(a2, "/Stream");
    a2->connectTo(dac, "/Stream");

	getchar();

    delete ac;
	delete dac;
	//delete adc;
    delete env;
    //delete co;
	delete a1;
	delete a2;
	delete a3;
	delete sine1;
    delete sine2;
	delete sine3;

 
    return 0;
}