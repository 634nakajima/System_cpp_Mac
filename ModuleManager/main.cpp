#include "rtsosc.h"

int main()
{
	Server				*s;
	ModuleController	*mc;
    
	s	= new Server();
    mc	= new ModuleController(s, "/ModuleManager");
	
	mc->sendModuleList();
	
    while (1) {
		if(getchar() == '0') break;
    }	
	
	delete mc;
	delete s;
	
	return 0;
}