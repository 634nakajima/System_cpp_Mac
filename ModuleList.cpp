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
    addMethodToServer("/setMList", "ssi", setMList, this);
	addMethodToServer("/Stream", "b", stream, this);
	serial = new Serial(s, "/Serial");
	serial->connectTo(this, "/Stream");
}

int ModuleList::setMList(const char   *path, 
                         const char   *types, 
                         lo_arg       **argv, 
                         int          argc,
                         void         *data, 
                         void         *user_data)
{
    int i = 0;
    ModuleList *mlc = (ModuleList *)user_data;
    //エラー処理、既存のモジュールリスト確認
    for (std::map<int, MToken*>::iterator iter = mlc->mlMap.begin(); iter != mlc->mlMap.end(); iter++) {
        MToken* ml = iter->second;
        if (strcmp(ml->ip, (char *)argv[0])==0) {
            if (strcmp(ml->osc, (char *)argv[1])==0) return 0;
        }
    }
    
    //モジュールリストの生成
    MToken *m = new MToken();
    if (argv[2]->i != -1) {//mColorが-1でなければ
        m->index = argv[2]->i;
        strcpy(m->ip, (char *)argv[0]);
        strcpy(m->osc, (char *)argv[1]);
        mlc->mlMap.insert(std::map<int, MToken*>::value_type(m->index, m));
    
        printf("set:%s,%s ModuleColor:%d\n",(char *)argv[0], (char *)argv[1], argv[2]->i);
    }
    return 0;
	
}

int ModuleList::stream(const char   *path, 
					   const char   *types, 
					   lo_arg       **argv, 
					   int          argc,
					   void         *data, 
					   void         *user_data)
{
	ModuleList *mlc = (ModuleList *)user_data;
	lo_blob b = (lo_blob)argv[0];
    char *dp = (char *)lo_blob_dataptr(b);
    int size = lo_blob_datasize(b);
	
	mlc->t = *dp;
}

void ModuleList::createModule(const char *tID, MToken *ml)
{
    lo_send(lo_address_new(ml->ip,"6340"), 
            ml->osc,
            "is", 
            1,
            tID);
}

void ModuleList::createModule(const char *tID, int mc)
{
    MToken *m = mlMap[mc];

    if (m != NULL) {
        createModule(tID, m);
    }
}

void ModuleList::deleteModule(const char *tID, MToken *ml)
{
    lo_send(lo_address_new(ml->ip,"6340"), 
            ml->osc,
            "is", 
            0,
            tID);
}

void ModuleList::deleteModule(const char *tID, int mc)
{
    MToken *m = mlMap[mc];
    if (m != NULL)
        deleteModule(tID, m);
}

ModuleList::~ModuleList()
{
    for (std::map<int, MToken*>::iterator iter = mlMap.begin(); iter!=mlMap.end(); iter++)
		delete iter->second;
	mlMap.empty();
}





