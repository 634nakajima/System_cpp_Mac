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
    addMethodToServer("/DAC", "is", dac, this);
    addMethodToServer("/ADC", "is", adc, this);
    addMethodToServer("/Sine", "is", sine, this);
    addMethodToServer("/Envelope", "is", env, this);
    addMethodToServer("/AudioSource", "is", as, this);

    for (int i=0; i<5; i++) {
        mn[i] = 0;
    }
}

void ModuleController::sendModuleList()
{
    int sock, n, d_len;
    struct sockaddr_in addr;
    void *data; 
    char path[] = "/Coordinator/ModuleList";
    char p[64];
    
    strcpy(p, OSCAddr);
    
    for (int i=0; i<5; i++) {
        switch (i) {
            case 0:
                strcat(p, "/DAC");
                break;
            case 1:
                strcat(p, "/ADC");
                break;
            case 2:
                strcat(p, "/Sine");
                break;
            case 3:
                strcat(p, "/Envelope");
                break;
            case 4:
                strcat(p, "/AudioSource");
            default:
                break;
        }
        
        //create lo_message
        lo_message m = lo_message_new();
        lo_message_add_string(m, IPAddr);
        lo_message_add_string(m, p);
    
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
        for (int j=0; j<2; i++) {
            n = sendto(sock, data, d_len, 0, (struct sockaddr *)&addr, sizeof(addr));
            if (n < 1) {
                perror("sendto");
            }
            usleep(1000);
        }
    }
    close(sock);
}

static int dac(const char   *path, 
               const char   *types, 
               lo_arg       **argv, 
               int          argc,
               void         *data, 
               void         *user_data)
{
    ModuleController *mc = (ModuleController *)user_data;

    char p[64];
    
    strcpy(p, (char *)argv[1]);
    strcat(p, "/SP/DAC");
    
    if ((int)argv[0]) {//argv[0] = 1:モジュール生成 0:モジュール解放
        DAC *dac = new DAC(mc->st, p));
        mc->dacList->push_back(dac);
    }else {
        for (std::list<DAC*>::iterator iter = dacList.begin(); iter!=dacList.end(); iter++) {
            DAC* dac = (*iter);
            if (strcmp(p,dac->OSCAddr)==0) {
                mc->dacList->remove(dac);
                delete dac;
            }
        }
    }
}


static int adc(const char   *path, 
               const char   *types, 
               lo_arg       **argv, 
               int          argc,
               void         *data, 
               void         *user_data)
{
    ModuleController *mc = (ModuleController *)user_data;
    
    char p[64];
    
    strcpy(p, (char *)argv[1]);
    strcat(p, "/GN/ADC");
    
    if ((int)argv[0]) {//argv[0] = 1:モジュール生成 0:モジュール解放
        ADC *adc = new ADC(mc->st, p));
        mc->adcList->push_back(adc);
    }else {
        for (std::list<ADC*>::iterator iter = adcList.begin(); iter!adcList.end(); iter++) {
            ADC* adc = (*iter);
            if (strcmp(p,adc->OSCAddr)==0) {
                mc->adcList->remove(adc);
                delete adc;
            }
        }
    }
}

static int sine(const char   *path, 
                const char   *types, 
                lo_arg       **argv, 
                int          argc,
                void         *data, 
                void         *user_data)
{
    ModuleController *mc = (ModuleController *)user_data;
    
    char p[64];
    
    strcpy(p, (char *)argv[1]);
    strcat(p, "/GN/Sine");
    
    if ((int)argv[0]) {//argv[0] = 1:モジュール生成 0:モジュール解放
        Sine *sine = new Sine(mc->st, p));
        mc->sineList->push_back(sine);
    }else {
        for (std::list<Sine*>::iterator iter = sineList.begin(); iter!=sineList.end(); iter++) {
            Sine* sine = (*iter);
            if (strcmp(p,sine->OSCAddr)==0) {
                mc->sineList->remove(sine);
                delete sine;
            }
        }
    }
}

static int env(const char   *path, 
               const char   *types, 
               lo_arg       **argv, 
               int          argc,
               void         *data, 
               void         *user_data)
{
    ModuleController *mc = (ModuleController *)user_data;
    
    char p[64];
    
    strcpy(p, (char *)argv[1]);
    strcat(p, "/EF/Envelope");
    
    if ((int)argv[0]) {//argv[0] = 1:モジュール生成 0:モジュール解放
        Envelope *env = new Envelope(mc->st, p));
        mc->envList->push_back(env);
    }else {
        for (std::list<Envelope*>::iterator iter = envList.begin(); iter!=envList.end(); iter++) {
            Envelope* env = (*iter);
            if (strcmp(p,env->OSCAddr)==0) {
                mc->envList->remove(env);
                delete env;
            }
        }
    }
}

static int as(const char   *path, 
              const char   *types, 
              lo_arg       **argv, 
              int          argc,
              void         *data, 
              void         *user_data)
{
    ModuleController *mc = (ModuleController *)user_data;
    
    char p[64];
    
    strcpy(p, (char *)argv[1]);
    strcat(p, "/GN/AudioSource");
    
    if ((int)argv[0]) {//argv[0] = 1:モジュール生成 0:モジュール解放
        AudioSource *as = new AudioSource(mc->st, p));
        mc->asList->push_back(as);
    }else {
        for (std::list<AudioSource*>::iterator iter = asList.begin(); iter!=asList.end(); iter++) {
            AudioSource* as = (*iter);
            if (strcmp(p,as->OSCAddr)==0) {
                mc->asList->remove(as);
                delete as;
            }
        }
    }
}

ModuleController::~ModuleController()
{
    for (std::list<DAC*>::iterator iter = dacList.begin(); iter!=dacList.end(); iter++) {
        DAC* dac = (*iter);
        dacList->remove(dac);
        delete dac;
    }

    for (std::list<ADC*>::iterator iter = adcList.begin(); iter!adcList.end(); iter++) {
        ADC* adc = (*iter);
        mc->adcList->remove(adc);
        delete adc;
    }

    for (std::list<Sine*>::iterator iter = sineList.begin(); iter!=sineList.end(); iter++) {
        Sine* sine = (*iter);
        mc->sineList->remove(sine);
        delete sine;
    }
    
    for (std::list<Envelope*>::iterator iter = envList.begin(); iter!=envList.end(); iter++) {
        Envelope* env = (*iter);
        mc->envList->remove(env);
        delete env;
    }
    
    for (std::list<AudioSource*>::iterator iter = asList.begin(); iter!=asList.end(); iter++) {
        AudioSource* as = (*iter);
        mc->asList->remove(as);
        delete as;
    }
}
