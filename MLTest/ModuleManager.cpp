//
//  ModuleManager.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/12/18.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "ModuleManager.h"

ModuleManager::ModuleManager(Server *s, const char *osc) : Module(s,osc)
{
    addMethodToServer("/SP/DACL", "is", dacL, this);//1:create 0:delete, tID
    addMethodToServer("/SP/DACR", "is", dacR, this);//1:create 0:delete, tID
    addMethodToServer("/GN/ADC", "is", adc, this);//1:create 0:delete, tID
	addMethodToServer("/EF/Delay", "is", delay, this);//1:create 0:delete, tID
    addMethodToServer("/GN/A1", "is", a1, this);//1:create 0:delete, tID
    addMethodToServer("/GN/A2", "is", a2, this);//1:create 0:delete, tID
    addMethodToServer("/RequestML", "i", requestML, this);
    
}

void ModuleManager::sendModuleList()
{
    int sock, n, d_len;
    struct sockaddr_in addr;
    void *data; 
    char path[] = "/ModuleList/setMList";
    char p[64];
    int  mColor;
    strcpy(p, OSCAddr);
    for (int i=0; i<6; i++) {
        switch (i) {
            case 0:
				strcpy(p, OSCAddr);
                strcat(p, "/SP/DACL");
                mColor = 1;
                break;
            case 1:
				strcpy(p, OSCAddr);
                strcat(p, "/SP/DACR");
                mColor = 1;
                break;
            case 2:
				strcpy(p, OSCAddr);
                strcat(p, "/GN/ADC");
                mColor = 2;
                break;
            case 3:
				strcpy(p, OSCAddr);
                strcat(p, "/GN/A1");
                mColor = 2;
                break;
            case 4:
				strcpy(p, OSCAddr);
                strcat(p, "/GN/A2");
                mColor = 2;
				break;
			case 5:
				strcpy(p, OSCAddr);
                strcat(p, "/EF/Delay");
                mColor = 3;
				break;
            default:
                break;
        }
        //create lo_message
        lo_message m = lo_message_new();
        lo_message_add_string(m, IPAddr);
        lo_message_add_string(m, p);
        lo_message_add_int32(m, mColor);
        
        
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
    }
    close(sock);
}

int ModuleManager::dacL(const char   *path, 
                        const char   *types, 
                        lo_arg       **argv, 
                        int          argc,
                        void         *data, 
                        void         *user_data)
{
    ModuleManager *mm = (ModuleManager *)user_data;
    char p[64] = "/ModuleManager/SP/DACL/Tile";
    strcat(p, &argv[1]->s);
    
    if (argv[0]->i) {//argv[0] = 1:モジュール生成 0:モジュール解放
        for (std::list<DAC*>::iterator iter = mm->dacLList.begin(); iter != mm->dacLList.end(); iter++) {
            DAC* dac = (*iter);
            if (strcmp(p,dac->OSCAddr)==0) {
                if (dac->tID == atoi(&argv[1]->s)) {
                    printf("err: Creating DACL\n");
                    return 0;
                }
            }
        }
        
        DAC *dac = new DAC(mm->st, p);
        dac->setTID(atoi(&argv[1]->s));
        dac->mColor = 1;
        dac->ch = 0;//Left
        dac->bs = 256;
        dac->sendSetMdtkn();
        mm->dacLList.push_back(dac);
        printf("create DACL\n");
        
    }else {
        for (std::list<DAC*>::iterator iter = mm->dacLList.begin(); iter != mm->dacLList.end(); iter++) {
            DAC* dac = (*iter);
            if (strcmp(p,dac->OSCAddr)==0) {
                dac->sendDeleteMdtkn();
                delete dac;
                mm->dacLList.remove(dac);
                printf("delete DACL\n");
            }
        }
    }
    return 0;
}

int ModuleManager::dacR(const char   *path, 
                        const char   *types, 
                        lo_arg       **argv, 
                        int          argc,
                        void         *data, 
                        void         *user_data)
{
    ModuleManager *mm = (ModuleManager *)user_data;
    char p[64] = "/ModuleManager/SP/DACR/Tile";
    strcat(p, &argv[1]->s);
    
    if (argv[0]->i) {//argv[0] = 1:モジュール生成 0:モジュール解放
        for (std::list<DAC*>::iterator iter = mm->dacRList.begin(); iter != mm->dacRList.end(); iter++) {
            DAC* dac = (*iter);
            if (strcmp(p,dac->OSCAddr)==0) {
                if (dac->tID == atoi(&argv[1]->s)) {
                    printf("err: Creating DACR\n");
                    return 0;
                }
            }
        }
        
        DAC *dac = new DAC(mm->st, p);
        dac->setTID(atoi(&argv[1]->s));
        dac->mColor = 1;
        dac->ch = 1;//Right
        dac->bs = 256;
        dac->sendSetMdtkn();
        mm->dacRList.push_back(dac);
        printf("create DACR\n");
        
    }else {
        for (std::list<DAC*>::iterator iter = mm->dacRList.begin(); iter != mm->dacRList.end(); iter++) {
            DAC* dac = (*iter);
            if (strcmp(p,dac->OSCAddr)==0) {
                dac->sendDeleteMdtkn();
                delete dac;
                mm->dacRList.remove(dac);
                printf("delete DACR\n");
            }
        }
    }
    return 0;
}

int ModuleManager::adc(const char   *path, 
                       const char   *types, 
                       lo_arg       **argv, 
                       int          argc,
                       void         *data, 
                       void         *user_data)
{
    ModuleManager *mm = (ModuleManager *)user_data;
    
    char p[64] = "/ModuleManager/GN/ADC/Tile";
    strcat(p, &argv[1]->s);
    
    if (argv[0]->i) {//argv[0] = 1:モジュール生成 0:モジュール解放
        for (std::list<ADC*>::iterator iter = mm->adcList.begin(); iter != mm->adcList.end(); iter++) {
            ADC* adc = (*iter);
            if (strcmp(p,adc->OSCAddr)==0) {
                if (adc->tID == atoi(&argv[1]->s)) {
                    printf("err: Creating ADC\n");
                    return 0;
                }
            }
        }
        
        ADC *adc = new ADC(mm->st, p);
        adc->setTID(atoi(&argv[1]->s));
        adc->mColor = 2;
        adc->sendSetMdtkn();
        mm->adcList.push_back(adc);
        printf("create ADC\n");
        
    }else {
        for (std::list<ADC*>::iterator iter = mm->adcList.begin(); iter != mm->adcList.end(); iter++) {
            ADC* adc = (*iter);
            if (strcmp(p,adc->OSCAddr)==0) {
                delete adc;
                mm->adcList.remove(adc);
                printf("delete ADC\n");
            }
        }
    }
    return 0;
}

int ModuleManager::a1(const char   *path, 
                      const char   *types, 
                      lo_arg       **argv, 
                      int          argc,
                      void         *data, 
                      void         *user_data)
{
    ModuleManager *mm = (ModuleManager *)user_data;
    
    char p[64] = "/ModuleManager/GN/A1/Tile";
    strcat(p, &argv[1]->s);
    
    if (argv[0]->i) {//argv[0] = 1:モジュール生成 0:モジュール解放
        for (std::list<AudioSource*>::iterator iter = mm->a1List.begin(); iter != mm->a1List.end(); iter++) {
            AudioSource* a1 = (*iter);
            if (strcmp(p,a1->OSCAddr)==0) {
                if (a1->tID == atoi(&argv[1]->s)) {
                    printf("err: Creating A1\n");
                    return 0;
                }
            }
        }
        
        AudioSource *a1 = new AudioSource(mm->st, p);
        a1->setTID(atoi(&argv[1]->s));
        a1->mColor = 2;
        a1->prepareAudioSource("beat.wav");
        a1->sendSetMdtkn();
        mm->a1List.push_back(a1);
        printf("create A1\n");
        
    }else {
        for (std::list<AudioSource*>::iterator iter = mm->a1List.begin(); iter != mm->a1List.end(); iter++) {
            AudioSource* a1 = (*iter);
            
            if (strcmp(p,a1->OSCAddr)==0) {
                delete a1;
                mm->a1List.remove(a1);
                printf("delete A1\n");
            }
        }
    }
    return 0;
}

int ModuleManager::a2(const char   *path, 
                      const char   *types, 
                      lo_arg       **argv, 
                      int          argc,
                      void         *data, 
                      void         *user_data)
{
    ModuleManager *mm = (ModuleManager *)user_data;
    
    char p[64] = "/ModuleManager/GN/A2/Tile";
    strcat(p, &argv[1]->s);
    
    if (argv[0]->i) {//argv[0] = 1:モジュール生成 0:モジュール解放
        for (std::list<AudioSource*>::iterator iter = mm->a2List.begin(); iter != mm->a2List.end(); iter++) {
            AudioSource* a2 = (*iter);
            if (strcmp(p,a2->OSCAddr)==0) {
                if (a2->tID == atoi(&argv[1]->s)) {
                    printf("err: Creating A2\n");
                    return 0;
                }
            }
        }
        
        AudioSource *a2 = new AudioSource(mm->st, p);
        a2->setTID(atoi(&argv[1]->s));
        a2->mColor = 2;
        a2->prepareAudioSource("pad.wav");
        a2->sendSetMdtkn();
        mm->a2List.push_back(a2);
        printf("create A2\n");
        
    }else {
        for (std::list<AudioSource*>::iterator iter = mm->a2List.begin(); iter != mm->a2List.end(); iter++) {
            AudioSource* a2 = (*iter);
            if (strcmp(p,a2->OSCAddr)==0) {
                delete a2;
                mm->a2List.remove(a2);
                printf("delete A2\n");
            }
        }
    }
    return 0;
}

int ModuleManager::delay(const char   *path, 
                         const char   *types, 
                         lo_arg       **argv, 
                         int          argc,
                         void         *data, 
                         void         *user_data)
{
	ModuleManager *mm = (ModuleManager *)user_data;
    
    char p[64] = "/ModuleManager/EF/Delay/Tile";
    strcat(p, &argv[1]->s);
    
    if (argv[0]->i) {//argv[0] = 1:モジュール生成 0:モジュール解放
        for (std::list<Delay*>::iterator iter = mm->delayList.begin(); iter != mm->delayList.end(); iter++) {
            Delay *delay = (*iter);
            if (strcmp(p,delay->OSCAddr)==0) {
                if (delay->tID == atoi(&argv[1]->s)) {
                    printf("err: Creating Delay\n");
                    return 0;
                }
            }
        }
        
        Delay *delay = new Delay(mm->st, p);
        delay->setTID(atoi(&argv[1]->s));
        delay->mColor = 3;
        delay->sendSetMdtkn();
        mm->delayList.push_back(delay);
        printf("create Delay\n");
		
    }else {
        for (std::list<Delay*>::iterator iter = mm->delayList.begin(); iter != mm->delayList.end(); iter++) {
            Delay* delay = (*iter);
            if (strcmp(p,delay->OSCAddr)==0) {
                delete delay;
                mm->delayList.remove(delay);
                printf("delete Delay\n");
            }
        }
    }
    return 0;
}

int ModuleManager::requestML(const char   *path, 
                             const char   *types, 
                             lo_arg       **argv, 
                             int          argc,
                             void         *data, 
                             void         *user_data)
{
    ModuleManager *mm = (ModuleManager *)user_data;
	mm->sendModuleList();
	return 0;
}

ModuleManager::~ModuleManager()
{
    for (std::list<DAC*>::iterator iter = dacLList.begin(); iter != dacLList.end(); iter++) {
        DAC* dac = (*iter);
        dacLList.remove(dac);
        delete dac;
    }
 
    for (std::list<DAC*>::iterator iter = dacRList.begin(); iter != dacRList.end(); iter++) {
        DAC* dac = (*iter);
        dacRList.remove(dac);
        delete dac;
    }
    
    for (std::list<ADC*>::iterator iter = adcList.begin(); iter != adcList.end(); iter++) {
        ADC* adc = (*iter);
        adcList.remove(adc);
        delete adc;
    }
    
    for (std::list<AudioSource*>::iterator iter = a1List.begin(); iter != a1List.end(); iter++) {
        AudioSource* a1 = (*iter);
        a1List.remove(a1);
        delete a1;
    }
    
    for (std::list<AudioSource*>::iterator iter = a2List.begin(); iter != a2List.end(); iter++) {
        AudioSource* a2 = (*iter);
        a2List.remove(a2);
        delete a2;
    }
    
    for (std::list<Delay*>::iterator iter = delayList.begin(); iter != delayList.end(); iter++) {
        Delay* delay = (*iter);
        delayList.remove(delay);
        delete delay;
    }
    
	deleteMethodFromServer("/SP/DACL", "is");
    deleteMethodFromServer("/SP/DACR", "is");
    deleteMethodFromServer("/GN/ADC", "is");
    deleteMethodFromServer("/GN/A1", "is");
    deleteMethodFromServer("/GN/A2", "is");
	deleteMethodFromServer("/EF/Delay", "is");
	deleteMethodFromServer("/RequestML", "i");
}


