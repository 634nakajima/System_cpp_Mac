/*
 *  AudioSource.cpp
 *  
 *
 *  Created by kannolab on 12/07/16.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "AudioSource.h"

void AudioSource::render(PtTimestamp timestamp, void *userData)
{
	AudioSource *audio = (AudioSource *)userData;
    
	int i;
	
    memset(audio->buf, 0, audio->numPackets*sizeof(float));
    memset(audio->output, 0, audio->numPackets*sizeof(short));
	
    for (i=0;i<audio->numPackets;i++) {
        float a,b,f;
        int l;
        
        b = audio->location - (unsigned)audio->location;
        a = 1.0-b;
        l = (unsigned)audio->location;
        
        audio->buf[i] += a*audio->sample[l];
        audio->buf[i] += b*audio->sample[l+1];
		
        audio->location += audio->rate*audio->packetCount/audio->sampleRate;
        
        while (audio->location >= audio->packetCount-1) {
            audio->location = audio->location - (float)audio->packetCount+1;
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

int AudioSource::stream(const char   *path, 
				 const char   *types, 
				 lo_arg       **argv, 
				 int          argc,
				 void         *data, 
				 void         *user_data)
{
    return 0;
}

int AudioSource::data1(const char   *path, 
				const char   *types, 
				lo_arg       **argv, 
				int          argc,
				void         *data, 
				void         *user_data)
{
    AudioSource *audio = (AudioSource *)user_data;
    
    float f = argv[0]->i;
    if (f > 64.0) {
        audio->rate = f/64.0;
    }
    else if (f >= 0) {
        audio->rate = (63.0+f)/127.0;
    }
    
    return 0;
}

AudioSource::AudioSource(lo_server_thread s, const char *osc) : Module(s, osc)
{
	addMethodToServer("/Stream", "b", AudioSource::stream, this);
    addMethodToServer("/Data", "ii", AudioSource::data1, this);
	
	sampleRate	= SAMPLE_RATE;
	numPackets	= 256;
	interval	= (double)numPackets/sampleRate;
	
	buf			= (float *)malloc(numPackets*sizeof(float));
	output		= (short *)malloc(numPackets*sizeof(short));
	
	prepareAudioResources();
	initAudioInfo();
	Pt_Start(interval, render, this);
}

int AudioSource::prepareAudioResources()
{
		
	ifd = -1;
	sample = NULL;
	PSF_PROPS props;
	
	if(psf_init()){
		printf("unable to start portsf\n");
		return 1;
	}
    
	ifd  = psf_sndOpen("sound_c.wav",&props,0);
	if(ifd < 0){
		printf("Unable to open infile %s\n","sound_c.wav");
		return 1;
	}

    packetCount = psf_sndSize(ifd);
	if(size <= 0)
		printf("cannot find file size\n");

	printf("File size = %ld frames\n",size);

	sample = (float *) malloc(packetCount * sizeof(float));
	if(sample==NULL){
        puts("no memory for frame buffer\n");
	}

    psf_sndReadFloatFrames(ifd, sample, packetCount);

	return 0;		
}

void AudioSource::initAudioInfo()
{
	rate = 1.0;
    location = 0.0;
}

AudioSource::~AudioSource()
{
	Pt_Stop();
	free(buf);
	free(output);
    free(sample);
    psf_sndClose(ifd);

}
