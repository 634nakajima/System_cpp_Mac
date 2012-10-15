//
//  Tile.h
//  
//
//  Created by NAKAJIMA Musashi on 12/10/14.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _Tile_h
#define _Tile_h

#define TH  10

class Tile
{
public:
    char    tid[3];
    int     mColor;
    int     deadCount;
    int     dead, alive, standby;
    char    XBeeAddr[8];
    
    Tile(const char *t, char a[8]);
    ~Tile();
    
    void    isAlive();
    int     deadCheck();
};

#endif
