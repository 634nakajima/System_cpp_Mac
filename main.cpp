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

int main()
{
    DAC *dac;
	ADC *adc;
    Envelope *env;
    Coordinator *co;
	//Sine *sine;
	AudioSource *audio;
    lo_server_thread st;
    
    st = lo_server_thread_new("6340", NULL);
    dac = new DAC(st, "/DAC/1");
	//adc = new ADC(st, "/ADC/1");
    //env = new Envelope(st, "/EF/Envelope/1");
	//sine = new Sine(st, "/GN/Sine/1");
    co = new Coordinator(st, "/Coordinator");
	audio = new AudioSource(st, "/GN/AudioSource");
    lo_server_thread_start(st);
    
	audio->sendSetMdtkn();
    //sine->sendSetMdtkn();
	dac->sendSetMdtkn();

    getchar();
	co->connect(0, 1, "/Stream");
	co->connect(3, 4, "/Data");
	co->connect(4, 0, "/Data");


	getchar();

	delete dac;
	//delete adc;
    //delete env;
    delete co;
	delete audio;
	
    return 0;
}