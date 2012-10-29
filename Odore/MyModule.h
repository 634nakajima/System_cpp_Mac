#ifndef _MyModule_h
#define _MyModule_h

#include "rtsosc.h"

class MyModule : public Module {
	
public:
    
    MyModule(Server *s, const char *osc);
    ~MyModule();
	AudioClock	*ac;
	AudioSource	*a1;
	AudioSource	*a2;
	DAC			*dac1;
	DAC			*dac2;
    float		melody[7];
	int			l, r;
	
	static int stream(const char   *path, 
					  const char   *types, 
					  lo_arg       **argv, 
					  int          argc,
					  void         *data, 
					  void         *user_data);
    
    static int data(const char   *path, 
                    const char   *types, 
                    lo_arg       **argv, 
                    int          argc,
                    void         *data, 
                    void         *user_data);
};

#endif