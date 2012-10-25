/*
 *  odoremain.cpp
 *  
 *
 *  Created by kannolab on 12/10/25.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "rtsosc.h"
#include "MyModule.h"

int main()
{
	MyModule    *o;
	Serial      *se;
	lo_server_thread	st;
    
	st = lo_server_thread_new("6340", NULL);
    lo_server_thread_start(st);
	
    o = new MyModule(st, "/Odore");
	se	= new Serial(st, "/Serial");

	se->connectTo(o, "/Stream");
    
	getchar();
	
	return 0;
}