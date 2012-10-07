/*
 *  ModuleList.h
 *  
 *
 *  Created by kannolab on 12/10/05.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ModuleList_h
#define _ModuleList_h

#include "Module.h"
#include "MToken.h"
#include <stdlib.h>
#include <list>

class ModuleList : public Module {
	
public:
    
    ModuleList(lo_server_thread s, const char *osc);
    ~ModuleList();
    int						t;
    std::list<MToken*>      mList;

    void    createModule(const char *tID, MToken *ml);
    void    deleteModule(const char *tID, MToken *ml);
    
    static int setMList(const char   *path, 
                        const char   *types, 
                        lo_arg       **argv, 
                        int          argc,
                        void         *data, 
                        void         *user_data);
};

#endif