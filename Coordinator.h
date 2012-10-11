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
#include <map>
#include <list>

class Coordinator : public Module {

public:
    
    Coordinator(lo_server_thread s, const char *osc);
    ~Coordinator();
    
    std::map<int, MToken*>  mtknMap;
    std::list<MToken*>      mList;
    int                     mNum, mID;
    XBeeController			*xbc;
	
	void	setXBC(XBeeController *xbeecontroller);
    void    connect(int mID1, int mID2, const char *t);
    void    disconnect(int mID1, int mID2, const char *t);
    void    display(int m);
    void    createModule(const char *tID, MToken *ml);
    void    deleteModule(const char *tID, MToken *ml);
    
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
    
    static int setMList(const char   *path, 
                        const char   *types, 
                        lo_arg       **argv, 
                        int          argc,
                        void         *data, 
                        void         *user_data);
};

#endif
