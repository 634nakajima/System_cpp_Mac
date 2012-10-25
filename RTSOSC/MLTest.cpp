/*
 *  MLTest.cpp
 *  
 *
 *  Created by kannolab on 12/10/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */



#include <iostream>
#include "DAC.h"
#include "ADC.h"
#include "Coordinator.h"
#include "Envelope.h"
#include "Sine.h"
#include "AudioSource.h"
#include "XBeeController.h"
#include "Serial.h"
#include "ModuleList.h"
#include "ModuleController.h"
#include "lo.h"
#ifndef	_PT_
#include "porttime.h"
#define _PT_
#endif

int main()
{
	Coordinator			*co;
	ModuleController	*mc;
    XBeeController      *xbc;
	lo_server_thread	st;
	int                 mColor;
    
	st = lo_server_thread_new("6340", NULL);
    lo_server_thread_start(st);

    xbc = new XBeeController(st, "/XBC");
	mc	= new ModuleController(st, "/MC");
	co	= new Coordinator(st, "/Coordinator");
	
	mc->sendModuleList();
    xbc->co = co;
    co->xbc = xbc;
    while (1) {
        printf("Enter Module Color\n");
        scanf("%d", &mColor);
        if (!mColor) break;
        co->ml->createModule((char *)"1", mColor);
    }
	
	return 0;
}