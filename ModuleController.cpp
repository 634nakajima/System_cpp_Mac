//
//  ModuleController.cpp
//  SimpleSimulator
//
//  Created by NAKAJIMA Musashi on 12/09/30.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "ModuleController.h"

ModuleController::ModuleController(lo_server_thread s, const char *osc) : Module(s,osc)
{
    addMethodToServer("/SP/DAC", "is", dac, this);
    addMethodToServer("/GN/ADC", "is", adc, this);
    addMethodToServer("/GN/Sine", "is", sine, this);
    addMethodToServer("/EF/Envelope", "is", env, this);
    addMethodToServer("/GN/AudioSource", "is", as, this);

}

void ModuleController::sendModuleList()
{
    int sock, n, d_len;
    struct sockaddr_in addr;
    void *data; 
    char path[] = "/ModuleList/setMList";
    char p[64];
    int  mColor;
    strcpy(p, OSCAddr);
    for (int i=0; i<5; i++) {
        switch (i) {
            case 0:
				strcpy(p, OSCAddr);
                strcat(p, "/SP/DAC");
                mColor = 1;
                break;
            case 1:
				strcpy(p, OSCAddr);
                strcat(p, "/GN/ADC");
                mColor = 2;
                break;
            case 2:
				strcpy(p, OSCAddr);
                strcat(p, "/GN/Sine");
                mColor = 3;
                break;
            case 3:
				strcpy(p, OSCAddr);
                strcat(p, "/EF/Envelope");
                mColor = 4;
                break;
            case 4:
				strcpy(p, OSCAddr);
                strcat(p, "/GN/AudioSource");
                mColor = 5;
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

        //send(念のため2回)
        for (int j=0; j<2; j++) {
            n = sendto(sock, data, d_len, 0, (struct sockaddr *)&addr, sizeof(addr));
            if (n < 1) {
                perror("sendto");
            }
            usleep(1000);
        }
    }
    close(sock);
}

int ModuleController::dac(const char   *path, 
        const char   *types, 
        lo_arg       **argv, 
        int          argc,
        void         *data, 
        void         *user_data)
{
    ModuleController *mc = (ModuleController *)user_data;

    char p[64] = "/Tile";

    strcat(p, &argv[1]->s);
    strcat(p, "/SP/DAC");
    
    if (argv[0]->i) {//argv[0] = 1:モジュール生成 0:モジュール解放
        DAC *dac = new DAC(mc->st, p);
        dac->setMID(atoi(&argv[1]->s));
        dac->sendSetMdtkn();
        mc->dacList.push_back(dac);
    }else {
        for (std::list<DAC*>::iterator iter = mc->dacList.begin(); iter != mc->dacList.end(); iter++) {
            DAC* dac = (*iter);
            if (strcmp(p,dac->OSCAddr)==0) {
                dac->sendDeleteMdtkn();
                mc->dacList.remove(dac);
                delete dac;
            }
        }
    }
}


int ModuleController::adc(const char   *path, 
        const char   *types, 
        lo_arg       **argv, 
        int          argc,
        void         *data, 
        void         *user_data)
{
    ModuleController *mc = (ModuleController *)user_data;
    
    char p[64] = "/Tile";
    
    strcat(p, &argv[1]->s);
    strcat(p, "/GN/ADC");
    
    if (argv[0]->i) {//argv[0] = 1:モジュール生成 0:モジュール解放
        ADC *adc = new ADC(mc->st, p);
        adc->setMID(atoi(&argv[1]->s));
        adc->sendSetMdtkn();
        mc->adcList.push_back(adc);
    }else {
        for (std::list<ADC*>::iterator iter = mc->adcList.begin(); iter != mc->adcList.end(); iter++) {
            ADC* adc = (*iter);
            if (strcmp(p,adc->OSCAddr)==0) {
                adc->sendDeleteMdtkn();
                mc->adcList.remove(adc);
                delete adc;
            }
        }
    }
}

int ModuleController::sine(const char   *path, 
         const char   *types, 
         lo_arg       **argv, 
         int          argc,
         void         *data, 
         void         *user_data)
{
    ModuleController *mc = (ModuleController *)user_data;
    
    char p[64] = "/Tile";
    
    strcat(p, &argv[1]->s);
    strcat(p, "/GN/Sine");
    
    if (argv[0]->i) {//argv[0] = 1:モジュール生成 0:モジュール解放
        Sine *sine = new Sine(mc->st, p);
        sine->setMID(atoi(&argv[1]->s));
        sine->sendSetMdtkn();
        mc->sineList.push_back(sine);
    }else {
        for (std::list<Sine*>::iterator iter = mc->sineList.begin(); iter != mc->sineList.end(); iter++) {
            Sine* sine = (*iter);
            if (strcmp(p,sine->OSCAddr)==0) {
                sine->sendDeleteMdtkn();
                mc->sineList.remove(sine);
                delete sine;
            }
        }
    }
}

int ModuleController::env(const char   *path, 
        const char   *types, 
        lo_arg       **argv, 
        int          argc,
        void         *data, 
        void         *user_data)
{
    ModuleController *mc = (ModuleController *)user_data;
    
    char p[64] = "/Tile";
    
    strcat(p, &argv[1]->s);
    strcat(p, "/EF/Envelope");
    
    if (argv[0]->i) {//argv[0] = 1:モジュール生成 0:モジュール解放
        Envelope *env = new Envelope(mc->st, p);
        env->setMID(atoi(&argv[1]->s));
        env->sendSetMdtkn();
        mc->envList.push_back(env);
    }else {
        for (std::list<Envelope*>::iterator iter = mc->envList.begin(); iter != mc->envList.end(); iter++) {
            Envelope* env = (*iter);
            if (strcmp(p,env->OSCAddr)==0) {
                env->sendDeleteMdtkn();
                mc->envList.remove(env);
                delete env;
            }
        }
    }
}

int ModuleController::as(const char   *path, 
       const char   *types, 
       lo_arg       **argv, 
       int          argc,
       void         *data, 
       void         *user_data)
{
    ModuleController *mc = (ModuleController *)user_data;
    
    char p[64] = "/Tile";
    
    strcat(p, &argv[1]->s);
    strcat(p, "/GN/AudioSource");
    
    if (argv[0]->i) {//argv[0] = 1:モジュール生成 0:モジュール解放
        AudioSource *as = new AudioSource(mc->st, p);
        as->setMID(atoi(&argv[1]->s));
        as->sendSetMdtkn();
        mc->asList.push_back(as);
    }else {
        for (std::list<AudioSource*>::iterator iter = mc->asList.begin(); iter != mc->asList.end(); iter++) {
            AudioSource* as = (*iter);
            if (strcmp(p,as->OSCAddr)==0) {
                as->sendDeleteMdtkn();
                mc->asList.remove(as);
                delete as;
            }
        }
    }
}

ModuleController::~ModuleController()
{
    for (std::list<DAC*>::iterator iter = dacList.begin(); iter != dacList.end(); iter++) {
        DAC* dac = (*iter);
        dacList.remove(dac);
        delete dac;
    }

    for (std::list<ADC*>::iterator iter = adcList.begin(); iter != adcList.end(); iter++) {
        ADC* adc = (*iter);
        adcList.remove(adc);
        delete adc;
    }

    for (std::list<Sine*>::iterator iter = sineList.begin(); iter != sineList.end(); iter++) {
        Sine* sine = (*iter);
        sineList.remove(sine);
        delete sine;
    }
    
    for (std::list<Envelope*>::iterator iter = envList.begin(); iter != envList.end(); iter++) {
        Envelope* env = (*iter);
        envList.remove(env);
        delete env;
    }
    
    for (std::list<AudioSource*>::iterator iter = asList.begin(); iter != asList.end(); iter++) {
        AudioSource* as = (*iter);
        asList.remove(as);
        delete as;
    }
}

