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
#include "Serial.h"

#define	TILE_NUM	16

class Coordinator;
class XBeeController : public Module {
	
public:
    
    XBeeController(lo_server_thread s, const char *osc);
    ~XBeeController();
    
    char                    address[TILE_NUM][8];
    Coordinator				*co;
	Serial					*serial;
	char					buf[256];
	int						rp,wp;
	
	void	setCoordinator(Coordinator *coordinator);

	
private:
	void	setXBeeAddress();
	void	parseData();
	char	readData();
	int		available();

    static int stream(const char   *path, 
					  const char   *types, 
					  lo_arg       **argv, 
					  int          argc,
					  void         *data, 
					  void         *user_data);
};

#endif