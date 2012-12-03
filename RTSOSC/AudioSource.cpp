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
	if (timestamp != -1) {
		memset(audio->buf, 0, audio->numPackets*sizeof(float));
	}
	memset(audio->output, 0, audio->numPackets*sizeof(short));

	if (audio->isPlaying) {
		for (i=0;i<audio->numPackets;i++) {
			float a,b,f;
			int	l;
        
			b = audio->location - (unsigned)audio->location;
			a = 1.0-b;
			l = (unsigned)audio->location;
        
			audio->buf[i] += a*audio->sample[l];
			audio->buf[i] += b*audio->sample[l+1];
		
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

int AudioSource::stream(const char   *path, 
						const char   *types, 
						lo_arg       **argv, 
						int          argc,
						void         *data, 
						void         *user_data)
{
	AudioSource *audio = (AudioSource *)user_data;

	lo_blob b = (lo_blob)argv[0];
    short *dp = (short *)lo_blob_dataptr(b);
    int size = lo_blob_datasize(b)/sizeof(short);
    audio->numPackets = size;
    
	for(int i=0; i<audio->numPackets; i++){
		audio->buf[i] = *dp++ /32768.0;
	}
	
	audio->render(-1, audio);
	
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
	
	int v = argv[0]->i;
	
	if (v < 0) {
		audio->isPlaying = false;
		audio->location = 0.0;
	} else {
		audio->isPlaying = true;
	}
    audio->sendData(NULL, argv, argc);
	return 0;
}
int AudioSource::data2(const char   *path, 
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
	else {
		audio->rate = 1.0;
	}
    audio->sendData(NULL, argv, argc);
    return 0;
}

int AudioSource::data3(const char   *path, 
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
    audio->sendData(NULL, argv, argc);
	return 0;
}

AudioSource::AudioSource(Server *s, const char *osc) : Module(s, osc)
{
	addMethodToServer("/Stream", "b", AudioSource::stream, this);
    addMethodToServer("/Data", "ii", AudioSource::data1, this);
	addMethodToServer("/Data", "iiii", AudioSource::data2, this);
	addMethodToServer("/Data", "iiiiii", AudioSource::data3, this);
	
	sampleRate	= SAMPLE_RATE;
	numPackets	= 256;
	interval	= (double)numPackets/sampleRate;
	
	buf			= (float *)malloc(numPackets*sizeof(float));
	output		= (short *)malloc(numPackets*sizeof(short));
	
	initAudioInfo();
}

int AudioSource::prepareAudioSource(const char *sound)
{
		
	int ifd = -1;
	sample = NULL;
	PSF_PROPS props;
	
	if(psf_init()){
		printf("unable to start portsf\n");
		return 1;
	}
    
	ifd  = psf_sndOpen(sound,&props,0);
	if(ifd < 0){
		printf("Unable to open infile %s\n",sound);
		return 1;
	}

    packetCount = psf_sndSize(ifd);
	if(packetCount <= 0)
		printf("cannot find file size\n");

	printf("File size = %ld frames\n",packetCount);

	sample = (float *) malloc(packetCount * sizeof(float));
	if(sample==NULL){
        puts("no memory for frame buffer\n");
	}

    psf_sndReadFloatFrames(ifd, sample, packetCount);
    psf_sndClose(ifd);
	prepared = true;
	return 0;		
}

void AudioSource::initAudioInfo()
{
	rate = 1.0;
    location = 0.0;
	isPlaying = false;
	isLooping = false;
	prepared = false;
}

AudioSource::~AudioSource()
{
	free(buf);
	free(output);
	if (prepared)
		free(sample);
	
	deleteMethodFromServer("/Stream", "b");
	deleteMethodFromServer("/Data", "ii");
	deleteMethodFromServer("/Data", "iiii");
	deleteMethodFromServer("/Data", "iiiiii");
}
