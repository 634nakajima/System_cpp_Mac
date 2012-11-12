#include "rtsosc.h"
#include "MyModule.h"

int main()
{
	Sine		*sine;
	AudioClock	*ac;
	Server		*s;
	DAC			*dac;
	ADC         *adc;
    
	s = new Server();
	sine = new Sine(s, "/Sine");
	ac = new AudioClock(s, "/AC");
	dac = new DAC(s, "/DAC");
    adc = new ADC(s, "/ADC");
    
	dac->ch = 2;
    dac->bs = 128;
	adc->connectTo(dac, "/Stream");
	
	getchar();
	
	return 0;
}