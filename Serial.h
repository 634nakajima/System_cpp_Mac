//
//  Serial.h
//  
//
//  Created by NAKAJIMA Musashi on 12/10/07.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _Serial_h
#define _Serial_h

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <pthread.h>

/* <asm/termbits.h> で定義されているボーレートの設定．これは
 <termios.h>からインクルードされる． */
#define BAUDRATE B9600           
/* 適切なシリアルポートを指すように，この定義を変更する．*/
#define MODEMDEVICE "/dev/cu.usbserial-A600afyl"
#define _POSIX_SOURCE 1  /* POSIX 準拠のソース */

#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;

typedef int (*serialHandler)(void *buf int size);

class Serial : public Module
{
    int         fd,c, res;
    volatile int active;
	volatile int done;
    pthread_t   thread;
    struct      termios oldtio,newtio;
    char        buf[255];
    
    Serial(lo_server_thread s, const char *osc);
    ~Serial();
    void threadStop();
    void prepareSerial();
    void threadStart();
    
    static int sWrite(const char   *path, 
                      const char   *types, 
                      lo_arg       **argv, 
                      int          argc,
                      void         *data, 
                      void         *user_data);

    static void threadFunction(void *data);
    
};

#endif
