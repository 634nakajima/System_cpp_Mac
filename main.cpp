#include "rtsosc.h"
#include "MyModule.h"

int main()
{
	MyModule    *o;
	Serial      *se;
	Server      *s;
    
	s = new Server();	
    o = new MyModule(s, "/Odore");
	se	= new Serial(s, "/Serial");
	se->connectTo(o, "/Stream");

	/*
	Sine		*sine;
	AudioClock	*ac;
	Server		*s;
	DAC			*dac;
	
	s = new Server();
	sine = new Sine(s, "/Sine");
	ac = new AudioClock(s, "/AC");
	dac = new DAC(s, "/DAC");
	dac->ch = 0;
	ac->connectTo(sine, "/Stream");
	sine->connectTo(dac, "/Stream");*/
	
	getchar();
	
	return 0;
}