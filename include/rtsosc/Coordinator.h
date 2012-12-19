//
//  Coordinator.h
//  SimpleSimulator
//
//  Created by NAKAJIMA Musashi on 12/06/18.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _Coordinator_h
#define _Coordinator_h

#include "Module.h"
#include "MToken.h"
#include "XBeeController.h"
#include "ModuleList.h"
#include <map>
#include <list>

class XBeeController;
class Coordinator : public Module {

public:
    
    Coordinator(Server *s, const char *osc);
    ~Coordinator();
    
    std::map<int, MToken*>      mtknMap;
    XBeeController              *xbc;
    ModuleList                  *ml;
	
	void	setXBC(XBeeController *xbeecontroller);
    void    connect(int tID1, int tID2, const char *t);
    void    addConnection(int tID1, int tID2, const char *t);
    void    disconnect(int tID1, int tID2, const char *t);
	void    disconnectAll(int tID, const char *t);
    void    deleteMtkn(int tID);
    
    static int setMtkn(const char   *path, 
                       const char   *types, 
                       lo_arg       **argv, 
                       int          argc,
                       void         *data, 
                       void         *user_data);
    
    static int deleteMtkn(const char   *path, 
                          const char   *types, 
                          lo_arg       **argv, 
                          int          argc,
                          void         *data, 
                          void         *user_data);

};

#endif
