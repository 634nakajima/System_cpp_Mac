//
//  Tile.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/10/14.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Tile.h"

Tile::Tile(const char *t, char a[8])
{
    for (int i = 0; i < 8; i++) {
        XBeeAddr[i] = a[i];
    }
    strcpy(tid, t);
    mColor = -1;
    dead = 1;
    standby = 0;
    alive = 0;
    deadCount = 0;
}

void Tile::isAlive()
{
    if (alive) {
        deadCount = 0;
    }
}

int Tile::deadCheck()
{
    if (dead) return 1;
    
    if(deadCount++ > TH) {
        dead = 1;
        standby = 0;
        alive = 0;
        mColor = -1;
        deadCount = 0;
        return 1;
    }
    return 0;
}

Tile::~Tile()
{}