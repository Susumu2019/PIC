long clock_setup_step = 0;

void lcd_sw_process_clock_setup(long end_address){
	int tmp1 = 0;

	if(clock_setup_step >= 1 && clock_setup_step < 6){
		//�J�n�O����

		//���\�����Ă��镶�������ɓ�����
		for(tmp1=0;tmp1<14;tmp1++){
			lcd_line1[tmp1] = lcd_line1[tmp1+1];
			lcd_line2[tmp1] = lcd_line2[tmp1+1];
		}
		lcd_line1[15] = 0x20;
		lcd_line2[15] = 0x20;

		for(tmp1=0;tmp1<14;tmp1++){
			lcd_line1[tmp1] = lcd_line1[tmp1+1];
			lcd_line2[tmp1] = lcd_line2[tmp1+1];
		}
		lcd_line1[15] = 0x20;
		lcd_line2[15] = 0x20;

		for(tmp1=0;tmp1<14;tmp1++){
			lcd_line1[tmp1] = lcd_line1[tmp1+1];
			lcd_line2[tmp1] = lcd_line2[tmp1+1];
		}
		lcd_line1[15] = 0x20;
		lcd_line2[15] = 0x20;

		clock_setup_step++;
	}else if(clock_setup_step == 6){
		//������

		//����������
		clock_setup_step = 1001;
	}else if(clock_setup_step >= 11 && clock_setup_step < 16){
		//�ݒ胂�[�h�I���O����

		//���\�����Ă��镶�������ɓ�����
		for(tmp1=0;tmp1<14;tmp1++){
			lcd_line1[tmp1] = lcd_line1[tmp1+1];
			lcd_line2[tmp1] = lcd_line2[tmp1+1];
		}
		lcd_line1[15] = 0x20;
		lcd_line2[15] = 0x20;

		for(tmp1=0;tmp1<14;tmp1++){
			lcd_line1[tmp1] = lcd_line1[tmp1+1];
			lcd_line2[tmp1] = lcd_line2[tmp1+1];
		}
		lcd_line1[15] = 0x20;
		lcd_line2[15] = 0x20;

		for(tmp1=0;tmp1<14;tmp1++){
			lcd_line1[tmp1] = lcd_line1[tmp1+1];
			lcd_line2[tmp1] = lcd_line2[tmp1+1];
		}
		lcd_line1[15] = 0x20;
		lcd_line2[15] = 0x20;

		clock_setup_step++;
	}else if(clock_setup_step == 16){
		//�ݒ胂�[�h�I��

		clock_setup_step = 0;
		//�ЂƂO�̃X�e�b�v�ɖ߂�
		main_step = end_address;

		rtc_date_set();
	}else if(clock_setup_step >= 1000 && clock_setup_step < 2000){


		//Enter�L�[
		if(sw_1 == 1){
			sw_1 = 0;

			if((clock_setup_step%10) == 6){
				//�ݒ胂�[�h���I��
				clock_setup_step = 11;
			}else{
				//���̐ݒ荀�ڂ�
				clock_setup_step++;
			}
		}

		//UP/LEFT�L�[
		if(sw_2 == 1){
			sw_2 = 0;

			if((clock_setup_step%10) == 1){
				//�N�ݒ�
				year = hval100_up(year);
			}else if((clock_setup_step%10) == 2){
				//���ݒ�
				month = hval12_up(month);
			}else if((clock_setup_step%10) == 3){
				//���ݒ�
				day = hval31_up(day);
			}else if((clock_setup_step%10) == 4){
				//���ݒ�
				hour = hval24_up(hour);
			}else if((clock_setup_step%10) == 5){
				//���ݒ�
				min = hval60_up(min);
			}else if((clock_setup_step%10) == 6){
				//�b�ݒ�
				sec = hval60_up(sec);
			}
			
		}

		//DOWN/RIGHT�L�[
		if(sw_3 == 1){
			sw_3 = 0;

			if((clock_setup_step%10) == 1){
				//�N�ݒ�
				year = hval100_down(year);
			}else if((clock_setup_step%10) == 2){
				//���ݒ�
				month = hval12_down(month);
			}else if((clock_setup_step%10) == 3){
				//���ݒ�
				day = hval31_down(day);
			}else if((clock_setup_step%10) == 4){
				//���ݒ�
				hour = hval24_down(hour);
			}else if((clock_setup_step%10) == 5){
				//���ݒ�
				min = hval60_down(min);
			}else if((clock_setup_step%10) == 6){
				//�b�ݒ�
				sec = hval60_down(sec);
			}
			//rtc_date_set();
		}

		//�\������	
		sprintf(lcd_line1,"  20%02x/%02x/%02x    ",year,month,day);
		sprintf(lcd_line2,"      %02x:%02x:%02x  ",hour,min,sec);
		if((clock_setup_step%10) == 1 && flicker_low == 1){
				lcd_line1[4] = 0x20;
				lcd_line1[5] = 0x20;
		}else if(((clock_setup_step%10) == 2) && flicker_low == 1){
				lcd_line1[7] = 0x20;
				lcd_line1[8] = 0x20;
		}else if(((clock_setup_step%10) == 3) && flicker_low == 1){
				lcd_line1[10] = 0x20;
				lcd_line1[11] = 0x20;
		}else if(((clock_setup_step%10) == 4) && flicker_low == 1){
				lcd_line2[6] = 0x20;
				lcd_line2[7] = 0x20;
		}else if(((clock_setup_step%10) == 5) && flicker_low == 1){
				lcd_line2[9] = 0x20;
				lcd_line2[10] = 0x20;
		}else if(((clock_setup_step%10) == 6) && flicker_low == 1){
				lcd_line2[12] = 0x20;
				lcd_line2[13] = 0x20;
		}
	}
}

