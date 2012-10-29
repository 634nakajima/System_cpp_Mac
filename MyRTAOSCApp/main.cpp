#include "rtsosc.h"
#include "MyModule.h"

int main()
{
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
	sine->connectTo(dac, "/Stream");
	
	getchar();
	
	return 0;
}