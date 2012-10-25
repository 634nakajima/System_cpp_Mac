/*
 *  odore.h
 *  
 *
 *  Created by kannolab on 12/10/25.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _MyModule_h
#define _MyModule_h

#include "rtsosc.h"
#include <stdlib.h>
#include <map>

class MyModule : public Module {
	
public:
    
    MyModule(lo_server_thread s, const char *osc);
    ~MyModule();
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
    
    static int data(const char   *path, 
                    const char   *types, 
                    lo_arg       **argv, 
                    int          argc,
                    void         *data, 
                    void         *user_data);
};

#endif