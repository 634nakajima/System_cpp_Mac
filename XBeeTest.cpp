/*
 *  XBeeTest.cpp
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
#include "lo.h"
#ifndef	_PT_
#include "porttime.h"
#define _PT_
#endif

int main()
{
	XBeeController *xbc;
	Coordinator	*co;
	lo_server_thread st;
	
	st = lo_server_thread_new("6340", NULL);
	xbc = new XBeeController(st, "/XBC");
	co = new Coordinator(st, "/Coordinator");

	co->xbc = xbc;
	xbc->co = co;
	
	lo_server_thread_start(st);
	
	getchar();
	
	return 0;
}