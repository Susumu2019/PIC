/*****************************************************************************************
LCDモジュール(aqmxx02a-RN-GBW)用ライブラリ 2015/2/12作成

□提供元
ZEATEC co.,ltd. 

□ご利用について
転載・無断使用可です。このライブラリを使用した上での不具合等に関しては、いかなる内容におい
ても一切の責任を追わないものとします。

□対応機器
AQM0802A-RN-GBW,AQM1602A-RN-GBW

□使用方法
aqmxx02a_init(5)をスタートアップに入れて下さい。カッコ内は使用電圧を指定します。3.3Vの時は
3と指定してください。setcursor_aqmxx02a(0,0)で表示位置を指定し、aqmxx02a_puts("ｱｲｳｴｵabc")
で任意の文字列を指定する。又はprintf(aqmxx02a_puts,"%02x:%02x:%02x",hour,min,sec);とすれば
変数の値も表示できます。
I2Cはハードウェア制御を推奨とし、宣言例は下記のとおり。

#use i2c(MASTER, SDA=PIN_C4, SCL=PIN_C3,SLOW,FORCE_HW)//FAST=400kbps SLOW=100kbps
ピンアサインはPIC16F1947で使用した場合とします。

*****************************************************************************************/
void aqmxx02a_init(int volt);
void aqmxx02a_setcursor(int col, int row);
void aqmxx02a_cmd(unsigned char c);
void aqmxx02a_puts(char s);
void aqmxx02a_clear();

void aqmxx02a_init(int volt){
	aqmxx02a_cmd(0x38) ;     // function set           : データ線は8本・表示は２行・フォントは5x8ドット
	aqmxx02a_cmd(0x39) ;     // function set           : 拡張コマンドの設定を有効にする
	aqmxx02a_cmd(0x14) ;     // Internal OSC frequency : バイアスの選択と内部OSC周波数の調整
	if(volt == 3){
		//コントラスト調整(3.3V)
		aqmxx02a_cmd(0x70) ;     // Contrast set           : コントラスト調整データ(下位4ビット)
		aqmxx02a_cmd(0x5e) ;     // Contrast set           : 昇圧回路有効、アイコン有効、コントラスト調整データ(下位2ビット)
		//aqmxx02a_cmd(0x56) ;     // Contrast set           : 昇圧回路有効、アイコン無効、コントラスト調整データ(下位2ビット)
	}else{
		//コントラスト調整(5V)
		aqmxx02a_cmd(0x7A) ;     // Contrast set           : コントラスト調整データ(下位4ビット)
		aqmxx02a_cmd(0x5c) ;     // Contrast set           : 昇圧回路有効、アイコン有効、コントラスト調整データ(下位2ビット)	
		//aqmxx02a_cmd(0x54) ;     // Contrast set           : 昇圧回路有効、アイコン無効、コントラスト調整データ(下位2ビット)	
	}
	aqmxx02a_cmd(0x6C) ;     // Follower control       : フォロア回路をON、増幅率の調整を行う
	delay_ms(200) ;			// 電力が安定するまで待つ
	aqmxx02a_cmd(0x38) ;     // function set           : 拡張コマンドを設定を無効にする
	aqmxx02a_cmd(0x0C) ;     // display control        : 画面表示はON・カーソル表示はOFF・カーソル点滅はOFF
	aqmxx02a_cmd(0x01) ;     // Clear Display : 画面全体に20Hのｽﾍﾟｰｽで表示、ｶｰｿﾙはcol=0,row=0に移動
	delay_us(1100) ;  // LCDが処理(1.08ms)するのを待ちます
}

void aqmxx02a_setcursor(int col, int row){
	int row_offsets[] = { 0x00, 0x40 };

	aqmxx02a_cmd(0x80 | (col + row_offsets[row])) ; // Set DDRAM Adddress : 00H-07H,40H-47H
}

void aqmxx02a_cmd(char c){
	//スタートコンディションを発行する
	i2c_start();
	//LCDのアドレス
	i2c_write(0x7c);
	// control byte の送信(コマンドを指定)
	i2c_write(0b10000000);
	// data byte の送信
	i2c_write(c);
	// ストップコンディションを発行する
	i2c_stop();
	//delay_us(27);
}

void aqmxx02a_puts(char s){
	//スタートコンディションを発行する
	i2c_start();
	//LCDのアドレス
	i2c_write(0x7c);
	//control byte の送信(データを指定)
	i2c_write(0b01000000);
	//data byte の送信
	i2c_write(s);
	//ストップコンディションを発行する
	i2c_stop();
}

//オリジナルアイコン　↑
char aqmxx02a_icon_1[7] = {
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b00100,
  0b00100,
  0b00000,
};

//オリジナルアイコン　↓
char aqmxx02a_icon_2[7] = {
  0b00100,
  0b00100,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00000,
};

void LCD_CreateChar(int p,char *dt){

	int tmp1 = 0;

	i2c_start();     // スタートコンディションを発行する
	//LCDのアドレス
	i2c_write(0x7c);
	//  LCDにキャラ保存先のアドレスを指示する
	i2c_write(0b10000000) ;             // control byte の送信(コマンドを指定)
	i2c_write(0x40 | (p << 3)) ;
	//delay_us(27) ;
	//  LCDにキャラデータを送信する
	i2c_write(0b01000000) ;             // control byte の送信(データを指定)
	for (tmp1=0; tmp1 < 7; tmp1++) {
	   i2c_write(*dt++) ;
	   //delay_us(27) ;
	}

	i2c_stop() ;                            // ストップコンディションを発行する
}

