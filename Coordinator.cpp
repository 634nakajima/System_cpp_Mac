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
    mID = 0;
    mNum = 0;
    addMethodToServer("/SetMdtkn", "ss", setMtkn, this);
    addMethodToServer("/deleteMdtkn", "ss", deleteMtkn, this);

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
    for (i=0; i<coordinator->mNum; i++) {
        if (strcmp(coordinator->mtknMap[i]->ip,(char *)argv[0])==0)
            if (strcmp(coordinator->mtknMap[i]->osc,(char *)argv[1])==0) return 0;
    }
    
    //モジュールトークンの生成、トークン数インクリメント
    MToken *m = new MToken();
    strcpy(m->ip, (char *)argv[0]);
    strcpy(m->osc, (char *)argv[1]);
    for (i=0; i<coordinator->mNum; i++) {
        MToken *tmp = coordinator->mtknMap[i];
        if (tmp == NULL) {
            m->index = i;
            break;
        }
    }
    if (i == coordinator->mNum) {
        m->index = coordinator->mNum;
        coordinator->mNum++;
    }
    coordinator->mtknMap.insert(std::map<int, MToken*>::value_type(m->index, m));
    printf("set:%s,%s,mID:%d\n",(char *)argv[0], (char *)argv[1], coordinator->mNum);

    
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
    
    //対象のトークン検索
    for (int i=0; i<coordinator->mNum; i++) {
        MToken *m = coordinator->mtknMap[i];
        if (strcmp(m->ip,(char *)argv[0])==0) {
            if (strcmp(m->osc,(char *)argv[1])==0) {
                
                //モジュールトークン解放
                coordinator->mtknMap.erase(i);
                printf("delete:%s,%s mID:%d\n",(char *)argv[0], (char *)argv[1], i);
                delete m;
                
                return 0;
            }
        }
    }

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
    for (int i=0; i<mNum; i++) {
        MToken *m = mtknMap[i];
        delete m;
    }
    mtknMap.empty();
}





