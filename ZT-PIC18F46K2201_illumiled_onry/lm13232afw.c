#define A0 PIN_C0
#define E PIN_C1
#define _RES PIN_C2

/*****************************************************************************************
グラフィックLCDモジュール(TOPWAY LM13232AFW)用ライブラリ 2016/10/29作成

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
スタートアップに入れるソースは下記の通り。
	//グラフィックLCDを表示
	//TOPWEY LM6029ACW又はLM13232AFW
	//LCD初期化
	lcd_ini();


また、更新時に使用するコードは下記の通り。

	//ロゴ表示
	lcd_bitmap8();	

	//グラフィックLCDのテキスト表示
	//TOPWEY LM13232AFW
	lcd_start(0);
	printf(lcd_char,"ZEATEC co.,ltd. ｼﾞｰﾃｯｸ");
	lcd_start(1);
	printf(lcd_char,"AN0 = %04lumV , RA1 = %u",analog_value,port1_ra1);
	lcd_start(2);
	printf(lcd_char,"TOPWEY LM13232AFW     ");
	lcd_start(3);
	printf(lcd_char,"Dot 132x32 , Text 22x4");
	lcd_start(3);
	printf(lcd_char,"%02x/%02x/%02x ",year,month,day);
	printf(lcd_char,c_week);
	printf(lcd_char," %02x:%02x:%02x ",hour,min,sec);

□接続方法
RE0 = DB0
RE1 = DB1
RE2 = DB2
RE3 = DB3
RE4 = DB4
RE5 = DB5
RE6 = DB6
RE7 = DB7
RF0 = A0 1:DisplayData 0:ControlData
RF1 = E Enable trigger
RF2 = /RES 1:Run 0:ini

上記以外の配線はLCDモジュール上またはハーネスによってVDDおよびVSSと接続する。
R/W = Vss
/CS1 = Vss
BLA = Vdd
P/S = Vdd
*****************************************************************************************/

void lcd_ini();
void lcd_cmd();
void lcd_data();
void lcd_write();

const int contrast_lavel = 0x18;

//LCDにコマンド送信
void lcd_cmd(int cmd){

	output_bit(A0,0);// A0 0:Control data.
	output_bit(E,0);// E Enable trigger
	output_a(cmd);// DB0-7
	output_bit(E,1);// E Enable trigger
	output_bit(E,0);// E Enable trigger
}

//LCDにデータ送信
void lcd_data(int cmd){

	output_bit(A0,1);// A0 0:Control data.
	output_bit(E,0);// E Enable trigger
	output_a(cmd);// DB0-7
	output_bit(E,1);// E Enable trigger
	output_bit(E,0);// E Enable trigger
}

//LCDを初期化
void lcd_ini(){

	//初期値
	output_bit(_RES,!0);// /RES 0:stop.
	output_bit(A0,1);// A0 1:Display data. 0:Control data.
	output_bit(E,0);// E Enable trigger

	//hardware reset LCD module
	output_bit(_RES,1);// /RES SET
	output_bit(_RES,0);// /RES RESET
	delay_ms(5);
	output_bit(_RES,1);// /RES SET
	delay_ms(10);
	
	lcd_cmd(0xaf);//Display ON
	lcd_cmd(0x40);//Set display Start line=0
	lcd_cmd(0xa0);//ADC = 0:Normal 1:Reverse
	lcd_cmd(0xa6);//Normal display
	lcd_cmd(0xa4);//Disible display all point
	lcd_cmd(0xa2);//Bais = 0xa2:1/9 0xa3:1/7
	lcd_cmd(0xc8);//SHL Select 0xc8:Flipped in y direction 0xc0:Normal display
	lcd_cmd(0x2f);//Power control = all on
	
	lcd_cmd(0x23);//RA/RB setting
	lcd_cmd(0xf8);//Booster Rabit = 4x
	lcd_cmd(0x00);//(tow byte command)
	lcd_cmd(0x81);//E-Vol setting
	lcd_cmd(contrast_lavel);//(tow byte command)
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

//ビットマップデータ送信
void lcd_bitmap8(){

	int tmp_data;
	long i,j;

	for(i=0;i<4;i++){
		lcd_cmd(0xb0 | i);//select page 0-3
		lcd_cmd(0x10);//start form colum 4
		lcd_cmd(0x00);//(tow byte command)
		for(j=0;j<132;j++){
			tmp_data = lcd_image8[(i*132)+j];
			lcd_data(tmp_data);
		}
	}
}

void lcd_bitmap32(){

	int32 tmp_data1;
	int tmp_data2;
	long i,j;

	for(i=0;i<4;i++){
		lcd_cmd(0xb0 | i);//select page 0-3
		lcd_cmd(0x10);//start form colum 4
		lcd_cmd(0x00);//(tow byte command)
		for(j=0;j<132;j++){
			tmp_data1 = lcd_image32[j];
			tmp_data2 = tmp_data1>>(8*i);
			lcd_data(tmp_data2);
		}
	}
}
