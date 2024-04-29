#include "font_data.c"//フォントデータ
#include "image_data.c"//イメージデータ

#include "lm6029acw.c"//グラフィックLCDモジュール

//LCD処理
void lcd_process(){
	int cursor[10] = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};

	//■グラフィックLCD(lm6029acw)
	//フルテキストで4.438msec(ini込み),3.932msec(更新のみ)
	if(mode == 300){
		lcd_start(0);	printf(lcd_char,"MODE:Uart        %04lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"----------------------");
		lcd_start(2);	printf(lcd_char,"uart1 value=%03u      -",get_command_value_c1);
		lcd_start(3);	printf(lcd_char,"%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c-",
			get_command_c1[0],get_command_c1[1],get_command_c1[2],get_command_c1[3],get_command_c1[4],
			get_command_c1[5],get_command_c1[6],get_command_c1[7],get_command_c1[8],get_command_c1[9],
			get_command_c1[10],get_command_c1[11],get_command_c1[12],get_command_c1[13],get_command_c1[14],
			get_command_c1[15],get_command_c1[16],get_command_c1[17],get_command_c1[18],get_command_c1[19],get_command_c1[20]);
		lcd_start(4);	printf(lcd_char,"%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c%01c-",
			get_command_c1[21],get_command_c1[22],get_command_c1[23],get_command_c1[24],get_command_c1[25],
			get_command_c1[26],get_command_c1[27],get_command_c1[28],get_command_c1[29],get_command_c1[30],
			get_command_c1[31],get_command_c1[32],get_command_c1[33],get_command_c1[34],get_command_c1[35],
			get_command_c1[36],get_command_c1[37],get_command_c1[38],get_command_c1[39],get_command_c1[40],get_command_c1[41]);
		lcd_start(5);	printf(lcd_char,"%02x,%02x,%02x,%02x,%02x,%02x,%02x -",
			get_command_c1[0],get_command_c1[1],get_command_c1[2],get_command_c1[3],get_command_c1[4],get_command_c1[5],get_command_c1[6]);
		lcd_start(6);	printf(lcd_char,"%02x,%02x,%02x,%02x,%02x,%02x,%02x -",
			get_command_c1[7],get_command_c1[8],get_command_c1[9],get_command_c1[10],get_command_c1[11],get_command_c1[12],get_command_c1[13]);
		lcd_start(7);	printf(lcd_char,"%02x,%02x,%02x,%02x,%02x,%02x,%02x -",
			get_command_c1[14],get_command_c1[15],get_command_c1[16],get_command_c1[17],get_command_c1[18],get_command_c1[19],get_command_c1[20]);
	}else if(mode == 400){
		//ロゴ表示
		lcd_bitmap8();
	}else if(mode == 600){
		cursor[cursor_index] = 0x3e;//> 0x2a;//*
		lcd_start(0);	printf(lcd_char,"MODE:Servo(OUT:A7)%03lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"----------------------");
		lcd_start(2);	printf(lcd_char,"%01cAngle = %03u %04luus  -",cursor[1],servo_angle,servo_time);
		lcd_start(3);	printf(lcd_char,"%01cTargetAngle = %03u   -",cursor[2],servo_target_angle);
		lcd_start(4);	printf(lcd_char,"%01cStepAngle = %03u     -",cursor[3],servo_step_angle);
		lcd_start(5);	printf(lcd_char,"%01cTurn = ",cursor[4]);
		if(servo_turn == 0){	
			printf(lcd_char,"OFF          -");
			lcd_start(6);
			if(servo_position == 0){printf(lcd_char,"%01c [Origin] Target    -",cursor[5]);
			}else{					printf(lcd_char,"%01c  Origin [Target]   -",cursor[5]);}
		}else{
			printf(lcd_char,"ON           -");
			lcd_start(6);
			if(servo_start == 0){	printf(lcd_char,"%01c [Stop] Start       -",cursor[5]);
			}else{					printf(lcd_char,"%01c  Stop [Start]      -",cursor[5]);}
		}
		lcd_start(7);	printf(lcd_char,"<Enter> to next.      -");
	}else if(mode == 800){
		lcd_start(0);	printf(lcd_char,"MODE:Temp&RH     %04lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"----------------------");
		lcd_start(2);	printf(lcd_char," BME280     AM2320   -");
		lcd_start(3);	printf(lcd_char," AM2322              -");
		lcd_start(4);	printf(lcd_char,"                     -");
		lcd_start(5);	printf(lcd_char,"                     -");
		lcd_start(6);	printf(lcd_char,"   Sorry.            -");
		lcd_start(7);	printf(lcd_char,"   in preparation.   -");
	}else if(mode == 900){
		lcd_start(0);	printf(lcd_char,"MODE:IR          %04lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"----------------------");
		lcd_start(2);	printf(lcd_char,"                     -");
		lcd_start(3);	printf(lcd_char,"                     -");
		lcd_start(4);	printf(lcd_char,"                     -");
		lcd_start(5);	printf(lcd_char,"                     -");
		lcd_start(6);	printf(lcd_char,"   Sorry.            -");
		lcd_start(7);	printf(lcd_char,"   in preparation.   -");
	}else if(mode >= 1000 && mode < 1100){
		lcd_start(0);	printf(lcd_char,"MODE:RFID        %04lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"----------------------");
		lcd_start(2);	printf(lcd_char,"                     -");
		lcd_start(3);	printf(lcd_char,"                     -");
		lcd_start(4);	printf(lcd_char,"                     -");
		lcd_start(5);	printf(lcd_char,"                     -");
		lcd_start(6);	printf(lcd_char,"   Sorry.            -");
		lcd_start(7);	printf(lcd_char,"   in preparation.   -");
	}else if(mode == 1100){
		lcd_start(0);	printf(lcd_char,"MODE:MP3         %04lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"----------------------");
		lcd_start(2);	printf(lcd_char," KT403A              -");
		lcd_start(3);	printf(lcd_char,"                     -");
		lcd_start(4);	printf(lcd_char,"                     -");
		lcd_start(5);	printf(lcd_char,"                     -");
		lcd_start(6);	printf(lcd_char,"   Sorry.            -");
		lcd_start(7);	printf(lcd_char,"   in preparation.   -");
	}else if(mode == 1200){
		lcd_start(0);	printf(lcd_char,"MODE:Motion     %04lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"----------------------");
		lcd_start(2);	printf(lcd_char," BMX055              -");
		lcd_start(3);	printf(lcd_char,"                     -");
		lcd_start(4);	printf(lcd_char,"                     -");
		lcd_start(5);	printf(lcd_char,"                     -");
		lcd_start(6);	printf(lcd_char,"   Sorry.            -");
		lcd_start(7);	printf(lcd_char,"   in preparation.   -");
	}else{
		lcd_start(0);	printf(lcd_char,"MODE:LCD Error   %04lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"----------------------");
		lcd_start(2);	printf(lcd_char," cursor_index=%03lu    -",cursor_index);
		lcd_start(3);	printf(lcd_char,"                     -");
		lcd_start(4);	printf(lcd_char,"                     -");
		lcd_start(5);	printf(lcd_char,"                     -");
		lcd_start(6);	printf(lcd_char,"   Sorry.            -");
		lcd_start(7);	printf(lcd_char,"   in preparation.   -");
	}

	//lcd_start(0);	printf(lcd_char,"MODE:LCD Error   %04lu%c",mode,life_char[life_index]);
	//lcd_start(1);	printf(lcd_char,"----------------------");
	//lcd_start(2);	printf(lcd_char," cursor_index=%03lu    -",cursor_index);
	//lcd_start(3);	printf(lcd_char,"                     -");
	//lcd_start(4);	printf(lcd_char,"                     -");
	//lcd_start(5);	printf(lcd_char,"                     -");
	//lcd_start(6);	printf(lcd_char,"   Sorry.            -");
	//lcd_start(7);	printf(lcd_char,"   in preparation.   -");

	//float f;
	//f=12.345;
	//printf(tostring,"\f%6.3f",f);
}

void modeselect_lcd_process(){
//フルテキストで4.438msec(ini込み),3.932msec(更新のみ)

	int cursor[20] = {0x20,0x20,0x20,0x20,0x20,	0x20,0x20,0x20,0x20,0x20,
					0x20,0x20,0x20,0x20,0x20,	0x20,0x20,0x20,0x20,0x20};
	cursor[cursor_index] = 0x3e;//> 0x2a;//*

	if(mode == 0){
		
		lcd_start(0);	printf(lcd_char,"MODE:Opening      %03lu%01c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"----------------------");
		lcd_start(2);	printf(lcd_char,"This show next time? -");
		lcd_start(3);	printf(lcd_char,"            %01cYes %01cNo -",cursor[1],cursor[2]);
		lcd_start(4);	printf(lcd_char,"                     -");
		lcd_start(5);	printf(lcd_char," How to use button.  -");
		lcd_start(6);	printf(lcd_char,"Enter Cancel Up Down -");
		lcd_start(7);	printf(lcd_char,"%01c     %01c      %01c    %01c  -",0x81,0x81,0x81,0x81);
	}else if(mode == 10){
		if(cursor_index >= 0 && cursor_index < 9){
			lcd_start(0);	printf(lcd_char,"MODE:ModeSelsct   %03lu%01c",mode,life_char[life_index]);
			lcd_start(1);	printf(lcd_char,"----------------------");
			lcd_start(2);	printf(lcd_char,"%01cHardInfo %01cRTC       -",cursor[1],cursor[5]);
			lcd_start(3);	printf(lcd_char,"%01cIO       %01cServo     -",cursor[2],cursor[6]);
			lcd_start(4);	printf(lcd_char,"%01cUART     %01cIllumiLED -",cursor[3],cursor[7]);
			lcd_start(5);	printf(lcd_char,"%01cGraphic  %01cTemp&RH   -",cursor[4],cursor[8]);
			lcd_start(6);	printf(lcd_char,"<CANSEL>Back to      -");
			lcd_start(7);	printf(lcd_char,"    this mode select.-");
		}else if(cursor_index >= 9){
			lcd_start(0);	printf(lcd_char,"MODE:ModeSelsct   %03lu%01c",mode,life_char[life_index]);
			lcd_start(1);	printf(lcd_char,"----------------------");
			lcd_start(2);	printf(lcd_char,"%01cIR                  -",cursor[9]);
			lcd_start(3);	printf(lcd_char,"%01cRFID                -",cursor[10]);
			lcd_start(4);	printf(lcd_char,"%01cMP3                 -",cursor[11]);
			lcd_start(5);	printf(lcd_char,"%01cMotion              -",cursor[12]);
			lcd_start(6);	printf(lcd_char,"<CANSEL>Back to      -");
			lcd_start(7);	printf(lcd_char,"    this mode select.-");
		}
	}
}

void hardwareinfo_lcd_process(){
	int cursor[11] = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	cursor[cursor_index] = 0x3e;//> 0x2a;//*
	if(mode == 100){
		lcd_start(0);	printf(lcd_char,"MODE:HardwareInfo %03lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"----------------------");
		lcd_start(2);	printf(lcd_char,"CPU=Microchip        -");
		lcd_start(3);	printf(lcd_char,"    PIC18F46K22      -");
		lcd_start(4);	printf(lcd_char,"LCD=TOPWAY LM6029SCW -");
		lcd_start(5);	printf(lcd_char,"Voltage:3.3V         -");
		lcd_start(6);	printf(lcd_char,"Dot:128x64 Text:21x8 -");
		lcd_start(7);	printf(lcd_char,"               %01cNEXT -",cursor[1]);
	}else if(mode == 101){
		lcd_start(0);	printf(lcd_char,"MODE:HardwareInfo %03lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"----------------------");
		lcd_start(2);	printf(lcd_char,"Producer=ZEATEC      -");
		lcd_start(3);	printf(lcd_char,"Template software    -");
		lcd_start(4);	printf(lcd_char,"     Ver=%s-",ver);
		lcd_start(5);	printf(lcd_char,"        %01cGoto Opening-",cursor[1]);
		lcd_start(6);	printf(lcd_char,"Please software      -");
		lcd_start(7);	printf(lcd_char,"       use it freely!-");
	}
}

void io_lcd_process(){
	if(mode == 200){
		lcd_start(0);	printf(lcd_char,"MODE:IO          %04lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"----------------------");
		lcd_start(2);	printf(lcd_char,"I･O ra7-0=%01u%01u%01u%01u,%01u%01u%01u%01u   -",ra7,ra6,ra5,ra4,ra3,ra2,ra1,ra0);
		lcd_start(3);	printf(lcd_char,"OUT rb5-0=%01u%01u%01u%01u%01u%01u     -",rb5,rb4,rb3,rb2,rb1,rb0);//,rb6,rb7
		lcd_start(4);	printf(lcd_char,"AN re1-0=%04lu,%04lu   -",re1,re0);
		lcd_start(5);	printf(lcd_char,"                     -");
		lcd_start(6);	printf(lcd_char,"                     -");
		lcd_start(7);	printf(lcd_char,"                     -");
	}
}
//イルミネーションLED
void illumiled_lcd_process(){

	int cursor[10] = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	cursor[cursor_index] = 0x3e;//0x3e=">",0x2a="*"

	if(mode == 700){
		lcd_start(0);	printf(lcd_char,"MODE:IllumiLED    %03lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"----------------------");
		lcd_start(2);	printf(lcd_char,"%01cWS2812b,2811(OUT:A6)-",cursor[1]);
		lcd_start(3);	printf(lcd_char,"                     -");
		lcd_start(4);	printf(lcd_char,"                     -");
		lcd_start(5);	printf(lcd_char,"                     -");
		lcd_start(6);	printf(lcd_char,"                     -");
		lcd_start(7);	printf(lcd_char,"                     -");
	}else if(mode == 701){
		lcd_start(0);	printf(lcd_char,"MODE:WS2812b,2811 %03lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"----------------------");		
		lcd_start(2);	printf(lcd_char," led_value=%03lu       -",ws281x_led_value);
		lcd_start(3);	printf(lcd_char,"%01ccolor_set=%02u        -",cursor[1],Illumi_color_set);
		lcd_start(4);	printf(lcd_char,"%01cmode_set=%02u         -",cursor[2],Illumi_mode_set);
		lcd_start(5);	printf(lcd_char,"%01cspeed=%02u            -",cursor[3],Illumi_speed);
		lcd_start(6);	printf(lcd_char,"%01csignaltype=%02u       -",cursor[4],Illumi_signaltype);
		lcd_start(7);	printf(lcd_char,"%01ctiming=%02u           -",cursor[5],Illumi_timing);
	}else if(mode == 702){
		lcd_start(0);	printf(lcd_char,"MODE:WS2812b,2811 %03lu%c",mode,life_char[life_index]);
		lcd_start(1);	printf(lcd_char,"----------------------");		
		lcd_start(2);	printf(lcd_char,"%01cbrightness=%03u      -",cursor[6],Illumi_brightness);
		lcd_start(3);	printf(lcd_char,"                     -");
		lcd_start(4);	printf(lcd_char,"                     -");
		lcd_start(5);	printf(lcd_char,"                     -");
		lcd_start(6);	printf(lcd_char,"                     -");
		lcd_start(7);	printf(lcd_char,"                     -");
	}
}

void rtc_lcd_process(){

	char *tmp_week[] = {"SUN","MON","TUE","WED","THU","FRI","SAT","xxx"};
	
	lcd_start(0);	printf(lcd_char,"MODE:RTC(RX8900)  %03lu%c",mode,life_char[life_index]);
	lcd_start(1);	printf(lcd_char,"Add:W/R=0x64/0x65    -");
	if(mode == 500){
		lcd_start(2);	printf(lcd_char,"DATE:20%02x/%02x/%02x ",year,month,day);
		printf(lcd_char,"%s  -",tmp_week[week2index(week)]);
		lcd_start(3);	printf(lcd_char,"TIME:%02x:%02x:%02x        -",hour,min,sec);
		lcd_start(6);	printf(lcd_char,"                     -");
		lcd_start(7);	printf(lcd_char,"<Enter> to TimeEdit. -");
	}else if(mode == 501){
		lcd_start(2);
		//年数を編集
		if(flicker_500 == 0){	printf(lcd_char,"DATE:    /%02x/%02x ",month,day);//点滅処理
		}else{					printf(lcd_char,"DATE:20%02x/%02x/%02x ",year,month,day);}
		printf(lcd_char,"%s  -",tmp_week[week2index(week)]);
		lcd_start(3);	printf(lcd_char,"TIME:%02x:%02x:%02x        -",hour,min,sec);
		lcd_start(6);	printf(lcd_char,"                     -");
		lcd_start(7);	printf(lcd_char,"<Enter> to Next.     -");
	}else if(mode == 502){
		lcd_start(2);
		//月を編集
		if(flicker_500 == 0){	printf(lcd_char,"DATE:20%02x/  /%02x ",year,day);//点滅処理
		}else{					printf(lcd_char,"DATE:20%02x/%02x/%02x ",year,month,day);}
		printf(lcd_char,"%s  -",tmp_week[week2index(week)]);
		lcd_start(3);	printf(lcd_char,"TIME:%02x:%02x:%02x        -",hour,min,sec);
		lcd_start(6);	printf(lcd_char,"                     -");
		lcd_start(7);	printf(lcd_char,"<Enter> to Next.     -");
	}else if(mode == 503){
		lcd_start(2);
		//日を編集
		if(flicker_500 == 0){	printf(lcd_char,"DATE:20%02x/%02x/   ",year,month);//点滅処理
		}else{					printf(lcd_char,"DATE:20%02x/%02x/%02x ",year,month,day);}
		printf(lcd_char,"%s  -",tmp_week[week2index(week)]);
		lcd_start(3);	printf(lcd_char,"TIME:%02x:%02x:%02x        -",hour,min,sec);
		lcd_start(6);	printf(lcd_char,"                     -");
		lcd_start(7);	printf(lcd_char,"<Enter> to Next.     -");
	}else if(mode == 504){
		lcd_start(2);
		//曜日を編集
		printf(lcd_char,"DATE:20%02x/%02x/%02x ",year,month,day);
		if(flicker_500 == 0){	printf(lcd_char,"     -");//点滅処理
		}else{					printf(lcd_char,"%s  -",tmp_week[week2index(week)]);}
		lcd_start(3);	printf(lcd_char,"TIME:%02x:%02x:%02x        -",hour,min,sec);
		lcd_start(6);	printf(lcd_char,"                     -");
		lcd_start(7);	printf(lcd_char,"<Enter> to Next.     -");
	}else if(mode == 505){
		lcd_start(2);
		//時間を編集
		printf(lcd_char,"DATE:20%02x/%02x/%02x ",year,month,day);
		printf(lcd_char,"%s  -",tmp_week[week2index(week)]);
		lcd_start(3);
		if(flicker_500 == 0){	printf(lcd_char,"TIME:  :%02x:%02x        -",min,sec);//点滅処理
		}else{					printf(lcd_char,"TIME:%02x:%02x:%02x        -",hour,min,sec);}
		lcd_start(6);	printf(lcd_char,"                     -");
		lcd_start(7);	printf(lcd_char,"<Enter> to Next.     -");
	}else if(mode == 506){
		lcd_start(2);
		//分を編集
		printf(lcd_char,"DATE:20%02x/%02x/%02x ",year,month,day);
		printf(lcd_char,"%s  -",tmp_week[week2index(week)]);
		lcd_start(3);
		if(flicker_500 == 0){	printf(lcd_char,"TIME:%02x:  :%02x        -",hour,sec);//点滅処理
		}else{					printf(lcd_char,"TIME:%02x:%02x:%02x        -",hour,min,sec);}
		lcd_start(6);	printf(lcd_char,"                     -");
		lcd_start(7);	printf(lcd_char,"<Enter> to Next.     -");
	}else if(mode == 507){
		lcd_start(2);
		//秒を編集
		printf(lcd_char,"DATE:20%02x/%02x/%02x ",year,month,day);
		printf(lcd_char,"%s  -",tmp_week[week2index(week)]);
		lcd_start(3);
		if(flicker_500 == 0){	printf(lcd_char,"TIME:%02x:%02x:          -",hour,min);//点滅処理
		}else{					printf(lcd_char,"TIME:%02x:%02x:%02x        -",hour,min,sec);}
		lcd_start(6);	printf(lcd_char,"                     -");
		lcd_start(7);	printf(lcd_char,"<Enter> to Next.     -");
	}else if(mode == 510){
		lcd_start(6);	printf(lcd_char,"Save?                -");
		lcd_start(7);	printf(lcd_char,"<Enter> or <Cancel>  -");
	}
	lcd_start(4);	printf(lcd_char,"                     -");
	lcd_start(5);	printf(lcd_char,"                     -");
}