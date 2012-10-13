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
#include "Serial.h"
#include "MToken.h"
#include <stdlib.h>
#include <map>

class ModuleList : public Module {
	
public:
    
    ModuleList(lo_server_thread s, const char *osc);
    ~ModuleList();
    std::map<int, MToken*>      mlMap;
	Serial                      *serial;
    char                        t;

    void    createModule(const char *tID, MToken *ml);
    void    createModule(const char *tID, int mc);
    void    deleteModule(const char *tID, MToken *ml);
    void    deleteModule(const char *tID, int mc);

    
    static int setMList(const char   *path, 
                        const char   *types, 
                        lo_arg       **argv, 
                        int          argc,
                        void         *data, 
                        void         *user_data);
	
	static int stream(const char   *path, 
					  const char   *types, 
					  lo_arg       **argv, 
					  int          argc,
					  void         *data, 
					  void         *user_data);
};

#endif