/*
 *  odore.h
 *  
 *
 *  Created by kannolab on 12/10/25.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _Odore_h
#define _Odore_h

#include "Module.h"
#include "AudioSource.h"
#include "AudioClock.h"
#include "DAC.h"
#include <stdlib.h>
#include <map>

class Odore : public Module {
	
public:
    
    Odore(lo_server_thread s, const char *osc);
    ~Odore();
	AudioClock	*ac;
	AudioSource	*a1;
	AudioSource	*a2;
	DAC			*dac;
    float		melody[7];
	int			l, r;
	
	static int stream(const char   *path, 
					  const char   *types, 
					  lo_arg       **argv, 
					  int          argc,
					  void         *data, 
					  void         *user_data);
};

#endif