int signboard_color_set = 4;
int signboard_mode_set = 0;
int signboard_speed = 1;
int led_signaltype = 0;//0=rgb,1=grb
int led_timing = 1;//タイミングパターン
int led_brightness = 50;//明るさ
int signboard_putcount = 0;//変更時のみ送信する場合の送信カウント
int signboard_putcount_max = 5;//変更時のみ送信する場合の送信回数

//#include "ws281x(64).c"//シリアル通信LED ws281x
#include "ws281x(256).c"//シリアル通信LED ws281x

void signboard_process(){
	int tmp_sw1_true = 0,tmp_sw2_true = 0,tmp_sw3_true = 0,tmp_sw4_true = 0;

	//スイッチ判定
	if(sw1_re != sw1 && sw1 == 0){	tmp_sw1_true = 1;}	sw1_re = sw1;
	if(sw2_re != sw2 && sw2 == 0){	tmp_sw2_true = 1;}	sw2_re = sw2;
	if(sw3_re != sw3 && sw3 == 0){	tmp_sw3_true = 1;}	sw3_re = sw3;
	if(sw4_re != sw4 && sw4 == 0){	tmp_sw4_true = 1;}	sw4_re = sw4;

	if(mode == 0){
		//enter
		if(tmp_sw1_true != 0){tmp_sw1_true = 0;
			//mode = 701;//	lcd_mode = mode;
			//cursor_index = 1;
			
			// save_mode(mode);//メモリーに保存

			// ws281x_color_set(signboard_color_set);
		}

		//cansel
		if(tmp_sw2_true != 0){tmp_sw2_true = 0;

		}

		//up
		if(tmp_sw3_true != 0){tmp_sw3_true = 0;
			if(signboard_color_set >= (ws281x_pattern_value-1)){		signboard_color_set = 0;
			}else{							signboard_color_set++;	}
			ws281x_color_set(signboard_color_set);
			write_eeprom(0x10,signboard_color_set);//設置値を保存
			//送信回数リセット
			//signboard_putcount = 0;
		}

		//down
		if(tmp_sw4_true != 0){tmp_sw4_true = 0;
			if(signboard_color_set == 0){		signboard_color_set = ws281x_pattern_value-1;
			}else{							signboard_color_set--;	}
			ws281x_color_set(signboard_color_set);
			write_eeprom(0x10,signboard_color_set);//設置値を保存
			//送信回数リセット
			//signboard_putcount = 0;
		}

		//LEDの信号をOUTピンから出力する
		ws281x_process();

	//}else if(mode == 701 || mode == 702){

		// if(signboard_process_one > signboard_speed){
		// 	signboard_process_one = 0;
		// 	//ws281x_mode_setの設定によって変化させる
		// 	//1=down(上から下がって回る)
		// 	//2=up(下から上がって回る) ...etc
		// 	//ws281x_mode_set(signboard_mode_set);
		// 	if(signboard_putcount < signboard_putcount_max){
		// 		signboard_putcount++;

		// 		//LEDのデータをピンから出力
		// 		ws281x_process();
		// 	}
		// }

		//enter
// 		if(tmp_sw1_true != 0){tmp_sw1_true = 0;
// /* 			if(cursor_index == 6){
// 				mode = 701;//lcd_mode = mode;
// 				cursor_index = 1;
// 				save_mode(mode);//メモリーに保存
// 			}else if(cursor_index == 5){
// 				mode = 702;//lcd_mode = mode;
// 				cursor_index = 6;
// 				save_mode(mode);//メモリーに保存
// 			}else{
// 				cursor_index++;
// 			} */
// 		}

/* 		if(cursor_index == 1){
			//signboard_color_set設定

			//up
			if(tmp_sw3_true != 0){tmp_sw3_true = 0;
				if(signboard_color_set >= (ws281x_pattern_value-1)){		signboard_color_set = 0;
				}else{							signboard_color_set++;	}
				ws281x_color_set(signboard_color_set);
				write_eeprom(0x10,signboard_color_set);//設置値を保存
				//送信回数リセット
				signboard_putcount = 0;
			}

			//down
			if(tmp_sw4_true != 0){tmp_sw4_true = 0;
				if(signboard_color_set == 0){		signboard_color_set = ws281x_pattern_value-1;
				}else{							signboard_color_set--;	}
				ws281x_color_set(signboard_color_set);
				write_eeprom(0x10,signboard_color_set);//設置値を保存
				//送信回数リセット
				signboard_putcount = 0;
			}
		}else if(cursor_index == 2){
			//signboard_mode_set

			//up
			if(tmp_sw3_true != 0){tmp_sw3_true = 0;
				if(signboard_mode_set >= 13){		signboard_mode_set = 0;
				}else{							signboard_mode_set++;	}
				ws281x_mode_set(signboard_mode_set);
				write_eeprom(0x11,signboard_mode_set);//設置値を保存

			}

			//down
			if(tmp_sw4_true != 0){tmp_sw4_true = 0;
				if(signboard_mode_set == 0){		signboard_mode_set = 13;
				}else{							signboard_mode_set--;	}
				ws281x_mode_set(signboard_mode_set);
				write_eeprom(0x11,signboard_mode_set);//設置値を保存
			}
		}else if(cursor_index == 3){
			//signboard_speed

			//up
			if(tmp_sw3_true != 0){tmp_sw3_true = 0;
				if(signboard_speed >= 99){		signboard_speed = 0;
				}else{						signboard_speed++;	}	
				write_eeprom(0x12,signboard_speed);//設置値を保存
			}

			//down
			if(tmp_sw4_true != 0){tmp_sw4_true = 0;
				if(signboard_speed == 0){		signboard_speed = 99;
				}else{						signboard_speed--;	}
				write_eeprom(0x12,signboard_speed);//設置値を保存
			}
		}else if(cursor_index == 4){
			//signboard_signaltype

			//up
			if(tmp_sw3_true != 0){tmp_sw3_true = 0;
				signboard_signaltype = !signboard_signaltype;
				write_eeprom(0x13,signboard_signaltype);//設置値を保存
			}

			//down
			if(tmp_sw4_true != 0){tmp_sw4_true = 0;
				signboard_signaltype = !signboard_signaltype;
				write_eeprom(0x13,signboard_signaltype);//設置値を保存
			}
		}else if(cursor_index == 5){
			//signboard_timing

			//up
			if(tmp_sw3_true != 0){tmp_sw3_true = 0;
				if(signboard_timing >= 3){	signboard_timing = 0;
				}else{					signboard_timing++;	}
				write_eeprom(0x14,signboard_timing);//設置値を保存
			}

			//down
			if(tmp_sw4_true != 0){tmp_sw4_true = 0;
				if(signboard_timing == 0){	signboard_timing = 3;
				}else{					signboard_timing--;	}
				write_eeprom(0x14,signboard_timing);//設置値を保存
			}
		}else if(cursor_index == 6){
			//signboard_brightness

			//up
			if(tmp_sw3_true != 0){tmp_sw3_true = 0;
				if(signboard_brightness == 100){	signboard_brightness = 0;
				}else{							signboard_brightness++;	}
				ws281x_color_set(signboard_color_set);
				write_eeprom(0x15,signboard_brightness);//設置値を保存
			}

			//down
			if(tmp_sw4_true != 0){tmp_sw4_true = 0;
				if(signboard_brightness == 0){	signboard_brightness = 100;
				}else{							signboard_brightness--;	}
				ws281x_color_set(signboard_color_set);
				write_eeprom(0x15,signboard_brightness);//設置値を保存
			}
		} */

	}



	////enter
	//if(tmp_sw1_true != 0){tmp_sw1_true = 0;
	//	

	//}
	//cansel
// 	if(tmp_sw2_true != 0){tmp_sw2_true = 0;
// /* 		mode = 10;	
// 		cursor_index = 7; */
// 	}

	////
	//if(tmp_sw3_true != 0){tmp_sw3_true = 0;

	//}

	////
	//if(tmp_sw4_true != 0){tmp_sw4_true = 0;

	//}


}