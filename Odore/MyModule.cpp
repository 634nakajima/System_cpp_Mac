#include "MyModule.h"

MyModule::MyModule(Server *s, const char *osc) : Module(s,osc) {
    
	addMethodToServer("/Stream", "b", stream, this);
    addMethodToServer("/Data", "ii", data, this);

	a1 = new AudioSource(s, "/A1");
	a2 = new AudioSource(s, "/A2");
	ac = new AudioClock(s, "/AC");
	
	dac1 = new DAC(s, "/DACL");
	dac1->bs = 128;
	dac1->ch = 0;//Left
	
	dac2 = new DAC(s, "/DACR");
	dac2->bs = 128;
	dac2->ch = 1;//Right

	a1->prepareAudioSource("mi.wav");
	a2->prepareAudioSource("mi.wav");

	ac->connectTo(a1, "/Stream");
	ac->connectTo(a2, "/Stream");
	a1->connectTo(dac1, "/Stream");
	a2->connectTo(dac2, "/Stream");
	
	l = 0;
	r = 0;
	
	melody[0] = pow(2.0, 1.0*7.0/12.0);//71,シ
	melody[1] = pow(2.0, 0.0);//64,ミ
	melody[2] = pow(2.0, 1.0*5.0/12.0);//69,ラ
	melody[3] = pow(2.0, 1.0*15.0/12.0);//79,ソ
	melody[4] = pow(2.0, 1.0*10.0/12.0);//74,レ
	melody[5] = pow(2.0, 1.0*12.0/12.0);//76,ミ
	melody[6] = pow(2.0, 1.0*17.0/12.0);//81,ラ

}

int MyModule::stream(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data) {

	MyModule *o = (MyModule *)user_data;
	
	lo_blob b = (lo_blob)argv[0];
    unsigned char *dp = (unsigned char *)lo_blob_dataptr(b);
    int size = lo_blob_datasize(b);

	if (*dp == 0x00) {
		o->a1->rate = o->melody[o->l];
		o->l = (o->l != 6 ? o->l+1 : 0);
		o->a1->location = 0.0;
		o->a1->isPlaying = true;
	}else if (*dp == 0x01) {
		o->a2->rate = o->melody[o->r];
		o->r = (o->r != 6 ? o->r+1 : 0);
		o->a2->location = 0.0;
		o->a2->isPlaying = true;
	}else {
		printf("!");
	}

	
	return 0;
}

int MyModule::data(const char   *path, 
                   const char   *types, 
                   lo_arg       **argv, 
                   int          argc,
                   void         *data, 
                   void         *user_data) {

	return 0;
}

MyModule::~MyModule() {
}

