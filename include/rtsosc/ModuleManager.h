//
//  ModuleManager.h
//  
//
//  Created by NAKAJIMA Musashi on 13/02/23.
//  Copyright 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef _ModuleManager_h
#define _ModuleManager_h


#include <stdio.h>
#include <stdlib.h>
#include "Module.h"

#include <list>

class ModuleManager : public Module
{
public:
    
    ModuleManager(Server *s, const char *osc);
    ~ModuleManager();
    char mPath[64];
    void sendModuleList();
    void deleteModuleList();
    
private:
    	
	static int requestML(const char   *path, 
						 const char   *types, 
						 lo_arg       **argv, 
						 int          argc,
						 void         *data, 
						 void         *user_data);	
};

#endif
