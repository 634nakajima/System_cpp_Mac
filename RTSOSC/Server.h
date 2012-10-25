//
//  Server.h
//  
//
//  Created by NAKAJIMA Musashi on 12/10/26.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _Server_h
#define _Server_h

#include "lo.h"

class Server
{
public:
    lo_server_thread st;
    
    Server();
    ~Server();
};

#endif
