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
    char                dn[128];
    dn[0] = '/';
    
    printf("Please Enter Your Device Name!\n");
    scanf("%s", &dn[1]);

	s = new Server();
    xbc = new XBeeController(s, "/XBC", "/dev/cu.usbserial-A50178PJ");
	mm	= new ModuleManager(s, dn);
	co	= new Coordinator(s, "/Coordinator");
    se  = new Serial(s, "/Serial", "/dev/cu.usbmodemfd131");
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