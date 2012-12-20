#include "Delay.h"

Delay::Delay(Server *s, const char *osc) : Module(s,osc) {
    
	addMethodToServer("/Stream", "b", stream, this);
    addMethodToServer("/Data", "ii", data1, this);
    addMethodToServer("/Data", "iiii", data2, this);
    addMethodToServer("/Data", "iiiiii", data3, this);

    maxbs       = sizeof(float)*44100;
    t           = 300;
    g           = 0.2;
    dp          = 0;   
    ps          = 44.1*t;
    isBypassing = false;
    output      = (short *)calloc(4192,sizeof(short));
    buf         = (float *)malloc(maxbs);
    memset(buf, 0, maxbs);
    
}

int Delay::stream(const char   *path, 
                  const char   *types, 
                  lo_arg       **argv, 
                  int          argc,
                  void         *data, 
                  void         *user_data) {
    
	Delay *delay = (Delay *)user_data;
	
	lo_blob b = (lo_blob)argv[0];
    short *p = (short *)lo_blob_dataptr(b);
    int dsize = lo_blob_datasize(b);
    
    short   sout;
    float   fout;
    memset(delay->output, 0, dsize);
    
    if (!delay->isBypassing) {
        for (int i=0;i<dsize/sizeof(short);i++) {
            fout = delay->buf[delay->dp] + (float)p[i]/32767.0;
            if (fout>1.0) fout = 1.0;
            if (fout<-1.0) fout = -1.0;
            sout = fout*32767;
            delay->buf[delay->dp] = (float)p[i]*delay->g/32767.0 + fout*delay->g;
            delay->output[i] = sout;
            delay->dp = (delay->dp <= delay->ps-1 ? delay->dp+1 : 0);
        }
    } else {
		for (int i=0;i<dsize/sizeof(short);i++) {
			delay->output[i] = p[i];
        }
    }
    delay->sendAudio(delay->output, dsize);

	return 0;
}

int Delay::data1(const char   *path, 
                 const char   *types, 
                 lo_arg       **argv, 
                 int          argc,
                 void         *data, 
                 void         *user_data) {
    
    Delay *d = (Delay *)user_data;
    int dval[argc], i;
    
    for (i=0; i<argc; i++) {
        dval[i] = argv[i]->i;
    }
    
    d->setDt(dval[0]);
	printf("!");
	return 0;
}

int Delay::data2(const char   *path, 
                 const char   *types, 
                 lo_arg       **argv, 
                 int          argc,
                 void         *data, 
                 void         *user_data) {
    
    Delay *d = (Delay *)user_data;
    int dval[argc], i;
    
    for (i=0; i<argc; i++) {
        dval[i] = argv[i]->i;
    }
    
    //d->setDt(dval[2]);
    d->setGain(dval[0]);

	return 0;
}

int Delay::data3(const char   *path, 
                    const char   *types, 
                    lo_arg       **argv, 
                    int          argc,
                    void         *data, 
                    void         *user_data) {
    
    Delay *d = (Delay *)user_data;
    int dval[argc], i;
    
    for (i=0; i<argc; i++) {
        dval[i] = argv[i]->i;
    }
    
    //d->setDt(dval[4]);
    //d->setGain(dval[2]);
    d->bypass(dval[0]);

	return 0;
}

void Delay::bypass(int data) {
    if (data == -1) return;
    
    isBypassing = (data < 1 ? true : false);
    memset(buf, 0, maxbs);
}

void Delay::setDt(int data) {
    if (data == -1) return;
    
    t = (float)data*500.0/127.0;
    ps = 44.1*t;
}

void Delay::setGain(int data) {
    if (data == -1) return;
    g = (float)data/127.0;
}

Delay::~Delay() {
    free(buf);
    free(output);
	
	deleteMethodFromServer("/Stream", "b");
    deleteMethodFromServer("/Data", "ii");
	deleteMethodFromServer("/Data", "iiii");
    deleteMethodFromServer("/Data", "iiiiii");
}

