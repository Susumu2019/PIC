int Illumi_color_set = 0;
int Illumi_mode_set = 0;
int Illumi_speed = 1;
int Illumi_signaltype = 0;//0=rgb,1=grb
int Illumi_timing = 0;//�^�C�~���O�p�^�[��
int Illumi_brightness = 100;//���邳

//#include "ws281x(64).c"//�V���A���ʐMLED ws281x
#include "ws281x(256).c"//�V���A���ʐMLED ws281x

void illumiled_process(){
	int tmp_sw1_true = 0,tmp_sw2_true = 0,tmp_sw3_true = 0,tmp_sw4_true = 0;

	//�X�C�b�`����
	if(sw1_re != sw1 && sw1 == 0){	tmp_sw1_true = 1;}	sw1_re = sw1;
	if(sw2_re != sw2 && sw2 == 0){	tmp_sw2_true = 1;}	sw2_re = sw2;
	if(sw3_re != sw3 && sw3 == 0){	tmp_sw3_true = 1;}	sw3_re = sw3;
	if(sw4_re != sw4 && sw4 == 0){	tmp_sw4_true = 1;}	sw4_re = sw4;

	if(mode == 700){
		//enter
		if(tmp_sw1_true != 0){tmp_sw1_true = 0;
			mode = 701;//	lcd_mode = mode;
			cursor_index = 1;
			
			save_mode(mode);//�������[�ɕۑ�

			ws281x_color_set(Illumi_color_set);
		}
	}else if(mode == 701 || mode == 702){

		if(illumiled_process_one > Illumi_speed){
			illumiled_process_one = 0;

			//ws281x_mode_set�̐ݒ�ɂ���ĕω�������
			//1=down(�ォ�牺�����ĉ��)
			//2=up(������オ���ĉ��) ...etc
			ws281x_mode_set(Illumi_mode_set);
			//LED�̃f�[�^���s������o��
			ws281x_process();
		}

		//enter
		if(tmp_sw1_true != 0){tmp_sw1_true = 0;
			if(cursor_index == 6){
				mode = 701;//lcd_mode = mode;
				cursor_index = 1;
				save_mode(mode);//�������[�ɕۑ�
			}else if(cursor_index == 5){
				mode = 702;//lcd_mode = mode;
				cursor_index = 6;
				save_mode(mode);//�������[�ɕۑ�
			}else{
				cursor_index++;
			}
		}

		if(cursor_index == 1){
			//Illumi_color_set�ݒ�

			//up
			if(tmp_sw3_true != 0){tmp_sw3_true = 0;
				if(Illumi_color_set >= (ws281x_pattern_value-1)){		Illumi_color_set = 0;
				}else{							Illumi_color_set++;	}
				ws281x_color_set(Illumi_color_set);
				write_eeprom(0x10,Illumi_color_set);//�ݒu�l��ۑ�
			}

			//down
			if(tmp_sw4_true != 0){tmp_sw4_true = 0;
				if(Illumi_color_set == 0){		Illumi_color_set = ws281x_pattern_value-1;
				}else{							Illumi_color_set--;	}
				ws281x_color_set(Illumi_color_set);
				write_eeprom(0x10,Illumi_color_set);//�ݒu�l��ۑ�
			}
		}else if(cursor_index == 2){
			//Illumi_mode_set

			//up
			if(tmp_sw3_true != 0){tmp_sw3_true = 0;
				if(Illumi_mode_set >= 13){		Illumi_mode_set = 0;
				}else{							Illumi_mode_set++;	}
				ws281x_mode_set(Illumi_mode_set);
				write_eeprom(0x11,Illumi_mode_set);//�ݒu�l��ۑ�

			}

			//down
			if(tmp_sw4_true != 0){tmp_sw4_true = 0;
				if(Illumi_mode_set == 0){		Illumi_mode_set = 13;
				}else{							Illumi_mode_set--;	}
				ws281x_mode_set(Illumi_mode_set);
				write_eeprom(0x11,Illumi_mode_set);//�ݒu�l��ۑ�
			}
		}else if(cursor_index == 3){
			//Illumi_speed

			//up
			if(tmp_sw3_true != 0){tmp_sw3_true = 0;
				if(Illumi_speed >= 99){		Illumi_speed = 0;
				}else{						Illumi_speed++;	}	
				write_eeprom(0x12,Illumi_speed);//�ݒu�l��ۑ�
			}

			//down
			if(tmp_sw4_true != 0){tmp_sw4_true = 0;
				if(Illumi_speed == 0){		Illumi_speed = 99;
				}else{						Illumi_speed--;	}
				write_eeprom(0x12,Illumi_speed);//�ݒu�l��ۑ�
			}
		}else if(cursor_index == 4){
			//Illumi_signaltype

			//up
			if(tmp_sw3_true != 0){tmp_sw3_true = 0;
				Illumi_signaltype = !Illumi_signaltype;
				write_eeprom(0x13,Illumi_signaltype);//�ݒu�l��ۑ�
			}

			//down
			if(tmp_sw4_true != 0){tmp_sw4_true = 0;
				Illumi_signaltype = !Illumi_signaltype;
				write_eeprom(0x13,Illumi_signaltype);//�ݒu�l��ۑ�
			}
		}else if(cursor_index == 5){
			//Illumi_timing

			//up
			if(tmp_sw3_true != 0){tmp_sw3_true = 0;
				if(Illumi_timing >= 2){	Illumi_timing = 0;
				}else{					Illumi_timing++;	}
				write_eeprom(0x14,Illumi_timing);//�ݒu�l��ۑ�
			}

			//down
			if(tmp_sw4_true != 0){tmp_sw4_true = 0;
				if(Illumi_timing == 0){	Illumi_timing = 2;
				}else{					Illumi_timing--;	}
				write_eeprom(0x14,Illumi_timing);//�ݒu�l��ۑ�
			}
		}else if(cursor_index == 6){
			//Illumi_brightness

			//up
			if(tmp_sw3_true != 0){tmp_sw3_true = 0;
				if(Illumi_brightness == 100){	Illumi_brightness = 0;
				}else{							Illumi_brightness++;	}
				ws281x_color_set(Illumi_color_set);
				write_eeprom(0x15,Illumi_brightness);//�ݒu�l��ۑ�
			}

			//down
			if(tmp_sw4_true != 0){tmp_sw4_true = 0;
				if(Illumi_brightness == 0){	Illumi_brightness = 100;
				}else{							Illumi_brightness--;	}
				ws281x_color_set(Illumi_color_set);
				write_eeprom(0x15,Illumi_brightness);//�ݒu�l��ۑ�
			}
		}

	}



	////enter
	//if(tmp_sw1_true != 0){tmp_sw1_true = 0;
	//	

	//}
	//cansel
	if(tmp_sw2_true != 0){tmp_sw2_true = 0;
		mode = 10;	
		cursor_index = 7;
	}

	////
	//if(tmp_sw3_true != 0){tmp_sw3_true = 0;

	//}

	////
	//if(tmp_sw4_true != 0){tmp_sw4_true = 0;

	//}


}