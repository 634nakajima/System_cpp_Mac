//
//  Delay.h
//  
//
//  Created by NAKAJIMA Musashi on 12/05/28.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _Delay_h
#define _Delay_h

#include "Module.h"

class Delay : public Module {
	
public:
    
    Delay(Server *s, const char *osc);
    ~Delay();
    
    float           *buf;
    short           *output;
    unsigned long   dp;
    float           t,g;
    int             ps,maxbs;
    bool            isBypassing;
	
    void bypass(int data);
    void setDt(int data);
    void setGain(int data);
    
	static int stream(const char   *path, 
					  const char   *types, 
					  lo_arg       **argv, 
					  int          argc,
					  void         *data, 
					  void         *user_data);
    
    static int data1(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data);
    
    static int data2(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data);
    
    static int data3(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data);
    
};

#endif
