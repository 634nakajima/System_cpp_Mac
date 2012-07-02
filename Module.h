//
//  Module.h
//  
//
//  Created by NAKAJIMA Musashi on 12/05/27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _Module_h
#define _Module_h

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "lo.h"
#include "portaudio.h"
#include "RoutingTable.h"
#include "Data.h"
#include "Audio.h"

class Module
{
public:
    char                IPAddr[16];
    char                OSCAddr[64];
    lo_server_thread    st;
    RoutingTable        *rTable;
    
    Module(lo_server_thread s, const char *osc);
    ~Module();
    
    void setRoute(char *ip, char *osc);
    void deleteRoute(char *ip, char *osc);
    void addMethodToServer(const char *path, const char *type, lo_method_handler h, void *user_data);
    void sendData(Data *d);
    void sendAudio(Audio *a);
    void sendAudio(short *a, unsigned long l);
    
private:
    void getAddr();
    void setOSCAddr(const char *osc);
    virtual void sendSetMdtkn();
    virtual void sendDeleteMdtkn();

    static int setRoute(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    static int deleteRoute(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    
};

#endif
