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
	AudioSource *sine = (AudioSource *)userData;
    
	int i;
	
    memset(sine->buf, 0, sine->numPackets*sizeof(float));
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
	initWave();
	Pt_Start(interval, render, this);
}

int AudioSource::prepareAudioResources()
{
		
	int ifd = -1,rc=0;
	MYLONG pos, size;
	float *framebuf = NULL;
	PSF_PROPS props;
	
	if(psf_init()){
		printf("unable to start portsf\n");
		return 1;
	}
	ifd  = psf_sndOpen(argv[1],&props,0);
	if(ifd < 0){
		printf("Unable to open infile %s\n",argv[1]);
		exit(1);
	}
	
	framebuf = (float *) malloc(props.chans * sizeof(float));
	if(framebuf==NULL){
			puts("no memory for frame buffer\n");
		goto finish;
	}
	
		size = psf_sndSize(ifd);
	if(size < 0)
		printf("cannot find file size\n");
	else if (size==0){
		printf("File %s is empty\n",argv[1]);
		goto finish;
	}
	printf("File size = %ld frames\n",size);
	/* seek to last sample frame */
	rc = psf_sndSeek(ifd,-1,PSF_SEEK_END);
	if(rc){
		printf("error seeking to last frame\n");
		goto finish;
	}
	
	pos = psf_sndTell(ifd);
	printf("starting at frame %ld\n",pos);
	printf("processing...\n");
	
	if(read_frame_and_reverse(framebuf,ifd)){
		printf("Error reading initial frame\n");
		goto finish;
	}
	while(size-- >= 0) {							
		if(psf_sndWriteFloatFrames(ofd,framebuf,1)<1){
			printf("error writing frame\n");
			goto finish;
		}
		if(size % 100000 == 0)
			printf("%ld\r",size);
		if(read_frame_and_reverse(framebuf,ifd) <0){
			/* we read the first sample, but failed to reverse; write final sample and break */
			if(psf_sndWriteFloatFrames(ofd,framebuf,1)<1){
				printf("error writing frame\n");
				goto finish;
			}
			break;
		}
	}
finish:
	if(framebuf)
		free(framebuf);
	if(ifd >=0)
		psf_sndClose(ifd);
	if(ofd >=0)
		psf_sndClose(ofd);
	psf_finish();
	
	return 0;		
}

void AudioSource::initWave()
{
	freq = 69;
    fVal = 440.0;
    sample = sample;
    packetCount = packetCount;
    location = 0.0;
}

AudioSource::~AudioSource()
{
	Pt_Stop();
	free(buf);
	free(output);
}
