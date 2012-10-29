//
//  LPF.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/10/29.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include "LPF.h"

LPF::LPF(Server *s, const char *osc) : Module(s,osc) {
    
	addMethodToServer("/Stream", "b", stream, this);
    addMethodToServer("/Data", "ii", data1, this);
    addMethodToServer("/Data", "iiii", data2, this);
    addMethodToServer("/Data", "iiiiii", data3, this);
    
    q           = 0.707;
    freq        = 500.0;
    isBypassing = false;
    output = (short *)calloc(4192,sizeof(short));

    double w = tan(M_PI*freq/44100.0);
    double a = sin(w)/q;
                   
    b0 = 0.1;//(float)((1.0-cos(w))/2*(1+a));
    b1 = 0.45;//(float)((1.0-cos(w))/(1+a));
    b2 = b1;
    a1 = 0.0;//(float)(2*cos(w)/(1-a));
    a2 = 0.0;//(float)((a-1)/(a+1));

}

int LPF::stream(const char   *path, 
                const char   *types, 
                lo_arg       **argv, 
                int          argc,
                void         *data, 
                void         *user_data) {
    
	LPF *lpf = (LPF *)user_data;
	lo_blob b = (lo_blob)argv[0];
    short *p = (short *)lo_blob_dataptr(b);
    int dsize = lo_blob_datasize(b);
    
    short   sout;
    float   fout;
    memset(lpf->output, 0, dsize);
    
    if (!lpf->isBypassing) {
        for (int i=0;i<dsize/sizeof(short);i++) {
            lpf->buf[0] = lpf->buf[1]*lpf->a1 + lpf->buf[2]*lpf->a2 + (float)p[i]/32767.0;
            if (lpf->buf[0]>1.0) lpf->buf[0] = 1.0;
            if (lpf->buf[0]<-1.0) lpf->buf[0] = -1.0;
            lpf->output[i] = (lpf->buf[0]*lpf->b0 + lpf->buf[1]*lpf->b1 + lpf->buf[2]*lpf->b2)*32767;
            lpf->buf[2] = lpf->buf[1];
            lpf->buf[1] = lpf->buf[0];
        }
    } else {
		for (int i=0;i<dsize/sizeof(short);i++) {
			lpf->output[i] = p[i];
        }
    }
    lpf->sendAudio(lpf->output, dsize);
    
	return 0;
}

int LPF::data1(const char   *path, 
                 const char   *types, 
                 lo_arg       **argv, 
                 int          argc,
                 void         *data, 
                 void         *user_data) {
    
    LPF *lpf = (LPF *)user_data;
    int dval[argc], i;
    
    for (i=0; i<argc; i++) {
        dval[i] = argv[i]->i;
    }
    
    lpf->setQ(dval[0]);
    
	return 0;
}

int LPF::data2(const char   *path, 
                 const char   *types, 
                 lo_arg       **argv, 
                 int          argc,
                 void         *data, 
                 void         *user_data) {
    
    LPF *lpf = (LPF *)user_data;
    int dval[argc], i;
    
    for (i=0; i<argc; i++) {
        dval[i] = argv[i]->i;
    }
    
    lpf->setQ(dval[2]);
    lpf->bypass(dval[0]);
    
	return 0;
}

int LPF::data3(const char   *path, 
                 const char   *types, 
                 lo_arg       **argv, 
                 int          argc,
                 void         *data, 
                 void         *user_data) {
    
    LPF *lpf = (LPF *)user_data;
    int dval[argc], i;
    
    for (i=0; i<argc; i++) {
        dval[i] = argv[i]->i;
    }
    
    lpf->setQ(dval[4]);
    lpf->bypass(dval[2]);
    lpf->setFreq(dval[0]);
    
	return 0;
}

void LPF::bypass(int data) {
    if (data == -1) return;
    isBypassing = (data < 64 ? true : false);
}

void LPF::setQ(int data) {
    if (data == -1) return;
    q = ((float)data/64.0 + 0.1);
    
    double w = tan(M_PI*freq/44100.0);
    double a = sin(w)/q;

    b0 = (float)((1.0-cos(w))/2*(1+a));
    b1 = (float)((1.0-cos(w))/(1+a));
    b2 = b1;
    a1 = (float)(2*cos(w)/(1-a));
    a2 = (float)((a-1)/(a+1));
}

void LPF::setFreq(int data) {
    if (data == -1) return;
    freq = pow(20000.0, (float)data/127.0);
    
    double w = tan(M_PI*freq/44100.0);
    double a = sin(w)/q;
    
    b0 = (float)((1.0-cos(w))/2*(1+a));
    b1 = (float)((1.0-cos(w))/(1+a));
    b2 = b1;
    a1 = (float)(2*cos(w)/(1-a));
    a2 = (float)((a-1)/(a+1));
}

LPF::~LPF() {
    free(output);
}

