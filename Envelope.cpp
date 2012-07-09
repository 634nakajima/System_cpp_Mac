//
//  Envelope.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/07/08.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Envelope.h"

int Envelope::Stream(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data)
{
    Envelope    *env    = (Envelope *)user_data;
    short       *out    = (short *)env->output;
    int         
    lo_blob     data    = (lo_blob)argv[0];
    int         dsize   = lo_blob_datasize(data);
    short       *in     = lo_blob_dataptr(data);

    if (i++%4 == 0)
        updatevVal();
    
    for (int i=0; i<dsize/sizeof(short); i++) {
        env->vol = 0.005*env->vTable[env->vVal] + 0.995*env->vol;
        *out++ = *in++*env->vol;
    }

    adc->sendAudio(env->putput, dsize);

    return 0;
}

int Envelope::data1(const char   *path, 
               const char   *types, 
               lo_arg       **argv, 
               int          argc,
               void         *data, 
               void         *user_data)
{
    Envelope *env = (Envelope *)user_data;
    
    if (argv[0]->i != -1)
        env->isPlaying = true;

    return 0;
}

int Envelope::data2(const char   *path, 
               const char   *types, 
               lo_arg       **argv, 
               int          argc,
               void         *data, 
               void         *user_data)
{
    Envelope *env = (Envelope *)user_data;
    
    if (argv[0]->i != -1)
        env->isPlaying = true;
    
    if (argv[2]->i != -1)
        env->a = argv[2]->i;
    
    return 0;
}

int Envelope::data3(const char   *path, 
               const char   *types, 
               lo_arg       **argv, 
               int          argc,
               void         *data, 
               void         *user_data)
{
    Envelope *env = (Envelope *)user_data;
    
    if (argv[0]->i != -1)
        env->isPlaying = true;
    
    if (argv[2]->i != -1)
        env->a = argv[2]->i;
    
    if (argv[4]->i != -1)
        env->d = argv[4]->i;
    
    return 0;
}

int Envelope::data4(const char   *path, 
               const char   *types, 
               lo_arg       **argv, 
               int          argc,
               void         *data, 
               void         *user_data)
{
    Envelope *env = (Envelope *)user_data;
    
    if (argv[0]->i != -1)
        env->isPlaying = true;
    
    if (argv[2]->i != -1)
        env->a = argv[2]->i;
    
    if (argv[4]->i != -1)
        env->d = argv[4]->i;
    
    if (argv[6]->i != -1)
        env->s = argv[6]->i;
    
    return 0;
}

int Envelope::data4(const char   *path, 
                    const char   *types, 
                    lo_arg       **argv, 
                    int          argc,
                    void         *data, 
                    void         *user_data)
{
    Envelope *env = (Envelope *)user_data;
    
    if (argv[0]->i != -1)
        env->isPlaying = true;
    
    if (argv[2]->i != -1)
        env->a = argv[0]->i;
    
    if (argv[4]->i != -1)
        env->d = argv[2]->i;
    
    if (argv[6]->i != -1)
        env->s = argv[4]->i;
    
    if (argv[8]->i != -1)
        env->r = argv[6]->i;
    
    return 0;
}

Envelope::Envelope(lo_server_thread s, const char *osc) : Module(s, osc)
{
    addMethodToServer("/Stream", "b", Envelope::stream, this);
    addMethodToServer("/Data", "ii", Envelope::data1, this);
    addMethodToServer("/Data", "iiii", Envelope::data2, this);
    addMethodToServer("/Data", "iiiiii", Envelope::data3, this);
    addMethodToServer("/Data", "iiiiiiii", Envelope::data4, this);
    addMethodToServer("/Data", "iiiiiiiiii", Envelope::data5, this);


    for (int i=0; i<128; i++) {
        vTable[i] = 1.0 - logf((float)(127-i))/logf(127.0);
    }
    
    vTable[127]     = 1.0;
    vVal            = 127;
    vol             = 1.0;
    isPlaying       = false;
    sampleRate      = 44100.0;
    output          = (short *)malloc(sizeof(short)*MAX_PACKET);
    memset(output, 0, sizeof(short)*MAX_PACKET);
    
    a       = 1;
    d       = 10;
    s       = 100;
    r       = 10;
    i       = 0;
    count   = 0;
}

void Envelope::updatevVal()
{
    float fc = (float)count+1.0;
    float fa = (float)a+1.0;
    float fd = (float)d+1.0;
    float fr = (float)r+1.0;
    
    if (isPlaying) {
        if (count <= a) {
            vVal = fc*127.0/fa
        }
        else if (count <= a+d) {
            vVal = 127.0 - (fc-fa)*(127.0-(float)s)/fd;
        }
        else {
            vVal = s;
        }
    } else {
        vVal -= (float)s/fr;
        if (vVal<=0)
            vVal = 0;
    }
    count++;
}
















