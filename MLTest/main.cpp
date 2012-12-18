#include "rtsosc.h"
#include "MyModule.h"
#include "ModuleManager.h"

int main()
{
    Server              *s;
    Coordinator			*co;
	ModuleManager       *mm;
    XBeeController      *xbc;
	Serial              *se;
    MyModule            *mym;
	int                 mIndex;
    
	s = new Server();
    xbc = new XBeeController(s, "/XBC", "/dev/cu.usbserial-A50178PJ");
	mm	= new ModuleManager(s, "/ModuleManager");
	co	= new Coordinator(s, "/Coordinator");
    se  = new Serial(s, "/Serial", "/dev/cu.usbserial-A600afyl");
	mym = new MyModule(s, "/MM");
    
	mm->sendModuleList();
    xbc->co = co;
    co->xbc = xbc;
    
    se->connectTo(mym, "/Stream");
    
    while (1) {
        printf("Enter Module Index\n");
        scanf("%d", &mIndex);
        if (!mIndex) break;
        
        if (mIndex == -1) {
            co->ml->requestML();
            co->ml->displayModules();
        }
        
        if (mym->tID) 
            co->ml->createModule(mym->tID, mIndex);

    }	

	return 0;
}