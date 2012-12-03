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

ModuleList::ModuleList(Server *s, const char *osc) : Module(s,osc)
{
    addMethodToServer("/setMList", "ssi", setMList, this);
	addMethodToServer("/Stream", "b", stream, this);
}

int ModuleList::setMList(const char   *path, 
                         const char   *types, 
                         lo_arg       **argv, 
                         int          argc,
                         void         *data,
                         void         *user_data)
{
	printf("!!\n");
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
        m->mColor = argv[2]->i;
        strcpy(m->ip, (char *)argv[0]);
        strcpy(m->osc, (char *)argv[1]);
        mlc->mlMap.insert(std::map<int, MToken*>::value_type(m->mColor, m));
    
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
	return 0;

}

void ModuleList::createModule(char *tID, MToken *ml)
{
    lo_send(lo_address_new(ml->ip,"6340"), 
            ml->osc,
            "is", 
            1,
            tID);
}

void ModuleList::deleteModule(char *tID, MToken *ml)
{
    lo_send(lo_address_new(ml->ip,"6340"), 
            ml->osc,
            "is", 
            0,
            tID);
}

void ModuleList::createModule(char *tID, int mc)
{
	if (!mlMap.count(mc)) return;
	
    MToken *m = mlMap[mc];
	createModule(tID, m);
}

void ModuleList::deleteModule(char *tID, int mc)
{
	if (!mlMap.count(mc)) return;

    MToken *m = mlMap[mc];
	deleteModule(tID, m);
}

void ModuleList::createModule(int tID, int mc)
{
	if (!mlMap.count(mc)) return;

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
    
    MToken *m = mlMap[mc];
	createModule(t, m);
}

void ModuleList::deleteModule(int tID, int mc)
{
	if (!mlMap.count(mc)) return;

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

    MToken *m = mlMap[mc];
	deleteModule(t, m);
}

void ModuleList::requestML()
{
	int sock, n, d_len;
    struct sockaddr_in addr;
    void *data; 
    char path[] = "/ModuleManager/requestML";
    char p[64];
    int  mColor;
    strcpy(p, OSCAddr);
	
	//create lo_message
	lo_message m = lo_message_new();
	lo_message_add_int32(m, 0);
	
	
	data = lo_message_serialise(m, path, NULL, NULL);
	d_len = lo_message_length(m, path);
	
	//create socket
	int opt = 1;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(int));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6340);
	inet_pton(AF_INET, "255.255.255.255", &addr.sin_addr.s_addr);
	
	//send(念のため3回)
	for (int j=0; j<3; j++) {
		n = sendto(sock, data, d_len, 0, (struct sockaddr *)&addr, sizeof(addr));
		if (n < 1) {
			perror("sendto");
		}
		usleep(1000);
	}
	
	lo_message_free(m);
    close(sock);	
}

ModuleList::~ModuleList()
{
    for (std::map<int, MToken*>::iterator iter = mlMap.begin(); iter!=mlMap.end(); iter++)
		delete iter->second;
	mlMap.empty();
	deleteMethodFromServer("/setMList", "ssi");
	deleteMethodFromServer("/Stream", "b");
}





