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
    addMethodToServer("/DeleteMdtkn", "ssii", deleteMtkn, this);//ip,osc,tID,mColor
	addMethodToServer("/TileState", "iss", tileState, this);//tID

    xbc = NULL;
    ml = new ModuleList(s, "/ModuleList");
}

int Coordinator::tileState(const char   *path, 
							const char   *types, 
							lo_arg       **argv, 
							int          argc,
							void         *data, 
							void         *user_data)
{
    Coordinator *co = (Coordinator *)user_data;
	
	for (std::map<int, MToken*>::iterator iter = co->mtknMap.begin(); iter!=co->mtknMap.end(); iter++) {
        MToken *tmp = iter->second;
        if (tmp->tID == argv[0]->i) {
			for (int i; i<3; i++) {
				lo_send(lo_address_new((char *)argv[1],"6340"), 
						(char *)argv[2],
						"ssii", 
						tmp->ip,
						tmp->osc,
						tmp->tID,
						tmp->mColor);
				usleep(10000);
			}
            return 0;
        }
    }
	
    for (int i; i<3; i++) {
		lo_send(lo_address_new((char *)argv[1],"6340"), 
				(char *)argv[2],
				"ssi", 
				NULL,
				NULL,
				0);
		usleep(10000);
	}
    return 0;
}

int Coordinator::setMtkn(const char   *path, 
                         const char   *types, 
                         lo_arg       **argv, 
                         int          argc,
                         void         *data, 
                         void         *user_data)
{
    Coordinator *co = (Coordinator *)user_data;
	
	lo_server_thread st = (lo_server_thread )co->st->st;
	lo_server s = lo_server_thread_get_server(st);
	struct sockaddr_in *sin = (struct sockaddr_in *)lo_server_get_addr(s);
	printf("itsumono:%s, s->addr:%s\n",
		   (char *)argv[0], 
		   inet_ntoa(sin->sin_addr));
	
	char ip[16];//このあとの"ip"は"(char *)argv[0]"から変更しましたよ。
	strcpy(ip, inet_ntoa(sin->sin_addr));
	
    //エラー処理、既存のモジュールトークン確認
    for (std::map<int, MToken*>::iterator iter = co->mtknMap.begin(); iter!=co->mtknMap.end(); iter++) {
        MToken *tmp = iter->second;
        if (strcmp(tmp->ip,ip)==0) {
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
    strcpy(m->ip, ip);
    strcpy(m->osc, (char *)argv[1]);

    if (argv[2]->i != -1) {//tIDが-1でなければ
        m->tID = argv[2]->i;
        m->mColor = argv[3]->i;

        co->mtknMap.insert(std::map<int, MToken*>::value_type(m->tID, m));
		printf("set:%s, %s, TileID:%d, Module Color:%d\n",
               ip, 
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
	
	char t[4];
    if (tID < 10) {
        t[0] = (tID + 0x30);
        t[1] = '\0';
    }else if (tID < 100) {
        t[0] = (tID/10 + 0x30);
        t[1] = (tID%10 + 0x30);
        t[2] = '\0';
    }else {
        t[0] = '1';
        t[1] = ((tID%100)/10 + 0x30);
        t[2] = ((tID%100)%10 + 0x30);
        t[3] = '\0';
    }
	char p[64];
	char *pp, *op;
	
	pp = p;
	op = mtknMap[tID]->osc;
	
	char *end = strstr(mtknMap[tID]->osc, "/Tile");
	
	while (op != end) {
		*pp++ = *op++;
	}
	*pp = '\0';
	printf("%s\n",p);
	
	lo_send(lo_address_new(mtknMap[tID]->ip,"6340"), 
            p,
            "is", 
            0,
            t);
	
	printf("delete:%s,%s tID:%d Module Color:%d\n",
		   mtknMap[tID]->ip, 
		   p, mtknMap[tID]->tID, 
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
        printf("err:connect tid1:%d, tid2:%d\n",tID1, tID2);
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

void Coordinator::addConnection(int tID1, int tID2, const char *t)
{
    char m1OSC[64], m2OSC[64];
    
	//エラー処理
    if (!mtknMap.count(tID1) || !mtknMap.count(tID2)) {
        printf("err:connect tid1:%d, tid2:%d\n",tID1, tID2);
        return;  
    }
	
    //モジュールトークン取得
    MToken *m1 = mtknMap[tID1];
    MToken *m2 = mtknMap[tID2];
    
    //モジュールに対して接続するルートのアドレスを送信
    strcpy(m1OSC, m1->osc);
    strcpy(m2OSC, m2->osc);
    
    lo_send(lo_address_new(m1->ip,"6340"), 
            strcat(m1OSC,"/AddRoute"),
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

void Coordinator::disconnectAll(int tID, const char *t)
{
    char m1OSC[64];
    
	//エラー処理
	if (!mtknMap.count(tID)) {
        //printf("err:disconnect\n");
        return;  
    }
	
    //モジュールトークン取得
    MToken *m1 = mtknMap[tID];
    
    //モジュールに対して切断するルートのアドレスを送信
    strcpy(m1OSC, m1->osc);
    
    lo_send(lo_address_new(m1->ip,"6340"), 
            strcat(m1OSC,"/DeleteAllRoute"),
            "s", 
            t);
}

Coordinator::~Coordinator()
{
	for (std::map<int, MToken*>::iterator iter = mtknMap.begin(); iter!=mtknMap.end(); iter++)
		delete iter->second;

    mtknMap.clear();
    delete ml;

	deleteMethodFromServer("/SetMdtkn", "ssii");
    deleteMethodFromServer("/DeleteMdtkn", "ssii");
	deleteMethodFromServer("/TileState", "iss");

}





