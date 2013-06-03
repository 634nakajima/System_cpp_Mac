/*
 *  XBeeController.h
 *  
 *
 *  Created by kannolab on 12/10/11.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _XBeeController_h
#define _XBeeController_h

#include "Module.h"
#include "Coordinator.h"
#include "Tile.h"
#include "Serial.h"
#include <map>
#ifndef	_PT_
#include "porttime.h"
#define _PT_
#endif


#define	TILE_NUM	40

class Coordinator;
class XBeeController : public Module {
	
public:
    
    XBeeController(Server *s, const char *osc);
	XBeeController(Server *s, const char *osc, const char *d);

    ~XBeeController();
    
	std::map<int, Tile*>    tMap;;
    Coordinator				*co;
	Serial					*serial;
	char					buf[1024];
	int						rp,wp;
	
	void	setCoordinator(Coordinator *coordinator);
    void    setAlive(int tID, int mColor);
	
private:
    void    initTile(int tID, unsigned char *a64, unsigned char *a16);
	void	parseData();
	char	readData();
	int		available();

    static void deadCheck(PtTimestamp   timestamp,
                          void          *userData);

    static int stream(const char   *path, 
					  const char   *types, 
					  lo_arg       **argv, 
					  int          argc,
					  void         *data, 
					  void         *user_data);
};

#endif