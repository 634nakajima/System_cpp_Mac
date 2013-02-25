#include "rtsosc.h"
#include "DACManager.h"
#include <stdio.h>

int main()
{
	Server      *s;
	DACManager  *dm;
    char        dn[128];
    dn[0] = '/';
    
    printf("Please Enter Your Device Name!\n");
    scanf("%s", &dn[1]);
    
	s   = new Server();
    dm	= new DACManager(s, dn);
	
	dm->sendModuleList();
	
    while (1);
	
	delete dm;
	delete s;
	
	return 0;
}