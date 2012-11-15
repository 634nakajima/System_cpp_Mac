#include "rtsosc.h"
#include "MyModule.h"

int main()
{
	Server		*s;//受信OSCデータの解析、対象モジュールのメソッド呼び出し
	AudioClock	*ac;//オーディオデータ送信用タイマ
	Serial		*se;//シリアル通信
    MyModule	*mm;//自作用モジュール
	
	s = new Server();
	ac = new AudioClock(s, "/AC");//アドレス"/AC"でインスタンス生成
	dac = new DAC(s, "/DAC");//アドレス"/DAC"でインスタンス生成
    mm = new MyModule(s, "/MM");//アドレス"/MM"でインスタンス生成
    
	dac->ch = 2;//LRで再生
    dac->bs = 128;//最小バッファサイズ
	
	se->connectTo(mm, "/Serial");//seをmm(/MM)の下位アドレス"/Serial"に接続
	ac->connectTo(mm, "/Stream");//acをmm(/MM)の下位アドレス"/Stream"に接続
	mm->connectTo(dac, "/Stream");//mmをdac(/DAC)の下位アドレス"/Stream"に接続
	
	getchar();//ブロック
	
	//インスタンスの解放
	delete mm;
	delete dac;
	delete ac;
	delete s;
	
	return 0;
}