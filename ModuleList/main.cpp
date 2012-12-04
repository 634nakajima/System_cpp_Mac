#include "rtsosc.h"
#include "MyModule.h"

int main()
{
    Server              *s;
	ModuleList			*ml;
	Serial              *se;
    MyModule            *mm;
	int                 mColor;
    
	s	= new Server();
	ml	= new ModuleList(s, "/ModuleList");
    se  = new Serial(s, "/Serial", "/dev/cu.usbserial-A600afyl");
	mm  = new MyModule(s, "/MM");
        
    se->connectTo(mm, "/Stream");
    ml->requestML();
	
    while (1) {
        printf("Enter Module Color\n");
        scanf("%d", &mColor);
        if (!mColor) break;
		if (mColor == 99) ml->requestML();
        if (mm->tID) 
            ml->createModule(mm->tID, mColor);
    }	
	
	delete se;
	delete mm;
	delete ml;
	delete s;
	
	return 0;
}