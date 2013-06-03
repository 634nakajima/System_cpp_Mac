//
//  DACManager.h
//  
//
//  Created by NAKAJIMA Musashi on 13/02/23.
//  Copyright 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef _DACManager_h
#define _DACManager_h

#include "ModuleManager.h"
#include "DAC.h"

class DACManager : public ModuleManager
{
public:
    std::list<DAC*> dacList;

    DACManager(Server *s, const char *osc);
    ~DACManager();
    
private:
    
	static int dac(const char   *path, 
                   const char   *types, 
                   lo_arg       **argv, 
                   int          argc,
                   void         *data, 
                   void         *user_data);
};

#endif
