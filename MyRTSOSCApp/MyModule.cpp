//自作用モジュールテンプレート
#include "MyModule.h"

//コンストラクタ
MyModule::MyModule(Server *s, const char *osc) : Module(s,osc) {
    
	addMethodToServer("/Stream", "b", stream, this);//下位アドレス"/Stream"で待ち受け
	addMethodToServer("/Serial", "b", serial, this);//下位アドレス"/Serial"で待ち受け
    addMethodToServer("/Data", "ii", data, this);//下位アドレス"/Serial"で待ち受け

}

int MyModule::stream(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data) {

	MyModule *o = (MyModule *)user_data;
	
	lo_blob b = (lo_blob)argv[0];
    short *dp = (short *)lo_blob_dataptr(b);//オーディオデータの先頭ポインタ
    int size = lo_blob_datasize(b);//オーディオのデータサイズ(２で割るとパケット数)

	//オーディオ処理
	
	
	return 0;
}

int MyModule::serial(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data) {
	
	MyModule *o = (MyModule *)user_data;
	
	lo_blob b = (lo_blob)argv[0];
    unsigned char *dp = (unsigned char *)lo_blob_dataptr(b);//シリアルデータの先頭ポインタ
    int size = lo_blob_datasize(b);//シリアルデータサイズ
	
	//シリアルデータ処理
	
	return 0;
}

int MyModule::data(const char   *path, 
                   const char   *types, 
                   lo_arg       **argv, 
                   int          argc,
                   void         *data, 
                   void         *user_data) {
	int data1 = argv[0]->i;//演奏データ1
	int data2 = argv[1]->i;//演奏データ2
	
	//演奏データ処理
	
	return 0;
}

//デストラクタ
MyModule::~MyModule() {
}

