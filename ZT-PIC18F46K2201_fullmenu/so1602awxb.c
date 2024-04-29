/*****************************************************************************************
LCDモジュール(SO1602AWGB-UC-WB-U,SO1602AWWB-UC-WB-U,SO1602AWYB-UC-WB-U)
用ライブラリ 2015/2/14作成

□提供元
ZEATEC co.,ltd. 

□ご利用について
転載・無断使用可です。このライブラリを使用した上での不具合等に関しては、いかなる内容におい
ても一切の責任を追わないものとします。

□使用方法
lcd_ini()をスタートアップに入れ、lcd_char("ｱｲｳｴｵabc")で任意の文字列を表示す
る。又はprintf(lcd_char,"    %02x:%02x:%02x    ",hour,min,sec);でprintfを使用して出
力する。
I2Cはハードウェア制御を推奨とし、宣言例は下記のとおり。

#use i2c(MASTER, SDA=PIN_C4, SCL=PIN_C3,SLOW,FORCE_HW)//FAST=400kbps SLOW=100kbps
ピンアサインはPIC16F1947で使用した場合とします。

スタートアップに入れるソース
	//有機ELキャラクタモジュールの初期化
	//lcd_ini();

	////有機ELキャラクタモジュールを表示
	////１行目表示
	//lcd_start(1);
	//lcd_char("ZEATEC co.,ltd. ");
	////２行目表示
	//lcd_start(2);
	//printf(lcd_char,	"ｶ)ｼﾞｰﾃｯｸ        ");

*****************************************************************************************/

void lcd_cmd(char c){
	//スタートコンディションを発行する
	i2c_start();
	//LCDのアドレス
	i2c_write(0x78);
	// control byte の送信(コマンドを指定)
	i2c_write(0x00);
	// data byte の送信
	i2c_write(c);
	// ストップコンディションを発行する
	i2c_stop();
}

void so1602awxb_contrast(int cont){
	lcd_cmd(0x2a);//RE=1
	lcd_cmd(0x79);//SD=1
	lcd_cmd(0x81);//コントラストセット
	lcd_cmd(cont);//コントラスト値
	lcd_cmd(0x78);//SDを0に戻す
	lcd_cmd(0x28);//2C=高文字　28=ノーマル
}

void lcd_ini(){
	lcd_cmd(0x01);//クリアーディスプレイ
	lcd_cmd(0x02);//カーソルを初期値にする
	lcd_cmd(0x0c);//ディスプレイオン（0x0c,0x0dならチラつき無い）
	lcd_cmd(0x01);//クリアーディスプレイ

	//コントラスト設定
	so1602awxb_contrast(0xff);
}

void lcd_start(int line){
	if(line == 1){
		lcd_cmd(0x80);//1行目指定
	}else if(line == 2){
		lcd_cmd(0xa0);//2行目指定
	}
}

void lcd_char(char s){

	//lcd_cmd(0x01);//クリアーディスプレイ

	//スタートコンディションを発行する
	i2c_start();
	//LCDのアドレス
	i2c_write(0x78);
	i2c_write(0x40);
	i2c_write(s) ;              // data byte の送信(連続送信)

	i2c_stop();
}

