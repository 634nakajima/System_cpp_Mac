#include "rtsosc.h"
#include "MyModule.h"

int main()
{
	MyModule    *o;
	Serial      *se;
	Server      *s;
    
	s = new Server();	
    o = new MyModule(s, "/Odore");
	se	= new Serial(s, "/Serial");
	se->connectTo(o, "/Stream");
    
	getchar();
	
	return 0;
}