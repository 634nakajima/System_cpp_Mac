/*
 *  PCMSynth.cpp
 *  
 *
 *  Created by kannolab on 12/08/03.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "PCMSynth.h"

PCMSynth::PCMSynth(lo_server_thread s, const char *osc) : Module(s,osc)
{
	addMethodToServer("/Stream", "b", AudioSource::stream, this);
    addMethodToServer("/Data", "ii", AudioSource::data1, this);
	addMethodToServer("/Data", "iiii", AudioSource::data2, this);
	addMethodToServer("/Data", "iiiiii", AudioSource::data3, this);
}

void PCMSynth::render(PtTimestamp timestamp, void *userData)
{
	PCMSynth *synth = (PCMSynth *)userData;
    
	int i;
	if (timestamp != -1) {
		memset(synth->buf, 0, synth->numPackets*sizeof(float));
	}
	memset(synth->output, 0, synth->numPackets*sizeof(short));
	
	if (synth->isPlaying) {
		for (i=0;i<synth->numPackets;i++) {
			float a,b,f;
			int	l;
			
			b = audio->location - (unsigned)audio->location;
			a = 1.0-b;
			l = (unsigned)audio->location;
			
			audio->buf[i] += 5.0*a*audio->sample[l];
			audio->buf[i] += 5.0*b*audio->sample[l+1];
			
			audio->location += audio->rate;
			
			if (audio->location >= audio->packetCount-1) {
				if (audio->isLooping) {
					audio->location = audio->location - (float)audio->packetCount+1;
				} else {
					audio->location = 0.0;
					audio->isPlaying = false;
					break;
				}
			}
		}
	}
	
	//クリッピング
	for (i=0; i<audio->numPackets;i++) {
		if (audio->buf[i]>1) {
			audio->buf[i] = 1;
		}
		if (audio->buf[i]<-1) {
			audio->buf[i] = -1;
		}
		audio->output[i] = audio->buf[i]*32767;
	}
	
    //データ送信
    audio->sendAudio(audio->output, audio->numPackets*sizeof(short));
	
}

int PCMSynth::stream(const char   *path, 
					 const char   *types, 
					 lo_arg       **argv, 
					 int          argc,
					 void         *data, 
					 void         *user_data)
{
	AudioSource *audio = (AudioSource *)user_data;
	
	lo_blob b = (lo_blob)argv[0];
    short *dp = (short *)lo_blob_dataptr(b);
	
	for(int i=0; i<audio->numPackets; i++){
		audio->buf[i] = *dp++ /32768.0;
	}
	
	audio->render(-1, audio);
	
    return 0;
}

int PCMSynth::data1(const char   *path, 
					const char   *types, 
					lo_arg       **argv, 
					int          argc,
					void         *data, 
					void         *user_data)
{
	AudioSource *audio = (AudioSource *)user_data;
	
	int v = argv[0]->i;
	
	if (v < 64) {
		audio->isPlaying = false;
		audio->location = 0.0;
	} else {
		audio->isPlaying = true;
	}
	return 0;
}
int PCMSynth::data2(const char   *path, 
					const char   *types, 
					lo_arg       **argv, 
					int          argc,
					void         *data, 
					void         *user_data)
{
    AudioSource *audio = (AudioSource *)user_data;
    
    float f = (float)argv[0]->i;
	
	if (f > 64.0) {
        audio->rate = f/64.0;
    }
    else if (f >= 0.0) {
        audio->rate = (63.0+f)/127.0;
    }
    return 0;
}

int PCMSynth::data3(const char   *path, 
					const char   *types, 
					lo_arg       **argv, 
					int          argc,
					void         *data, 
					void         *user_data)
{
    AudioSource *audio = (AudioSource *)user_data;
    
	int v = argv[0]->i;
	
	if (v < 1) {
		audio->isLooping = false;
	} else {
		audio->isLooping = true;
	}
	return 0;
}