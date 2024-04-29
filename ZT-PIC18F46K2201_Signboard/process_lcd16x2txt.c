//#include "so1602awxb.c"//有機ELキャラクタモジュール
#include "aqmxx02a.c"//キャラクタLCDモジュール

void lcd_process(){
	//■キャラクタLCD(AQM1602A)
	//if(mode <= 1){
	//	//ロゴ表示
	//	if(lcd_row == 0){	lcd_row = 1;
	//		aqmxx02a_setcursor(0,0);
	//		printf(aqmxx02a_puts,"TemplateSoftware");
	//	}else if(lcd_row == 1){	lcd_row = 0;
	//		aqmxx02a_setcursor(0,1);
	//		printf(aqmxx02a_puts," ZEATEC co.,ltd.");
	//	}
	//}else if(mode == 10){
	//	if(lcd_row == 0){	lcd_row = 1;
	//		aqmxx02a_setcursor(0,0);
	//		printf(aqmxx02a_puts,"re0-1=%04lu,%04lu ",re0,re1);
	//	}else if(lcd_row == 1){	lcd_row = 0;
	//		aqmxx02a_setcursor(0,1);
	//		printf(aqmxx02a_puts,"ra0-3=%01u,%01u,%01u,%01u  %c",ra0,ra1,ra2,ra3,life_char[life_index]);
	//	}
	//}

	lcd_start(0);printf(lcd_char,"ZEATEC co.,ltd.%c",life_char[life_index]);
	lcd_start(1);printf(lcd_char," PIC16F46K22+LCD");

	//float f;
	//f=12.345;
	//printf(tostring,"\f%6.3f",f);
}
void signboard_lcd_process(){

	int cursor[10] = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	cursor[cursor_index] = 0x3e;//0x3e=">",0x2a="*"

	if(mode == 0){
		lcd_start(0);printf(lcd_char,"Signboard UNIT%c",life_char[life_index]);
		//lcd_start(0);	printf(lcd_char,"Mode=%03lu    %c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"PatternNo=%02u,%01u,%01u",signboard_color_set,led_timing,led_signaltype);
	}else if(mode == 1){
		lcd_start(0);	printf(lcd_char,"                 %03lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char," cWS2812b,2811(OUT:B0)-");
	}else if(mode == 2){
		lcd_start(0);	printf(lcd_char,"                 %03lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char," cWS2812b,2811(OUT:B0)-");
	}
}
void modeselect_lcd_process(){

	int cursor[10] = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	cursor[cursor_index] = 0x3e;//> 0x2a;//*
	
	lcd_start(0);printf(lcd_char,"%01cInfo           ",cursor[1]);
	lcd_start(1);printf(lcd_char,"%01cIO             ",cursor[2]);

}

void hardwareinfo_lcd_process(){
	if(lcd_mode == 100){
		lcd_start(0);	printf(lcd_char,"CPU=PIC18F46K22 ");
		lcd_start(1);	printf(lcd_char,"LCD=AQM1602XA   ");
	}
}

void io_lcd_process(){
	if(lcd_mode == 200){
		lcd_start(0);	printf(lcd_char,"ra7-0=%01u%01u%01u%01u,%01u%01u%01u%01u ",ra7,ra6,ra5,ra4,ra3,ra2,ra1,ra0);
		//lcd_start(1);	printf(lcd_char,"rb5-0=%01u%01u%01u%01u%01u%01u    ",rb5,rb4,rb3,rb2,rb1,rb0);//,rb6,rb7
		lcd_start(1);	printf(lcd_char,"re1-0=%04lu,%04lumV",re1,re0);
	}
}
