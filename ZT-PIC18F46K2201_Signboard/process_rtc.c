int sec     = 0x00;//0x00-0x59
int min     = 0x00;//0x00-0x59
int hour    = 0x00;//0x00-0x23
unsigned char week    = 0x01;//SUN=0x01,MON=0x02,TUE=0x04,WED=0x08,THU=0x10,FRI=0x20,SAT=0x40
unsigned char day     = 0x01;//0x00-0x28～0x31)
unsigned char month   = 0x01;//JUN=0x01,FEB=0x02,MAR=0x03,APR=0x04,MAY=0x05,JUN=0x06,JLY=0x07,AUG=0x08,SEP=0x09,OCT=0x10,NOV=0x11,DEC=0x12
unsigned char year    = 0x19;//0x00-0x99

//void rtc_date_set();
//void rtc_date_read();
int bcd2i(int bcd);
int i2bcd(int i);
char week2index(int i);
char index2week(int i);

#include "rx8900.c"//RTC(時計) EPSON RX8900

void rtc_process(){
	int tmp1 = 0;
	int tmp_sw1_true = 0,tmp_sw2_true = 0,tmp_sw3_true = 0,tmp_sw4_true = 0;

	//モードセレクト画面を表示
	//lcd_mode = 500;

	//スイッチ判定
	if(sw1_re != sw1 && sw1 == 0){	tmp_sw1_true = 1;}	sw1_re = sw1;
	if(sw2_re != sw2 && sw2 == 0){	tmp_sw2_true = 1;}	sw2_re = sw2;
	if(sw3_re != sw3 && sw3 == 0){	tmp_sw3_true = 1;}	sw3_re = sw3;
	if(sw4_re != sw4 && sw4 == 0){	tmp_sw4_true = 1;}	sw4_re = sw4;

	if(mode == 500){
		//時計処理
		if(rtc_process_one != 0){rtc_process_one = 0;
			rtc_date_read();//日時の読み込み
		}

		//enter
		if(tmp_sw1_true != 0){tmp_sw1_true = 0;
			mode = 501;	//lcd_mode = mode;
		}
	}else if(mode == 501){
		//年を修正
		if(tmp_sw3_true != 0){	tmp_sw3_true = 0;
			//UP処理
			tmp1 = bcd2i(year);
			if(tmp1 == 99){	tmp1 = 0;	}else{	tmp1++;	}
			year = i2bcd(tmp1);
		}
		if(tmp_sw4_true != 0){	tmp_sw4_true = 0;
			//DOWN処理
			year = i2bcd(bcd2i(year)-1);
		}
		
		//enter
		if(tmp_sw1_true != 0){tmp_sw1_true = 0;
			mode = 502;	//lcd_mode = mode;
		}
	}else if(mode == 502){
		//月を修正
		if(tmp_sw3_true != 0){	tmp_sw3_true = 0;
			//UP処理
			tmp1 = bcd2i(month);
			if(tmp1 >= 12){	tmp1 = 1;	}else{	tmp1++;	}
			month = i2bcd(tmp1);
		}
		if(tmp_sw4_true != 0){	tmp_sw4_true = 0;
			//DOWN処理
			tmp1 = bcd2i(month);
			if(tmp1 <= 1){	tmp1 = 12;	}else{	tmp1--;	}
			month = i2bcd(tmp1);
		}
		
		//enter
		if(tmp_sw1_true != 0){tmp_sw1_true = 0;
			mode = 503;	//lcd_mode = mode;
		}
	}else if(mode == 503){
		//日を修正
		if(tmp_sw3_true != 0){	tmp_sw3_true = 0;
			//UP処理
			tmp1 = bcd2i(day);
			if(tmp1 >= 31){	tmp1 = 1;	}else{	tmp1++;	}
			day = i2bcd(tmp1);
		}
		if(tmp_sw4_true != 0){	tmp_sw4_true = 0;
			//DOWN処理
			tmp1 = bcd2i(day);
			if(tmp1 <= 1){	tmp1 = 31;	}else{	tmp1--;	}
			day = i2bcd(tmp1);
		}
		
		//enter
		if(tmp_sw1_true != 0){tmp_sw1_true = 0;
			mode = 504;	//lcd_mode = mode;
		}
	}else if(mode == 504){
		//曜日
		if(tmp_sw3_true != 0){	tmp_sw3_true = 0;
			//UP処理
			tmp1 = week2index(week);
			if(tmp1 >= 6){	tmp1 = 0;	}else{	tmp1++;	}
			week = index2week(tmp1);
		}
		if(tmp_sw4_true != 0){	tmp_sw4_true = 0;
			//DOWN処理
			tmp1 = week2index(week);
			if(tmp1 <= 0){	tmp1 = 6;	}else{	tmp1--;	}
			week = index2week(tmp1);
		}
		//enter
		if(tmp_sw1_true != 0){tmp_sw1_true = 0;
			mode = 505;	//lcd_mode = mode;
		}
	}else if(mode == 505){
		//時間を修正
		if(tmp_sw3_true != 0){	tmp_sw3_true = 0;
			//UP処理
			tmp1 = bcd2i(hour);
			if(tmp1 >= 23){	tmp1 = 0;	}else{	tmp1++;	}
			hour = i2bcd(tmp1);
		}
		if(tmp_sw4_true != 0){	tmp_sw4_true = 0;
			//DOWN処理
			tmp1 = bcd2i(hour);
			if(tmp1 <= 0){	tmp1 = 23;	}else{	tmp1--;	}
			hour = i2bcd(tmp1);
		}
		
		//enter
		if(tmp_sw1_true != 0){tmp_sw1_true = 0;
			mode = 506;	//lcd_mode = mode;
		}
	}else if(mode == 506){
		//分を修正
		if(tmp_sw3_true != 0){	tmp_sw3_true = 0;
			//UP処理
			tmp1 = bcd2i(min);
			if(tmp1 >= 59){	tmp1 = 0;	}else{	tmp1++;	}
			min = i2bcd(tmp1);
		}
		if(tmp_sw4_true != 0){	tmp_sw4_true = 0;
			//DOWN処理
			tmp1 = bcd2i(min);
			if(tmp1 <= 0){	tmp1 = 59;	}else{	tmp1--;	}
			min = i2bcd(tmp1);
		}
		
		//enter
		if(tmp_sw1_true != 0){tmp_sw1_true = 0;
			mode = 507;//	lcd_mode = mode;
		}
	}else if(mode == 507){
		//秒を修正
		if(tmp_sw3_true != 0){	tmp_sw3_true = 0;
			//UP処理
			tmp1 = bcd2i(sec);
			if(tmp1 >= 59){	tmp1 = 0;	}else{	tmp1++;	}
			sec = i2bcd(tmp1);
		}
		if(tmp_sw4_true != 0){	tmp_sw4_true = 0;
			//DOWN処理
			tmp1 = bcd2i(sec);
			if(tmp1 <= 0){	tmp1 = 59;	}else{	tmp1--;	}
			sec = i2bcd(tmp1);
		}
		
		//enter
		if(tmp_sw1_true != 0){tmp_sw1_true = 0;
			mode = 510;	//lcd_mode = mode;
		}
	}else if(mode == 510){
		//更新の確認

		//enter
		if(tmp_sw1_true != 0){tmp_sw1_true = 0;
			rtc_date_set();//日時を保存
			mode = 500;	//lcd_mode = mode;
		}
		//cancel
		if(tmp_sw2_true != 0){tmp_sw2_true = 0;
			mode = 500;	//lcd_mode = mode;
		}
	}

	//cansel
	if(tmp_sw2_true != 0){tmp_sw2_true = 0;
		mode = 10;
		//lcd_mode = mode;
		cursor_index = 5;
	}
}

//10進数を16進数表記している数字を10進数に変換 例:0x19→19
int bcd2i(int bcd){	return (((bcd - (bcd % 16))/16)*10)+(bcd % 16);}

//10進数を16進数表記の10進数に変換 例:19→0x19
int i2bcd(int i){return (((i - (i % 10))/10)*16)+(i % 10);}
//曜日のインデックスに変換
char week2index(int i){
	int tmp1 = 0;

	if(i == 0x01){			tmp1 = 0;
	}else if(i == 0x02){	tmp1 = 1;
	}else if(i == 0x04){	tmp1 = 2;
	}else if(i == 0x08){	tmp1 = 3;
	}else if(i == 0x10){	tmp1 = 4;
	}else if(i == 0x20){	tmp1 = 5;
	}else if(i == 0x40){	tmp1 = 6;
	}else{					tmp1 = 7;
	}
	return tmp1;
}
//曜日のインデックスに変換
char index2week(int i){
	int tmp1 = 0;

	if(i == 0){			tmp1 = 0x01;
	}else if(i == 1){	tmp1 = 0x02;
	}else if(i == 2){	tmp1 = 0x04;
	}else if(i == 3){	tmp1 = 0x08;
	}else if(i == 4){	tmp1 = 0x10;
	}else if(i == 5){	tmp1 = 0x20;
	}else if(i == 6){	tmp1 = 0x40;
	}else{				tmp1 = 0x01;
	}
	return tmp1;
}
