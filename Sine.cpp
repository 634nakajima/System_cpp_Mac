/*
 *  Sine.cpp
 *  
 *
 *  Created by kannolab on 12/07/13.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sine.h"

void Sine::render(PtTimestamp timestamp, void *userData)
{
	Sine *sine = (Sine *)userData;
    
	int i;
	if (timestamp != -1) {
		memset(sine->buf, 0, sine->numPackets*sizeof(float));
	}
	memset(sine->output, 0, sine->numPackets*sizeof(short));
	
    for (i=0;i<sine->numPackets;i++) {
        float a,b,f;
        int l;
        
        b = sine->location - (unsigned)sine->location;
        a = 1.0-b;
        l = (unsigned)sine->location;
        f = (float)sine->fTable[sine->freq];
        
        sine->fVal = f*0.0004 + sine->fVal*0.9996;
        sine->buf[i] += a*sine->sample[l];
        sine->buf[i] += b*sine->sample[l+1];
		
        sine->location += sine->fVal*sine->packetCount/sine->sampleRate;
        
        while (sine->location >= sine->packetCount-1) {
            sine->location = sine->location - (float)sine->packetCount+1;
        }
    }
	
    //クリッピング
    for (i=0; i<sine->numPackets;i++) {
        if (sine->buf[i]>1) {
            sine->buf[i] = 1;
        }
        if (sine->buf[i]<-1) {
            sine->buf[i] = -1;
        }
        sine->output[i] = sine->buf[i]*32767;
    }
    
    //データ送信
    sine->sendAudio(sine->output, sine->numPackets*sizeof(short));

}

int Sine::stream(const char   *path, 
				 const char   *types, 
				 lo_arg       **argv, 
				 int          argc,
				 void         *data, 
				 void         *user_data)
{
	Sine *sine = (Sine *)user_data;
	
	lo_blob b = (lo_blob)argv[0];
    short *dp = (short *)lo_blob_dataptr(b);
	
	for(int i=0; i<sine->numPackets; i++){
		sine->buf[i] = *dp++ /32768.0;
	}
	sine->render(-1, sine);
    return 0;
}

int Sine::data1(const char   *path, 
				const char   *types, 
				lo_arg       **argv, 
				int          argc,
				void         *data, 
				void         *user_data)
{
    Sine *sine = (Sine *)user_data;
    int f = argv[0]->i;
    if (f > 0)
        sine->freq = f;
    
    return 0;
}

Sine::Sine(lo_server_thread s, const char *osc) : Module(s, osc)
{
	addMethodToServer("/Stream", "b", Sine::stream, this);
    addMethodToServer("/Data", "ii", Sine::data1, this);

	sampleRate	= SAMPLE_RATE;
	numPackets	= 256;
	interval	= (double)numPackets/sampleRate;

	buf			= (float *)malloc(numPackets*sizeof(float));
	output		= (short *)malloc(numPackets*sizeof(short));
	
	prepareAudioResources();
	initWave();
}

void Sine::prepareAudioResources()
{
	int i;
    for (i=0; i<128; i++) {
        fTable[i] = 440.0 * pow(2.0, (i-69)/12.0);
    }
	
    packetCount = 8192*16;
    sample	= (float *)malloc(packetCount*sizeof(float));
    
    for (i=0; i<packetCount; i++) {
        sample[i] = (float)0.3*sin(2.0*M_PI*i/(float)packetCount);
    }
}

void Sine::initWave()
{
	freq = 69;
    fVal = 440.0;
    location = 0.0;
}

Sine::~Sine()
{
	free(buf);
	free(output);
}
