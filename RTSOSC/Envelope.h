//
//  Envelope.h
//  
//
//  Created by NAKAJIMA Musashi on 12/07/08.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _Envelope_h
#define _Envelope_h

#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include "Module.h"

#define MAX_PACKET          44100

class Envelope : public Module {
public:
    int     A, D, S, R, adsr, count, vVal;
    float   vol, v, vTable[128], sampleRate;
    bool    isPlaying;
    short   *output;
    
    Envelope(lo_server_thread s, const char *osc);
    ~Envelope();
    
private:
    
    void updatevVal();
    
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
    
    static int data4(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data);
    
    static int data5(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data);
};


#endif
