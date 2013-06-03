#include <iostream>
#include "ModuleManager.h"

ModuleManager::ModuleManager(Server *s, const char *osc) : Module(s,osc)
{
    lo_server_thread_add_method(st->st, "/ModuleManager/RequestML", "i", requestML, this);
    mPath[0] = '\0';
    mColor = -1;
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
    strcat(p, mPath);

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
    close(sock);
}

void ModuleManager::deleteModuleList()
{
    int sock, n, d_len;
    struct sockaddr_in addr;
    void *data; 
    char path[] = "/ModuleList/deleteMList";
    char p[64];
    int  mColor;
    strcpy(p, OSCAddr);
    strcat(p, mPath);
    
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
    close(sock);
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
	
	return 1;
}

ModuleManager::~ModuleManager()
{
    lo_server_thread_del_method(st->st, "ModuleManager/RequestML", "i");
}

