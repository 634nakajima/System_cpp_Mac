#include "rtsosc.h"
#include "MyModule.h"

int main()
{
	Server		*s;//受信OSCデータの解析、対象モジュールのメソッド呼び出し
	AudioClock	*ac;//オーディオデータ送信用タイマ
	Sine		*sine;
	DAC			*dac;
	Coordinator	*co;
	
	s = new Server();
	ac = new AudioClock(s, "/AC");
	sine = new Sine(s, "/Sine");
	dac = new DAC(s, "/DAC");
	co = new Coordinator(s, "/Coordinator");
	
	ac->connectTo(sine, "/Stream");//acをmm(/MM)の下位アドレス"/Stream"に接続
	
	dac->ch = 2;//LRで再生
    dac->bs = 1024;//バッファサイズ
	sine->tID = 1;//コーディネータに登録するID
	sine->sendSetMdtkn();//コーディネータにモジュールのアドレスを送信
	
	printf("Please Press 'Enter' to Connect the Modules");
	getchar();//ブロック
	sine->connectTo(dac, "/Stream");
	co->connect(1, 2, "/Stream");//ID1のモジュールをID2のモジュールの下位アドレス"/Stream"に接続
	
	getchar();
	//インスタンスの解放
	delete dac;
	delete ac;
	delete sine;
	delete s;
	delete co;
	
	return 0;
}