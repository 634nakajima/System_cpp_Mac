#ifndef _DACManager_h
#define _DACManager_h

#include "ModuleManager.h"
#include "MyModule.h"

class MyModuleManager : public ModuleManager
{
public:
    std::list<MyModule*> mmList;

    MyModuleManager(Server *s, const char *osc);
    ~MyModuleManager();
    
private:
    
	static int mm(const char   *path, 
                  const char   *types, 
                  lo_arg       **argv, 
                  int          argc,
                  void         *data, 
                  void         *user_data);
};

#endif
