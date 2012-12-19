//
//  Module.cpp
//  System1
//
//  Created by NAKAJIMA Musashi on 12/05/27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include "Module.h"

void Module::setTID(int t)
{
    tID = t;
}

int Module::setRoute(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data)
{
    Module *mod = (Module *)user_data;
    mod->setRoute((char *)argv[0], (char *)argv[1]);

    return 0;
}

int Module::addRoute(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data)
{
    Module *mod = (Module *)user_data;
    mod->addRoute((char *)argv[0], (char *)argv[1]);
    
    return 0;
}

int Module::deleteRoute(const char   *path, 
                        const char   *types, 
                        lo_arg       **argv, 
                        int          argc,
                        void         *data, 
                        void         *user_data)
{
    Module *mod = (Module *)user_data;
    mod->deleteRoute((char *)argv[0], (char *)argv[1]);
    return 0;
}

int Module::deleteAllRoute(const char   *path, 
						   const char   *types, 
						   lo_arg       **argv, 
						   int          argc,
						   void         *data, 
						   void         *user_data)
{
    Module *mod = (Module *)user_data;
    mod->deleteAllRoute((char *)argv[0]);
    return 0;
}

Module::Module(Server *s, const char *osc)
{
    st = s;
    getAddr();
    setOSCAddr(osc);
    
    addMethodToServer("/SetRoute", "ss", Module::setRoute, this);
    addMethodToServer("/AddRoute", "ss", Module::addRoute, this);
    addMethodToServer("/DeleteRoute", "ss", Module::deleteRoute, this);
	addMethodToServer("/DeleteAllRoute", "s", Module::deleteAllRoute, this);


    rTable = new RoutingTable();
    tID = -1;
	mColor = -1;
}

void Module::sendSetMdtkn()
{
    int sock, n, d_len;
    struct sockaddr_in addr;
    void *data; 
    char path[] = "/Coordinator/SetMdtkn";
    
    //create lo_message
    lo_message m = lo_message_new();
    lo_message_add_string(m, IPAddr);
    lo_message_add_string(m, OSCAddr);
    lo_message_add_int32(m, tID);
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
    for (int i=0; i<3; i++) {
        n = sendto(sock, data, d_len, 0, (struct sockaddr *)&addr, sizeof(addr));
        if (n < 1) {
            perror("sendto");
        }
        usleep(1000);
    }
    lo_message_free(m);
    close(sock);
}

void Module::sendDeleteMdtkn()
{
    int sock, n, d_len;
    struct sockaddr_in addr;
    void *data;
    char path[] = "/Coordinator/DeleteMdtkn";
    
    //create lo_message
    lo_message m = lo_message_new();
    lo_message_add_string(m, IPAddr);
    lo_message_add_string(m, OSCAddr);
    lo_message_add_int32(m, tID);
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
    
    //send
    for (int i=0; i<3; i++) {
        n = sendto(sock, data, d_len, 0, (struct sockaddr *)&addr, sizeof(addr));
        if (n < 1) {
            perror("sendto");
        }
        usleep(1000);
    }
    close(sock);
    usleep(10000);
}

void Module::sendData(Data *d, lo_arg **argv, int argc)
{
    if (d) {
        switch (argc) {
            case 0:
                for (int i=0; i<rTable->aNum; i++) {
                    if (strstr(rTable->oscAddr[i], "/Data")) {
                        lo_send(rTable->loAddr[i], 
                                rTable->oscAddr[i],
                                "ii", 
                                d->value,
                                d->dataID);
                    }
                }
                break;
            
            case 2:
                for (int i=0; i<rTable->aNum; i++) {
                    if (strstr(rTable->oscAddr[i], "/Data")) {
                        lo_send(rTable->loAddr[i], 
                                rTable->oscAddr[i],
                                "iiii",
                                argv[0]->i,
                                argv[1]->i,
                                d->value,
                                d->dataID);
                    }
                } 
                break;
            
            case 4:
                for (int i=0; i<rTable->aNum; i++) {
                    if (strstr(rTable->oscAddr[i], "/Data")) {
                        lo_send(rTable->loAddr[i], 
                                rTable->oscAddr[i],
                                "iiiiii",
                                argv[0]->i,
                                argv[1]->i,
                                argv[2]->i,
                                argv[3]->i,
                                d->value,
                                d->dataID);
                    }
                } 
                break;
            
            case 6:
                for (int i=0; i<rTable->aNum; i++) {
                    if (strstr(rTable->oscAddr[i], "/Data")) {
                        lo_send(rTable->loAddr[i], 
                                rTable->oscAddr[i],
                                "iiiiiiii",
                                argv[0]->i,
                                argv[1]->i,
                                argv[2]->i,
                                argv[3]->i,
                                argv[4]->i,
                                argv[5]->i,
                                d->value,
                                d->dataID);
                    }
                } 
                break;
        
            case 8:
                for (int i=0; i<rTable->aNum; i++) {
                    if (strstr(rTable->oscAddr[i], "/Data")) {
                        lo_send(rTable->loAddr[i], 
                                rTable->oscAddr[i],
                                "iiiiiiiiii",
                                argv[0]->i,
                                argv[1]->i,
                                argv[2]->i,
                                argv[3]->i,
                                argv[4]->i,
                                argv[5]->i,
                                argv[6]->i,
                                argv[7]->i,
                                d->value,
                                d->dataID);
                    }
                } 
                break;
        
            case 10:
                for (int i=0; i<rTable->aNum; i++) {
                    if (strstr(rTable->oscAddr[i], "/Data")) {
                        lo_send(rTable->loAddr[i], 
                                rTable->oscAddr[i],
                                "iiiiiiiiii",
                                argv[2]->i,
                                argv[3]->i,
                                argv[4]->i,
                                argv[5]->i,
                                argv[6]->i,
                                argv[7]->i,
                                argv[8]->i,
                                argv[9]->i,
                                d->value,
                                d->dataID);
                    }
                } 
                break;
            
            default:
                break;
        }
    }else {
        switch (argc) {
            case 2:
                for (int i=0; i<rTable->aNum; i++) {
                    if (strstr(rTable->oscAddr[i], "/Data")) {
                        lo_send(rTable->loAddr[i], 
                                rTable->oscAddr[i],
                                "ii", 
                                argv[0]->i,
                                argv[1]->i);
                    }
                }
                break;
                
            case 4:
                for (int i=0; i<rTable->aNum; i++) {
                    if (strstr(rTable->oscAddr[i], "/Data")) {
                        lo_send(rTable->loAddr[i], 
                                rTable->oscAddr[i],
                                "iiii",
                                argv[0]->i,
                                argv[1]->i,
                                argv[2]->i,
                                argv[3]->i);
                    }
                } 
                break;
                
            case 6:
                for (int i=0; i<rTable->aNum; i++) {
                    if (strstr(rTable->oscAddr[i], "/Data")) {
                        lo_send(rTable->loAddr[i], 
                                rTable->oscAddr[i],
                                "iiiiii",
                                argv[0]->i,
                                argv[1]->i,
                                argv[2]->i,
                                argv[3]->i,
                                argv[4]->i,
                                argv[5]->i);
                    }
                } 
                break;
                
            case 8:
                for (int i=0; i<rTable->aNum; i++) {
                    if (strstr(rTable->oscAddr[i], "/Data")) {
                        lo_send(rTable->loAddr[i], 
                                rTable->oscAddr[i],
                                "iiiiiiii",
                                argv[0]->i,
                                argv[1]->i,
                                argv[2]->i,
                                argv[3]->i,
                                argv[4]->i,
                                argv[5]->i,
                                argv[6]->i,
                                argv[7]->i);
                    }
                } 
                break;
                
            case 10:
                for (int i=0; i<rTable->aNum; i++) {
                    if (strstr(rTable->oscAddr[i], "/Data")) {
                        lo_send(rTable->loAddr[i], 
                                rTable->oscAddr[i],
                                "iiiiiiiiii",
                                argv[0]->i,
                                argv[1]->i,
                                argv[2]->i,
                                argv[3]->i,
                                argv[4]->i,
                                argv[5]->i,
                                argv[6]->i,
                                argv[7]->i,
                                argv[8]->i,
                                argv[9]->i);
                    }
                } 
                break;
                
            case 0: 
                break;
                
            default:
                break;
        }
    }
}

void Module::sendAudio(Audio *a)
{
    Module::sendAudio(a->audio, a->length);
}

void Module::sendAudio(short *a, unsigned long l)
{
    lo_blob b = lo_blob_new(l, a);
    for (int i=0; i<rTable->aNum; i++) {
        if (strstr(rTable->oscAddr[i], "/Stream")) {
            lo_send(rTable->loAddr[i], 
                    rTable->oscAddr[i],
                    "b", 
                    b);
        }
    }
}

void Module::getAddr()
{
    int fd;
    struct ifreq ifr;
    
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    /* IPv4のIPアドレスを取得 */
    ifr.ifr_addr.sa_family = AF_INET;
    
    /* eth0のIPアドレスを取得 */

    strncpy(ifr.ifr_name, "en1", IFNAMSIZ-1);

    ioctl(fd, SIOCGIFADDR, &ifr);
    
    close(fd);
    
    strcpy(IPAddr, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
}

void Module::setOSCAddr(const char *osc)
{
    strcpy(OSCAddr, osc);
}

void Module::addMethodToServer(const char *path, const char *type, lo_method_handler h, void *user_data)
{
    char p[64];
    strcpy(p, OSCAddr);
    strcat(p, path);
    lo_server_thread_add_method(st->st, p, type, h, user_data);
}

void Module::deleteMethodFromServer(const char *path, const char *type)
{
    char p[64];
    strcpy(p, OSCAddr);
    strcat(p, path);
    lo_server_thread_del_method(st->st, p, type);
}

void Module::setRoute(char *ip, char *osc)
{
    if (rTable->isRouting(ip, osc) && rTable->numRoute(osc) == 1) return;
    
    rTable->deleteAllRoute(osc);
    rTable->setRoute(ip, osc);
}

void Module::addRoute(char *ip, char *osc)
{
    rTable->setRoute(ip, osc);
}

void Module::deleteRoute(char *ip,char *osc)
{
    rTable->deleteRoute(ip, osc);
}

void Module::deleteAllRoute(char *osc)
{
    rTable->deleteAllRoute(osc);
}

void Module::connectTo(Module *m, const char *t)
{
	char p[64];
	strcpy(p, m->OSCAddr);
    strcat(p, t);
	addRoute(m->IPAddr, p);
}

void Module::disconnectFrom(Module *m, const char *t)
{
	char p[64];
	strcpy(p, m->OSCAddr);
    strcat(p, t);
	deleteRoute(m->IPAddr, p);
}

Module::~Module()
{
    //sendDeleteMdtkn();
	deleteMethodFromServer("/SetRoute", "ss");
	deleteMethodFromServer("/AddRoute", "ss");
    deleteMethodFromServer("/DeleteRoute", "ss");
	deleteMethodFromServer("/DeleteAllRoute", "s");
    delete rTable;
}