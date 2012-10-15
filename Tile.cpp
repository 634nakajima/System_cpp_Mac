//
//  Tile.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/10/14.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Tile.h"

Tile::Tile(int t, char *a64, char *a16)
{
    for (int i = 0; i < 8; i++) {
        XBeeAddr64[i] = a64[i];
    }
	for (int i = 0; i < 2; i++) {
        XBeeAddr16[i] = a16[i];
    }
	
    tID = t;
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
    }else {
        dead = 0;
        alive = 1;
    }
}

int Tile::deadCheck()
{
    if (dead) return 0;

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