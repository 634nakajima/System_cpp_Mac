//
//  ModuleManager.h
//  
//
//  Created by NAKAJIMA Musashi on 12/12/18.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _ModuleManager_h
#define _ModuleManager_h

#include <stdio.h>
#include <stdlib.h>
#include "Module.h"
#include "DAC.h"
#include "ADC.h"
#include "AudioSource.h"
#include "Delay.h"

#include <list>

class ModuleManager : public Module
{
public:
    
    std::list<DAC*>         dacLList;
    std::list<DAC*>         dacRList;
    std::list<ADC*>         adcList;
    std::list<AudioSource*> a1List;
    std::list<AudioSource*> a2List;
	std::list<Delay*>       delayList;
    
    ModuleManager(Server *s, const char *osc);
    ~ModuleManager();
    
    void sendModuleList();
    
private:
    
	static int dacL(const char   *path, 
                    const char   *types, 
                    lo_arg       **argv, 
                    int          argc,
                    void         *data, 
                    void         *user_data);
    
    static int dacR(const char   *path, 
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
	
	static int a1(const char   *path, 
                  const char   *types, 
                  lo_arg       **argv, 
                  int          argc,
                  void         *data, 
                  void         *user_data);
	
    static int a2(const char   *path, 
                  const char   *types, 
                  lo_arg       **argv, 
                  int          argc,
                  void         *data, 
                  void         *user_data);

    static int delay(const char   *path, 
					 const char   *types, 
					 lo_arg       **argv, 
					 int          argc,
					 void         *data, 
					 void         *user_data);
	
	static int requestML(const char   *path, 
						 const char   *types, 
						 lo_arg       **argv, 
						 int          argc,
						 void         *data, 
						 void         *user_data);	
};

#endif
