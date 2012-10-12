//
//  Coordinator.cpp
//  SimpleSimulator
//
//  Created by NAKAJIMA Musashi on 12/06/18.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Coordinator.h"

Coordinator::Coordinator(lo_server_thread s, const char *osc) : Module(s,osc)
{
    addMethodToServer("/SetMdtkn", "ssii", setMtkn, this);
    addMethodToServer("/deleteMdtkn", "ssii", deleteMtkn, this);
    addMethodToServer("/ModuleList", "ss", setMList, this);

}

int Coordinator::setMtkn(const char   *path, 
                         const char   *types, 
                         lo_arg       **argv, 
                         int          argc,
                         void         *data, 
                         void         *user_data)
{
    int i = 0;
    Coordinator *coordinator = (Coordinator *)user_data;
    //エラー処理、既存のモジュールトークン確認
    if (coordinator->mNum >= 32) return 0;
    for (std::map<int, MToken*>::iterator iter = coordinator->mtknMap.begin(); iter!=coordinator->mtknMap.end(); iter++) {
        MToken *tmp = iter->second;
        if (strcmp(tmp->ip,(char *)argv[0])==0) {
            if (strcmp(tmp->osc,(char *)argv[1])==0) return 0;
        }
    }
    
    //モジュールトークンの生成
    MToken *m = new MToken();
    strcpy(m->ip, (char *)argv[0]);
    strcpy(m->osc, (char *)argv[1]);
    
    if (argv[2]->i != -1) {//mIDが-1でなければ
        m->index = argv[2]->i;
        coordinator->mtknMap.insert(std::map<int, MToken*>::value_type(m->index, m));
		printf("set:%s,%s,mID:%d\n",(char *)argv[0], (char *)argv[1], argv[2]->i);
    }
    
    return 0;
}

int Coordinator::deleteMtkn(const char   *path,
                            const char   *types, 
                            lo_arg       **argv, 
                            int          argc,
                            void         *data, 
                            void         *user_data)
{
    Coordinator *coordinator = (Coordinator *)user_data;
    
    for (std::map<int, MToken*>::iterator iter = coordinator->mtknMap.begin(); iter!=coordinator->mtknMap.end(); iter++) {
        MToken *tmp = iter->second;
        if (strcmp(tmp->ip,(char *)argv[0])==0) {
            if (strcmp(tmp->osc,(char *)argv[1])==0) {
                if (argv[2]->i == tmp->index) {
                    coordinator->mtknMap.erase(argv[2]->i);
                    delete tmp;
                    printf("delete:%s,%s mID:%d\n",(char *)argv[0], (char *)argv[1], argv[2]->i);
                    return 0;
                }
            }
        }
    }
    return 1;
}

int Coordinator::setMList(const char   *path, 
             const char   *types, 
             lo_arg       **argv, 
             int          argc,
             void         *data, 
             void         *user_data)
{
    int i = 0;
    Coordinator *coordinator = (Coordinator *)user_data;
    //エラー処理、既存のモジュールリスト確認
    for (std::list<MToken*>::iterator iter = coordinator->mList.begin(); iter!=coordinator->mList.end(); iter++) {
        MToken* ml = (*iter);
        if (strcmp(ml->ip, (char *)argv[0])==0) {
            if (strcmp(ml->osc, (char *)argv[1])==0) return 0;
        }
    }
    
    //モジュールリストの生成
    MToken *m = new MToken();
    strcpy(m->ip, (char *)argv[0]);
    strcpy(m->osc, (char *)argv[1]);
    coordinator->mList.push_back(m);
    
    printf("set:%s,%s\n",(char *)argv[0], (char *)argv[1]);
    
    
    return 0;

}

void Coordinator::connect(int mID1, int mID2, const char *t)
{
    char m1OSC[64], m2OSC[64];
    
    //モジュールトークン取得
    MToken *m1 = mtknMap[mID1];
    MToken *m2 = mtknMap[mID2];
    
    //エラー処理
    if ( m1 == NULL || m2 == NULL ) {
        printf("err:connect\n");
        return;  
    }
    
    //モジュールに対して接続するルートのアドレスを送信
    strcpy(m1OSC, m1->osc);
    strcpy(m2OSC, m2->osc);
    
    lo_send(lo_address_new(m1->ip,"6340"), 
            strcat(m1OSC,"/SetRoute"),
            "ss", 
            m2->ip,
            strcat(m2OSC,t));
}

void Coordinator::disconnect(int mID1, int mID2, const char *t)
{
    char m1OSC[64], m2OSC[64];
    
    //モジュールトークン取得
    MToken *m1 = mtknMap[mID1];
    MToken *m2 = mtknMap[mID2];
    
    //エラー処理
    if ( m1 == NULL || m2 == NULL ) {
        printf("err:disconnect\n");
        return;  
    }
    
    //モジュールに対して切断するルートのアドレスを送信
    strcpy(m1OSC, m1->osc);
    strcpy(m2OSC, m2->osc);
    
    lo_send(lo_address_new(m1->ip,"6340"), 
            strcat(m1OSC,"/DeleteRoute"),
            "ss", 
            m2->ip,
            strcat(m2OSC,t));
}

Coordinator::~Coordinator()
{
    for (std::list<MToken*>::iterator iter = mList.begin(); iter!=mList.end(); iter++)
		delete (*iter);
	for (std::map<int, MToken*>::iterator iter = mtknMap.begin(); iter!=mtknMap.end(); iter++)
		delete iter->second;

    mtknMap.empty();
	mList.clear();

}





