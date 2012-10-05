/*
 *  ModuleList.cpp
 *  
 *
 *  Created by kannolab on 12/10/05.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ModuleList.h"
#include <iostream>

ModuleList::ModuleList(lo_server_thread s, const char *osc) : Module(s,osc)
{
    addMethodToServer("/setMList", "ss", setMList, this);
	
}

static int setMList(const char   *path, 
                    const char   *types, 
                    lo_arg       **argv, 
                    int          argc,
                    void         *data, 
                    void         *user_data)
{
    int i = 0;
    Coordinator *coordinator = (Coordinator *)user_data;
    //エラー処理、既存のモジュールリスト確認
    for (std::list<MToken*>::iterator iter = mList.begin(); iter!=mList.end(); iter++) {
        MToken* ml = (*iter);
        if (strcmp(ml->ip, (char *)argv[0])==0) {
            if (strcmp(ml->osc, (char *)argv[1])==0) return 0;
        }
    }
    
    //モジュールリストの生成
    MToken *m = new MToken();
    strcpy(m->ip, (char *)argv[0]);
    strcpy(m->osc, (char *)argv[1]);
    coordinator->mList->push_back(m);
    
    printf("set:%s,%s\n",(char *)argv[0], (char *)argv[1]);
    
    return 0;
	
}

void ModuleList::createModule(int *tID, MToken *ml)
{
    lo_send(lo_address_new(ml->ip,"6340"), 
            ml->osc,
            "is", 
            1,
            strcat("/Tile",itoa(tID)));
	
}

void ModuleList::deleteModule(int *tID, MToken *ml)
{
    lo_send(lo_address_new(ml->ip,"6340"), 
            ml->osc,
            "is", 
            0,
            strcat("/Tile",itoa(tID)));
}

ModuleList::~ModuleList()
{
    for (std::list<MToken*>::iterator iter = mList.begin(); iter!=mList.end(); iter++)
		delete (*iter);
	mList.clear();
}





