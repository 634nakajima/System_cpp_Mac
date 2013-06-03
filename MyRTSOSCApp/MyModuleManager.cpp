#include <iostream>
#include "MyModuleManager.h"

MyModuleManager::MyModuleManager(Server *s, const char *osc) : ModuleManager(s, osc) {
    strcpy(mPath, "/MyModule"); //アドレスを入れる
    mColor = 1;                 //モジュールカラー番号を入れる
    addMethodToServer(mPath, "is", mm, this);//1:create 0:delete, tID
}

int MyModuleManager::mm(const char   *path, 
                        const char   *types, 
                        lo_arg       **argv, 
                        int          argc,
                        void         *data, 
                        void         *user_data)
{
    MyModuleManager *mmm = (MyModuleManager *)user_data;
    char p[64];
    strcpy(p, mmm->OSCAddr);        //ModuleManagerアドレス
    strcat(p, mmm->mPath);          //ModuleManagerが管理するModuleアドレス
    strcat(p, "/Tile");
    strcat(p, &argv[1]->s);         //タイルID
    printf("Module Addr:%s\n",p);
    
    if (argv[0]->i) {               //argv[0] = 1:モジュール生成 0:モジュール解放
        for (std::list<MyModule*>::iterator iter = mmm->mmList.begin(); iter != mmm->mmList.end(); iter++) {
            MyModule* mm = (*iter);
            if (strcmp(p,mm->OSCAddr)==0) {
                if (mm->tID == atoi(&argv[1]->s)) {
                    printf("err: Creating MyModule\n");
                    return 0;
                }
            }
        }
        
        MyModule *mm = new MyModule(mmm->st, p);
        mm->setTID(atoi(&argv[1]->s));
        mm->mColor = dm->mColor;
        mm->sendSetMdtkn();
        mmm->dacList.push_back(mm);
        printf("create MyModlue\n");
        
    }else {
        for (std::list<MyModule*>::iterator iter = mmm->mmList.begin(); iter != mmm->mmList.end(); iter++) {
            MyModule* mm = (*iter);
            if (strcmp(p,mm->OSCAddr)==0) {
                mm->sendDeleteMdtkn();
                delete mm;
                mmm->dacList.remove(mm);
                printf("delete MyModule\n");
            }
        }
    }
    return 0;
}

MyModuleManager::~MyModuleManager() {
    for (std::list<MyModule*>::iterator iter = mmList.begin(); iter != mmList.end(); iter++) {
        MyModule* mm = (*iter);
        mmList.remove(mm);
        delete mm;
    }
    
    deleteMethodFromServer("/MyModule", "is");      //入れたアドレス
}
