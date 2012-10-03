//
//  ModuleController.h
//  
//
//  Created by NAKAJIMA Musashi on 12/10/01.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _ModuleController_h
#define _ModuleController_h

#include <stdio.h>
#include <stdlib.h>
#include "Module.h"
#include "DAC.h"
#include "ADC.h"
#include "Sine.h"
#include "Envelope.h"
#include "AudioSource.cpp"
#include <list>

class ModuleController : public Module
{
public:
    
    std::list<DAC*> dacMap;
    std::map<ADC*> adcList;
    std::map<Sine*> sineList;
    std::map<Envelope*> envList;
    std::map<AudioSource*> asList;
    
    ModuleController(lo_server_thread s, const char *osc);
    ~ModuleController();
    
private:
    
	static int dac(const char   *path, 
                   const char   *types, 
                   lo_arg       **argv, 
                   int          argc,
                   void         *data, 
                   void         *user_data);
    
    static int adc(const char   *path, 
                   const char   *types, 
                   lo_arg       **argv, 
                   int          argc,
                   void         *data, 
                   void         *user_data);
	
	static int sine(const char   *path, 
                    const char   *types, 
                    lo_arg       **argv, 
                    int          argc,
                    void         *data, 
                    void         *user_data);
	
    static int env(const char   *path, 
                   const char   *types, 
                   lo_arg       **argv, 
                   int          argc,
                   void         *data, 
                   void         *user_data);
    
    static int as(const char   *path, 
                  const char   *types, 
                  lo_arg       **argv, 
                  int          argc,
                  void         *data, 
                  void         *user_data);

};


#endif
