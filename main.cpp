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
#include "lo.h"

static void error(int num, const char *msg, const char *path) {
    printf("lo_server_thread err\n");
}

int main()
{
    DAC *dac;
	ADC *adc;
    Envelope *env;
    Coordinator *co;
    lo_server_thread st;
    
    st = lo_server_thread_new("6340", error);
    dac = new DAC(st, "/DAC/1");
	adc = new ADC(st, "/ADC/1");
    env = new Envelope(st, "/EF/Envelope/1");
    co = new Coordinator(st, "/Coordinator");
    lo_server_thread_start(st);
    
    env->sendSetMdtkn();
    co->connect(7, 2, "/Stream");
    getchar();
	delete dac;
	delete adc;
    //delete co;
    return 0;
}