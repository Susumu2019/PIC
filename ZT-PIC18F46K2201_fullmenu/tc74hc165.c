/*****************************************************************************************
74HC165用ライブラリ 2015/4/10作成

□提供元
ZEATEC co.,ltd. 

□ご利用について
転載・無断使用可です。このライブラリを使用した上での不具合等に関しては、いかなる内容におい
ても一切の責任を追わないものとします。

□使用方法
#include "tc74hc165.c"をヘッダーに追加してください。
取得したい変数を用意してtest_val = tc74hc595();というように使います。

*****************************************************************************************/
//シリアルパラレル変換
long tc74hc165(){
	//PIN_E3	QH シリアルデータ
	//PIN_E4	CK クロック
	//PIN_E5	S/L データ送信開始指示

	int tmp1 = 0;
	long tmp_ans = 0;

	//S/L
	output_bit(PIN_E5,0);
	output_bit(PIN_E5,1);

	for(tmp1=0;tmp1<16;tmp1++){
		//CK
		output_bit(PIN_E4,0);
		
		if(input(PIN_E3) == 0){
			bit_set(tmp_ans,15-tmp1);
		}else{
			bit_clear(tmp_ans,15-tmp1);
		}

		//CK
		output_bit(PIN_E4,1);

	}

	return tmp_ans;
}

