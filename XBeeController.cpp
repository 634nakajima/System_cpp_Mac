/*
 *  XBeeController.cpp
 *  
 *
 *  Created by kannolab on 12/10/11.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "XBeeController.h"

XBeeController::XBeeController(lo_server_thread s, const char *osc) : Module(s, osc)
{	
	co = NULL;
	rp = 0;
	wp = 0;
	setXBeeAddress();
	addMethodToServer("/Stream", "b", stream, this);
	
	serial = new Serial(s, "/Serial");
	serial->connectTo(this, "/Stream");
    
    Pt_Start(0.1, this->deadCheck, this);

}

void XBeeController::deadCheck(PtTimestamp timestamp, void *userData)
{
    XBeeController *xbc = (XBeeController *)userData;
    for (int i=0; i<8; i++) {
        if(xbc->tile[i]->deadCheck() && (xbc->co != NULL)) {
            xbc->co->ml->deleteModule((char *)xbc->tile[i]->tid, xbc->tile[i]->mColor);
        }
    }
}

void XBeeController::initTile()
{
    tile[0] = new Tile("1", address[0]);
    tile[1] = new Tile("2", address[1]);
    tile[2] = new Tile("3", address[2]);
    tile[3] = new Tile("4", address[3]);
    tile[4] = new Tile("5", address[4]);
    tile[5] = new Tile("6", address[5]);
    tile[6] = new Tile("7", address[6]);
    tile[7] = new Tile("8", address[7]);

}
void XBeeController::setXBeeAddress()
{/*
	address[0][0] = ;
	address[0][1] = ;
	address[0][2] = ;
	address[0][3] = ;
	address[0][4] = ;
	address[0][5] = ;
	address[0][6] = ;
	address[0][7] = ;
	address[1][0] = ;
	address[1][1] = ;
	address[1][2] = ;
	address[1][3] = ;
	address[1][4] = ;
	address[1][5] = ;
	address[1][6] = ;
	address[1][7] = ;
	address[2][0] = ;
	address[2][1] = ;
	address[2][2] = ;
	address[2][3] = ;
	address[2][4] = ;
	address[2][5] = ;
	address[2][6] = ;
	address[2][7] = ;
	address[3][0] = ;
	address[3][1] = ;
	address[3][2] = ;
	address[3][3] = ;
	address[3][4] = ;
	address[3][5] = ;
	address[3][6] = ;
	address[3][7] = ;
	address[4][0] = ;
	address[4][1] = ;
	address[4][2] = ;
	address[4][3] = ;
	address[4][4] = ;
	address[4][5] = ;
	address[4][6] = ;
	address[4][7] = ;
	address[5][0] = ;
	address[5][1] = ;
	address[5][2] = ;
	address[5][3] = ;
	address[5][4] = ;
	address[5][5] = ;
	address[5][6] = ;
	address[5][7] = ;
	address[6][0] = ;
	address[6][1] = ;
	address[6][2] = ;
	address[6][3] = ;
	address[6][4] = ;
	address[6][5] = ;
	address[6][6] = ;
	address[6][7] = ;
	address[7][0] = ;
	address[7][1] = ;
	address[7][2] = ;
	address[7][3] = ;
	address[7][4] = ;
	address[7][5] = ;
	address[7][6] = ;
	address[7][7] = ;*/
}

void XBeeController::setCoordinator(Coordinator *coordinator)
{
	co = coordinator;
}

void XBeeController::parseData()
{
	int mode, tid1, tid2, mColor, type;
	tid1 = readData();
	printf("%d\n",tid1);
	if (available()!=21) return;
	
	if (readData() == 0x7E) {
		for (int i = 0; i < 14; i++) {
			readData();
		}
		
		mode = readData();
		tid1 = readData();
		tid2 = readData();
        mColor = readData();
		type = readData();
		readData();
		printf("%d,%d, %d\n", tid1, tid2, mColor);
		
        tile[tid2]->isAlive();
        
		if (co != NULL) {
			switch (mode) {
				case 0x00:
					switch (type) {
						case 0x00:
							co->connect(tid1, tid2, "/Data");
							break;
						case 0x01:
							co->connect(tid1, tid2, "/Stream");
							break;
						default:
							break;
					}
					break;

				case 0x01:
					switch (type) {
						case 0x00:
							co->disconnect(tid1, tid2, "/Data");
							break;
						case 0x01:
							co->disconnect(tid1, tid2, "/Stream");
							break;
						default:
							break;
					}
					break;

				default:
					break;
			}
		}
	}
}

char XBeeController::readData()
{
	char value;
	value = buf[rp];
	rp = (rp != 255 ? rp+1 : 0);
	return value;
}

int XBeeController::available()
{
	if (rp <= wp) {
		return (wp-rp);
	}else {
		return (256+wp-rp);
	}
}

int XBeeController::stream(const char   *path, 
						   const char   *types, 
						   lo_arg       **argv, 
						   int          argc,
						   void         *data, 
						   void         *user_data)
{
	XBeeController *xbc = (XBeeController *)user_data;
	lo_blob b = (lo_blob)argv[0];
    char *dp = (char *)lo_blob_dataptr(b);
    int size = lo_blob_datasize(b);
	for (int i = 0; i < size; i++) {
		xbc->buf[xbc->wp] = *dp++;
		xbc->wp = (xbc->wp != 255 ? xbc->wp+1 : 0);
	}
	xbc->parseData();
}

XBeeController::~XBeeController()
{
    for (int i=0; i<8; i++) {
        delete tile[i];
    }
	delete serial;
}