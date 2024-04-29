/*****************************************************************************************
グラフィックLCDモジュール(TOPWAY LM6029acw)用ライブラリ 2016/4/1作成

□提供元
ZEATEC co.,ltd. 

□ご利用について
転載・無断使用可です。このライブラリを使用した上での不具合等に関しては、いかなる内容におい
ても一切の責任を追わないものとします。

□使用方法
lcd_ini()をスタートアップに入れ、用途に合わせて下記を定期的（0.1sec単位ぐらい）に実行する。
フォントのデータはfont_data.cを修正することで記号なども追加できます。イメージデータの場合は、
image_data.cデータを修正してください。１バイト単位のデータ作成は伊藤 哲雅様作のLCD_Imageが
便利です。

１バイト単位のビットマップデータの場合	lcd_bitmap8();
４バイト単位のビットマップデータの場合	lcd_bitmap32();
テキストの場合は行番号を指定してからテキストを送信。
lcd_start(0);
printf(lcd_char,"///////////////////// ");
lcd_start(1);
printf(lcd_char,"/  ZEATEC co.,ltd.  / ");
lcd_start(2);
printf(lcd_char,"/      ｼﾞｰﾃｯｸ       / ");
lcd_start(3);
printf(lcd_char,"/                   / ");
lcd_start(4);
printf(lcd_char,"/AN0=%04lumV , RA1=%u / ",analog_value,port1_ra1);
lcd_start(5);
printf(lcd_char,"/TOPWEY LM6029ACW   / ");
lcd_start(6);
printf(lcd_char,"/Dot128x64,Text21x8 / ");
lcd_start(7);
printf(lcd_char,"///////////////////// ");

□接続方法
RE0 = DB0
RE1 = DB1
RE2 = DB2
RE3 = DB3
RE4 = DB4
RE5 = DB5
RE6 = DB6
RE7 = DB7
RF0 = /WR
RF1 = RS
RF2 = /RES

上記以外の配線はLCDモジュール上またはハーネスによってVDDおよびVSSと接続する。
/CS1 = Vss
/RD = VDD
BLA = Vdd

*****************************************************************************************/

#define _WR PIN_C0
#define RS PIN_C1
#define _RES PIN_C2
#define BLA PIN_C5

int contrast_lavel = 0x16;
void lcd_start(int row);
void lcd_char(char c);

//LCDにコマンド送信
void lcd_cmd(int cmd){

	output_bit(RS,0);// RS 0:Control data.
	output_d(cmd);// DB0-7
	output_bit(_WR,!1);// /WR 0:Write enable input active.
	//delay_us(1);//
	output_bit(_WR,!0);// /WR 0:Write enable input active.
}

//LCDにデータ送信
void lcd_data(int cmd){

	output_bit(RS,1);// RS 1:Display data.
	output_d(cmd);// DB0-7
	output_bit(_WR,!1);// /WR 0:Write enable input active.
	//delay_us(1);//
	output_bit(_WR,!0);// /WR 0:Write enable input active.
}

//LCDを初期化
void lcd_ini(){

	//初期値
	output_bit(_RES,!0);// /RES 0:stop.
	output_bit(RS,1);// RS 1:Display data. 0:Control data.
	output_bit(_WR,!0);// /WR 0:Write enable input active.

	//hardware reset LCD module
	output_bit(_RES,1);// /RES SET
	output_bit(_RES,0);// /RES RESET
	delay_ms(5);
	output_bit(_RES,1);// /RES SET
	delay_us(3);
	
	//contrast_lavel = 0x20;//Default Contrast Level

	lcd_cmd(0xaf);//1.Display ON
	lcd_cmd(0x40);//2.Set display Start line=0
	lcd_cmd(0xa0);//3.ADC = 0:Normal 1:Reverse
	lcd_cmd(0xa6);//4.Normal display
	lcd_cmd(0xa4);//5.Disible display all point
	lcd_cmd(0xa2);//6.Bais = 0xa2:1/9 0xa3:1/7
	lcd_cmd(0xc8);//7.SHL Select 0xc8:Flipped in y direction 0xc0:Normal display
	lcd_cmd(0x2f);//8.Power control = all on
	
	lcd_cmd(0x25);//9.RA/RB setting
	//lcd_cmd(0xf8);//Booster Rabit = 4x
	//lcd_cmd(0x00);//(tow byte command)
	lcd_cmd(0x81);//10.E-Vol setting
	lcd_cmd(contrast_lavel);//11.(tow byte command)

}

void lcd_start(int row){
	lcd_cmd(0xb0+row);//select page 0-3
	lcd_cmd(0x10);//start form colum 4
	lcd_cmd(0x00);//(tow byte command)
}

void lcd_char(char c){
	lcd_data(font[c-0x20][0]);
	lcd_data(font[c-0x20][1]);
	lcd_data(font[c-0x20][2]);
	lcd_data(font[c-0x20][3]);
	lcd_data(font[c-0x20][4]);
	lcd_data(0x00);
}

//表示データ送信
void lcd_bitmap8(){

	int tmp_data;
	long i,j;

	for(i=0;i<8;i++){
		lcd_cmd(0xb0 | i);//select page 0-7
		lcd_cmd(0x10);//start form colum 4
		lcd_cmd(0x00);//(tow byte command)
		for(j=0;j<128;j++){
			tmp_data = lcd_image8[(i*128)+j];
			lcd_data(tmp_data);
		}
	}
}
void lcd_bla(int on){
	output_bit(BLA,on);//
}





