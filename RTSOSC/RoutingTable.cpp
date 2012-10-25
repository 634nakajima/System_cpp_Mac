//
//  RoutingTable.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/05/27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include "RoutingTable.h"

RoutingTable::RoutingTable()
{
    aNum = 0;
    
    for (int i=0; i<MAX_ANUM; i++) {
        oscAddr[i] = (char *)malloc(sizeof(char)*64);//メモリ確保
        memset(oscAddr[i], 0, sizeof(char)*64);//初期化
		loAddr[i] = NULL;
    }
}

void RoutingTable::setRoute(char *ip, char *osc)
{    
    //エラー処理
    if (aNum > MAX_ANUM-1) return;
    //既存のアドレス確認
    for (int i=0; i<aNum; i++) {
        const char *addr = lo_address_get_hostname(loAddr[i]);
        
        if (strcmp(ip, addr) == 0) {
            if (strcmp(osc, oscAddr[i]) == 0) {
                return;
            }
        }
    }
    //既存アドレスに無ければアドレス生成
    loAddr[aNum] = lo_address_new(ip, "6340");
    strcpy(oscAddr[aNum], osc);
    aNum++;
}

void RoutingTable::deleteRoute(char *ip, char *osc)
{
    for (int i=0; i<aNum; i++) {
        const char *addr = lo_address_get_hostname(loAddr[i]);
        
        //対象のアドレス検索
        if (strcmp(ip, addr)==0 && strcmp(osc, oscAddr[i])==0) {
                
            //対象のアドレス解放、アドレス数デクリメント
            lo_address_free(loAddr[i]);
            memset(oscAddr[i], 0, sizeof(char)*64);
            aNum--;
            
            //ルートのアドレスをずらす
            for (int j=i; j<aNum; j++) {
                loAddr[j] = loAddr[j+1];
                strcpy(oscAddr[j], oscAddr[j+1]);
            }
            loAddr[aNum] = NULL;
            memset(oscAddr[aNum], 0, sizeof(char)*64);
            
            //ループから出る
            break;
        }
    }
}

RoutingTable::~RoutingTable()
{
    for (int i=0; i<MAX_ANUM; i++) {
        if (loAddr[i] != NULL) lo_address_free(loAddr[i]);
        free(oscAddr[i]);
    }
}