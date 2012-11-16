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

class MyModule : public Module {
	
public:
    int tID;
    
    MyModule(Server *s, const char *osc);
    ~MyModule();
    
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