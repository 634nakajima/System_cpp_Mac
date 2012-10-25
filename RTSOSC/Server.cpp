//
//  Server.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/10/26.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include "Server.h"

Server::Server() {
    st = lo_server_thread_new("6340", NULL);
    lo_server_thread_start(st);
}

Server::~Server(){}