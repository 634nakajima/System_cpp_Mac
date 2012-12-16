/*
 *  XBeeController.cpp
 *  
 *
 *  Created by kannolab on 12/10/11.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "XBeeController.h"

XBeeController::XBeeController(Server *s, const char *osc) : Module(s, osc)
{	
	co = NULL;
	rp = 0;
	wp = 0;

	addMethodToServer("/Stream", "b", stream, this);
	
	serial = new Serial(s, "/Serial");
	serial->connectTo(this, "/Stream");
    Pt_Start(0.25, this->deadCheck, this);
}

XBeeController::XBeeController(Server *s, const char *osc, const char *d) : Module(s, osc)
{	
	co = NULL;
	rp = 0;
	wp = 0;

	addMethodToServer("/Stream", "b", stream, this);

	serial = new Serial(s, "/Serial", d);
	serial->connectTo(this, "/Stream");
    Pt_Start(0.25, this->deadCheck, this);
}

void XBeeController::deadCheck(PtTimestamp timestamp, void *userData)
{
    XBeeController *xbc = (XBeeController *)userData;
	for (std::map<int, Tile*>::iterator iter = xbc->tMap.begin(); iter!=xbc->tMap.end(); iter++) {
		Tile *t = iter->second;
		if(t->deadCheck()) {
			if (xbc->co != NULL) {
				xbc->co->deleteMtkn(t->tID);
				xbc->setAlive(t->tID, 0);
			}
		}
    }
}

void XBeeController::initTile(int tID, unsigned char *a64, unsigned char *a16)
{
	for (std::map<int, Tile*>::iterator iter = tMap.begin(); iter!=tMap.end(); iter++) {
        Tile *t = iter->second;
		if (t->tID == tID) return;
    }
	Tile *tile = new Tile(tID, a64, a16);

	tMap.insert(std::map<int, Tile*>::value_type(tID, tile));
	printf("init Tile:%d\n", tMap[tID]->tID);
}

void XBeeController::setCoordinator(Coordinator *coordinator)
{
	co = coordinator;
}

void XBeeController::parseData()
{
	unsigned int mode, tid1, tid2, mColor, type;
	unsigned char a64[8], a16[2], s;

	if (available() > 20) {
		if (readData() == 0x7E) {
			for (int i = 0; i < 3; i++) {
				readData();
			}
			
			a64[0] = readData();
			a64[1] = readData();
			a64[2] = readData();
			a64[3] = readData();
			a64[4] = readData();
			a64[5] = readData();
			a64[6] = readData();
			a64[7] = readData();

			a16[0] = readData();
			a16[1] = readData();

			readData();
		
			mode = readData();
			tid1 = readData();
			tid2 = readData();
			type = readData();
			mColor = readData();

			readData();
			
			if (tMap.count(tid2)) {
				tMap[tid2]->isAlive();
			}else {
				//printf("%d, %d, %d, %d, %d\n",mode, tid1, tid2, type, mColor);
			}
			
			if (co != NULL) {
				switch (mode) {
					case 0x00:
						switch (type) {
							case 0x00:
								if (tMap.count(tid2)) {
									if (tMap[tid2]->data == tid1 || tMap[tid2]->data == 0) {					
										co->connect(tid2, tid1, "/Data");
										tMap[tid2]->data = tid1;
									}else {
										tMap[tid2]->data = tid1;
									}
								}
								break;
							case 0x01:
								if (tMap.count(tid2)) {
									if (tMap[tid2]->stream == tid1 || tMap[tid2]->stream == 0) {					
										co->connect(tid2, tid1, "/Stream");
										tMap[tid2]->stream = tid1;

									}else {
										tMap[tid2]->stream = tid1;
									}
								}
								break;
							default:
								break;
						}
						break;

					case 0x01:
						switch (type) {
							case 0x00:
								if (tMap.count(tid2)) {
									co->disconnectAll(tid2, "/Data");
									tMap[tid2]->data = 0;
								}
								break;
							case 0x01:
								if (tMap.count(tid2)) {
									co->disconnectAll(tid2, "/Stream");
									tMap[tid2]->stream = 0;
								}
								break;
							default:
								break;
						}
						break;
					
					case 0x02:
						initTile(tid2, a64, a16);
						break;
					
					default:
						break;
				}
			}
			
			//printf("%d, %d, %d, %d, %d\n",mode, tid1, tid2, type, mColor);
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

void XBeeController::setAlive(int tID, int mColor)
{
	if (!tMap.count(tID)) return;
	Tile *t = tMap[tID];
    t->mColor = mColor;
	if (mColor) 
		t->isAlive();
    //送信フレームの作成
    unsigned char data[19];

    data[0] = 0x7E;
    data[1] = 0x0;
    data[2] = 0x0F;
    data[3] = 0x10;
    data[4] = 0x00;
    data[5] = t->XBeeAddr64[0];
    data[6] = t->XBeeAddr64[1];
    data[7] = t->XBeeAddr64[2];
    data[8] = t->XBeeAddr64[3];
    data[9] = t->XBeeAddr64[4];
    data[10] = t->XBeeAddr64[5];
    data[11] = t->XBeeAddr64[6];
    data[12] = t->XBeeAddr64[7];
    data[13] = t->XBeeAddr16[0];
    data[14] = t->XBeeAddr16[1];
    data[15] = 0x00;
    data[16] = 0x00;
    data[17] = (char)mColor;
	
	long sum = 0;
	for (int i=3; i<18; i++) {
		sum += data[i];
	}
	
    data[18] = (0xFF - (sum & 0xFF));
    //送信
    serial->serialWrite(data, 19);
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
	for (std::map<int, Tile*>::iterator iter = tMap.begin(); iter!=tMap.end(); iter++) {
        delete iter->second;
    }
	tMap.clear();
	delete serial;
	deleteMethodFromServer("/Stream", "b");
}