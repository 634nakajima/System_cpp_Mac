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
	ModuleList			*ml;
	ModuleController	*mc;
	lo_server_thread	st;
	int                 mColor;
	st = lo_server_thread_new("6340", NULL);
	ml	= new ModuleList(st, "/ModuleList");
	mc	= new ModuleController(st, "/MC");
	co	= new Coordinator(st, "/Coordinator");
	
	lo_server_thread_start(st);
	mc->sendModuleList();
    ml->t = '7';
    while (1) {
        printf("Enter Module Color\n");
        scanf("%d", &mColor);
        if (!mColor) break;
        ml->createModule("7", mColor);
    }
	
	return 0;
}