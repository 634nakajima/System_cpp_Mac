#include "rtsosc.h"
#include "MyModule.h"

int main()
{
    Server              *s;
    Coordinator			*co;
	ModuleController	*mc;
    XBeeController      *xbc;
	Serial              *se;
    MyModule            *mm;
	int                 mColor;
    
	s = new Server();
    xbc = new XBeeController(s, "/XBC", "/dev/cu.usbserial-A50178PJ");
	mc	= new ModuleController(s, "/MC");
	co	= new Coordinator(s, "/Coordinator");
    se  = new Serial(s, "/Serial", "/dev/cu.usbserial-A600afyl");
	mm  = new MyModule(s, "/MM");
    
	mc->sendModuleList();
    xbc->co = co;
    co->xbc = xbc;
    
    se->connectTo(mm, "/Stream");
    
    while (1) {
        printf("Enter Module Color\n");
        scanf("%d", &mColor);
        if (!mColor) break;

        if (mm->tID) 
            co->ml->createModule(mm->tID, mColor);

    }	

	return 0;
}