#include "rtsosc.h"
#include "MyModule.h"

int main()
{
	Server	*s;
	DAC		*dac;
	
	s = new Server();
	dac = new DAC(s, "/DAC");//アドレス"/DAC"でインスタンス生成
    
	dac->ch = 2;//LRで再生
    dac->bs = 1024;//バッファサイズ
	dac->tID = 2;//コーディネータに登録するID
	dac->sendSetMdtkn();//コーディネータにモジュールのアドレスを送信
	
	getchar();//ブロック
	
	//インスタンスの解放
	delete dac;
	delete s;
	
	return 0;
}