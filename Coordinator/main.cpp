#include "rtsosc.h"

int main()
{
	Server              *s;
    Coordinator			*co;
    XBeeController      *xbc;
	ModuleController	*mc;
    
	s = new Server();
    xbc = new XBeeController(s, "/XBC", "/dev/cu.usbserial-A50178PJ");
	co	= new Coordinator(s, "/Coordinator");
    mc	= new ModuleController(s, "/ModuleManager");
	
    xbc->co = co;
    co->xbc = xbc;
	mc->sendModuleList();
	
    while (1) {
    }	
	
	delete xbc;
	delete co;
	delete s;
	
	return 0;
}