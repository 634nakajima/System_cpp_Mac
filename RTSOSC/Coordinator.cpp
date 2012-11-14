//
//  Coordinator.cpp
//  SimpleSimulator
//
//  Created by NAKAJIMA Musashi on 12/06/18.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Coordinator.h"

Coordinator::Coordinator(Server *s, const char *osc) : Module(s,osc)
{
    addMethodToServer("/SetMdtkn", "ssii", setMtkn, this);//ip,osc,tID,mColor
    addMethodToServer("/deleteMdtkn", "ssii", deleteMtkn, this);//ip,osc,tID,mColor

    xbc = NULL;
    ml = new ModuleList(s, "/ModuleList");
}

int Coordinator::setMtkn(const char   *path, 
                         const char   *types, 
                         lo_arg       **argv, 
                         int          argc,
                         void         *data, 
                         void         *user_data)
{
    Coordinator *co = (Coordinator *)user_data;
    //エラー処理、既存のモジュールトークン確認
    for (std::map<int, MToken*>::iterator iter = co->mtknMap.begin(); iter!=co->mtknMap.end(); iter++) {
        MToken *tmp = iter->second;
        if (strcmp(tmp->ip,(char *)argv[0])==0) {
            if (strcmp(tmp->osc,(char *)argv[1])==0) {
                return 0;
            }
        }
    }

    //先にタイルに登録されているモジュールの確認
    for (std::map<int, MToken*>::iterator iter = co->mtknMap.begin(); iter!=co->mtknMap.end(); iter++) {
        MToken *tmp = iter->second;
        if (tmp->tID == argv[2]->i) {//先に登録されていればそのモジュールを消去
            co->deleteMtkn(tmp->tID);
            break;
        }
    }
    //モジュールトークンの生成
    MToken *m = new MToken();
    strcpy(m->ip, (char *)argv[0]);
    strcpy(m->osc, (char *)argv[1]);

    if (argv[2]->i != -1) {//tIDが-1でなければ
        m->tID = argv[2]->i;
        m->mColor = argv[3]->i;

        co->mtknMap.insert(std::map<int, MToken*>::value_type(m->tID, m));
		printf("set:%s, %s, TileID:%d, Module Color:%d\n",
               (char *)argv[0], 
               (char *)argv[1], 
               argv[2]->i, 
               argv[3]->i);
        
        if (co->xbc)//XBCでタイルにモジュールを登録
            co->xbc->setAlive(m->tID, m->mColor);
    }
    
    return 0;
}

void Coordinator::deleteMtkn(int tID)
{
	if (!mtknMap.count(tID)) return;
	
	ml->deleteModule(mtknMap[tID]->tID, mtknMap[tID]->mColor);
	printf("delete:%s,%s tID:%d Module Color:%d\n",
		   mtknMap[tID]->ip, 
		   mtknMap[tID]->osc, mtknMap[tID]->tID, 
		   mtknMap[tID]->mColor);
    
	delete mtknMap[tID];
	mtknMap.erase(tID);
}

int Coordinator::deleteMtkn(const char   *path,
                            const char   *types, 
                            lo_arg       **argv, 
                            int          argc,
                            void         *data, 
                            void         *user_data)
{
    Coordinator *co = (Coordinator *)user_data;
    co->deleteMtkn(argv[2]->i);
    
    return 0;
}

void Coordinator::connect(int tID1, int tID2, const char *t)
{
    char m1OSC[64], m2OSC[64];
    
	//エラー処理
    if (!mtknMap.count(tID1) || !mtknMap.count(tID2)) {
        printf("err:connect\n");
        return;  
    }
	
    //モジュールトークン取得
    MToken *m1 = mtknMap[tID1];
    MToken *m2 = mtknMap[tID2];
    

    
    //モジュールに対して接続するルートのアドレスを送信
    strcpy(m1OSC, m1->osc);
    strcpy(m2OSC, m2->osc);
    
    lo_send(lo_address_new(m1->ip,"6340"), 
            strcat(m1OSC,"/SetRoute"),
            "ss", 
            m2->ip,
            strcat(m2OSC,t));
}

void Coordinator::disconnect(int tID1, int tID2, const char *t)
{
    char m1OSC[64], m2OSC[64];
    
	//エラー処理
	if (!mtknMap.count(tID1) || !mtknMap.count(tID2)) {
        //printf("err:disconnect\n");
        return;  
    }
	
    //モジュールトークン取得
    MToken *m1 = mtknMap[tID1];
    MToken *m2 = mtknMap[tID2];
    
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
	for (std::map<int, MToken*>::iterator iter = mtknMap.begin(); iter!=mtknMap.end(); iter++)
		delete iter->second;

    mtknMap.clear();
    delete ml;

}





