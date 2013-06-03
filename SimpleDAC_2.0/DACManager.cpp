//
//  DACManager.cpp
//  
//
//  Created by NAKAJIMA Musashi on 13/02/23.
//  Copyright 2013年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "DACManager.h"

DACManager::DACManager(Server *s, const char *osc) : ModuleManager(s, osc) {
    strcpy(mPath, "/SP/DAC");
    mColor = 1;
    addMethodToServer(mPath, "is", dac, this);//1:create 0:delete, tID
}

int DACManager::dac(const char   *path, 
                    const char   *types, 
                    lo_arg       **argv, 
                    int          argc,
                    void         *data, 
                    void         *user_data)
{
    DACManager *dm = (DACManager *)user_data;
    char p[64];
    strcpy(p, dm->OSCAddr);     //ModuleManagerアドレス
    strcat(p, dm->mPath);       //ModuleManagerが管理するModuleアドレス
    strcat(p, "/Tile");
    strcat(p, &argv[1]->s); //タイルID
    printf("Module Addr:%s\n",p);
    
    if (argv[0]->i) {       //argv[0] = 1:モジュール生成 0:モジュール解放
        for (std::list<DAC*>::iterator iter = dm->dacList.begin(); iter != dm->dacList.end(); iter++) {
            DAC* dac = (*iter);
            if (strcmp(p,dac->OSCAddr)==0) {
                if (dac->tID == atoi(&argv[1]->s)) {
                    printf("err: Creating DAC\n");
                    return 0;
                }
            }
        }
        
        DAC *dac = new DAC(dm->st, p);
        dac->setTID(atoi(&argv[1]->s));
        dac->mColor = dm->mColor;
        dac->sendSetMdtkn();
        dm->dacList.push_back(dac);
        printf("create DAC\n");
        
    }else {
        for (std::list<DAC*>::iterator iter = dm->dacList.begin(); iter != dm->dacList.end(); iter++) {
            DAC* dac = (*iter);
            if (strcmp(p,dac->OSCAddr)==0) {
                dac->sendDeleteMdtkn();
                delete dac;
                dm->dacList.remove(dac);
                printf("delete DAC\n");
            }
        }
    }
    return 0;
}

DACManager::~DACManager() {
    for (std::list<DAC*>::iterator iter = dacList.begin(); iter != dacList.end(); iter++) {
        DAC* dac = (*iter);
        dacList.remove(dac);
        delete dac;
    }
    
    deleteMethodFromServer("/SP/DAC", "is");
}


