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
#include "lo.h"

int main()
{
    DAC *dac;
	ADC *adc;
    Envelope *env;
    Coordinator *co;
	Sine *sine;
    lo_server_thread st;
    
    st = lo_server_thread_new("6340", NULL);
    dac = new DAC(st, "/DAC/1");
	//adc = new ADC(st, "/ADC/1");
    //env = new Envelope(st, "/EF/Envelope/1");
	sine = new Sine(st, "/GN/Sine/1");
    co = new Coordinator(st, "/Coordinator");
    lo_server_thread_start(st);
    
    sine->sendSetMdtkn();
	dac->sendSetMdtkn();

    getchar();
	co->connect(0, 1, "/Stream");
	getchar();

	delete dac;
	//delete adc;
    //delete env;
    delete co;
	delete sine;
	
    return 0;
}