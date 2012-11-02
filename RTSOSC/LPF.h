//
//  LPF.h
//  
//
//  Created by NAKAJIMA Musashi on 12/10/29.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _LPF_h
#define _LPF_h

#include "rtsosc.h"

class LPF : public Module {
	
public:
    
    LPF(Server *s, const char *osc);
    ~LPF();
    
    float           buf[3];
    short           *output;
    float           q,freq;
    float           a[3], b[3];
    bool            isBypassing;
	
    void bypass(int data);
    void setQ(int data);
    void setFreq(int data);
    
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
