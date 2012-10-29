#include "rtsosc.h"
#include "MyModule.h"

int main()
{
	Sine		*sine;
	AudioClock	*ac;
	Server		*s;
	DAC			*dac;
	ADC         *adc;
    LPF         *lpf;
    
	s = new Server();
	sine = new Sine(s, "/Sine");
	ac = new AudioClock(s, "/AC");
	dac = new DAC(s, "/DAC");
    adc = new ADC(s, "/ADC");
    lpf = new LPF(s, "/LPF");
    
	dac->ch = 2;
    dac->bs = 128;
	adc->connectTo(lpf, "/Stream");
	lpf->connectTo(dac, "/Stream");
	
	getchar();
	
	return 0;
}