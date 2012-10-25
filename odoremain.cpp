/*
 *  odoremain.cpp
 *  
 *
 *  Created by kannolab on 12/10/25.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include "DAC.h"
#include "AudioSource.h"
#include "AudioClock.h"
#include "Serial.h"
#include "odore.h"
#include "lo.h"
#ifndef	_PT_
#include "porttime.h"
#define _PT_
#endif

int main()
{
	Odore	*o;
	Serial	*se;
	lo_server_thread	st;
    
	st = lo_server_thread_new("6340", NULL);
    lo_server_thread_start(st);
	
    o = new Odore(st, "/Odore");
	se	= new Serial(st, "/Serial");

	se->connectTo(o, "/Stream");
    
	getchar();
	
	return 0;
}