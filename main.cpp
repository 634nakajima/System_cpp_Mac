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
#include "Envelope.h"
#include "Sine.h"
#include "AudioSource.h"
#include "lo.h"
#ifndef	_PT_
#include "porttime.h"
#define _PT_
#endif

int main()
{
    DAC *dac;
	ADC *adc;
    Envelope *env;
    Coordinator *co;
	Sine *sine;
	AudioSource *a1, *a2, *a3;
    lo_server_thread st;

	double interval	= (double)256.0/44100.0;
	
    st = lo_server_thread_new("6340", NULL);
    dac = new DAC(st, "/DAC/1");
	//adc = new ADC(st, "/ADC/1");
    env = new Envelope(st, "/EF/Envelope/1");
	sine = new Sine(st, "/GN/Sine/1");
    //co = new Coordinator(st, "/Coordinator");
	a1 = new AudioSource(st, "/GN/AudioSource/1");
	a2 = new AudioSource(st, "/GN/AudioSource/2");
	a3 = new AudioSource(st, "/GN/AudioSource/3");


	
	a1->prepareAudioResources("sound_c.wav");
	a2->prepareAudioResources("1_1_0_Intro_l_4.wav");
	a3->prepareAudioResources("digital.wav");


    lo_server_thread_start(st);
    
	Pt_Start(interval, sine->render, sine);
	a1->sendSetMdtkn();
	a2->sendSetMdtkn();
	a3->sendSetMdtkn();
    sine->sendSetMdtkn();
	env->sendSetMdtkn();
	dac->sendSetMdtkn();

    /*getchar();
	co->connect(1, 0, "/Stream");
	co->connect(0, 3, "/Stream");
	co->connect(4, 0, "/Data");
	co->connect(5, 1, "/Data");
	 */

	getchar();
	Pt_Stop();

	delete dac;
	//delete adc;
    delete env;
    //delete co;
	delete a1;
	delete a2;
	delete a3;
	delete sine;
	
    return 0;
}