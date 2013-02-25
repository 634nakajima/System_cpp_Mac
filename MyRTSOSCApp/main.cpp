#include "rtsosc.h"
#include "MyModuleManager.h"

int main()
{
	Server          *s;//受信OSCデータの解析、対象モジュールのメソッド呼び出し
	AudioClock      *ac;//オーディオデータ送信用タイマ
	Serial          *se;//シリアル通信
    MyModuleManager	*mmm;//自作用モジュール
	
	s = new Server();
	ac = new AudioClock(s, "/AC");//アドレス"/AC"でインスタンス生成
	dac = new DAC(s, "/DAC");//アドレス"/DAC"でインスタンス生成
    mmm = new MyModuleManager(s, "/MMM");//アドレス"/MMM"でインスタンス生成
    
	dac->ch = 2;//LRで再生
    dac->bs = 128;//最小バッファサイズ
	
	se->connectTo(mmm, "/Serial");//seをmm(/MM)の下位アドレス"/Serial"に接続
	ac->connectTo(mmm, "/Stream");//acをmm(/MM)の下位アドレス"/Stream"に接続
	mmm->connectTo(dac, "/Stream");//mmをdac(/DAC)の下位アドレス"/Stream"に接続
	
	getchar();//ブロック
	
	//インスタンスの解放
	delete mmm;
	delete dac;
	delete ac;
	delete s;
	
	return 0;
}