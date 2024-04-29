//�A���[���ݒ�֌W
long alarm_setup_step = 0;//
int setup_alarm_id = 0x01;
int setup_alarm_week = 0x01;
int setup_alarm_time = 0x01;
int setup_alarm_ch = 0x01;
int alarm_setup_cursor = 0;
int alarm_week[31],alarm_hour[31],alarm_min[31],alarm_sec[31],alarm_ch[31];
//int alarm_process_one = 0;
int sec_history = 0;
int alarm_out_ch[9] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

int hval12_down(int val);
int hval12_up(int val);
int hval24_down(int val);
int hval24_up(int val);
int hval31_down(int val);
int hval31_up(int val);
int hval60_down(int val);
int hval60_up(int val);
int hval100_down(int val);
int hval100_up(int val);
//�j���𕶎��ŗp��
char i2week[10][5]={"SUN ","MON ","TUE ","WED ","THU ","FRI ","SAT ","WEEK","ALL ","OFF "};

//�A���[������
void alarm_process(){
	int tmp1 = 0;
	int tmp_alarm_ok = 0;

	for(tmp1=1;tmp1<=30;tmp1++){

		//�j���Ƃ��Ăn�j���ǂ����m�F
		if(alarm_week[tmp1]==7){
			//�T�̐ݒ肪WEEK�̏ꍇ
			if(week>=0 && week<=6){
				tmp_alarm_ok = 1;
			}
		}else if(alarm_week[tmp1]==8){
			//�T�̐ݒ肪ALL�̏ꍇ
			tmp_alarm_ok = 1;
		}else if(alarm_week[tmp1]==9){
			//�T�̐ݒ肪OFF�̏ꍇ
		}else{
			//�j���w�肪����Ƃ�
			if(alarm_week[tmp1] == week){
				tmp_alarm_ok = 1;
			}
		}

		//���ԂƂ��Ăn�j���ǂ����m�F
		if(alarm_hour[tmp1] == hour && alarm_min[tmp1] == min && alarm_sec[tmp1] == sec){
			//�S�Ă̎��Ԃ���v�����ꍇ
			tmp_alarm_ok++;
		}else if(alarm_hour[tmp1] == 0x60 && alarm_min[tmp1] == min && alarm_sec[tmp1] == sec){
			//�����̎�
			tmp_alarm_ok++;
		}else if(alarm_hour[tmp1] == hour && alarm_min[tmp1] == 0x60 && alarm_sec[tmp1] == sec){
			//�����̎�
			tmp_alarm_ok++;
		}else if(alarm_hour[tmp1] == hour && alarm_min[tmp1] == min && alarm_sec[tmp1] == 0x60){
			//���b�̎�
			tmp_alarm_ok++;
		}

		////�`�����l���Ƃ��Ăn�j���ǂ���
		//if(alarm_ch[tmp1] != 0){
		//	//�`�����l���̎w�肪����ꍇ
		//	tmp_alarm_ok++;
		//}

		//�A���[�����s
		if(tmp_alarm_ok==2){
			tmp_alarm_ok = 0;
			alarm_out_ch[alarm_ch[tmp1]] = 255;			
			//fprintf(c1,"alarm %u %02x:%02x:%02x alarm_ch=%x ok.\r\n",tmp1,hour,min,sec,alarm_ch[tmp1]);
		}
	}
}

void alarm_read(){

	int tmp1 = 0;
	int tmp2 = 0;
	int tmp3 = 0;

	//�A���[���ݒ�
	for(tmp1=0;tmp1<30;++tmp1){
		tmp2 = (tmp1*5);
		tmp3 = tmp1+1;
		alarm_week[tmp3] = READ_EEPROM(tmp2+0);	if(alarm_week[tmp3] == 0xff){	alarm_week[tmp3] = 0x09;}
		alarm_hour[tmp3] = READ_EEPROM(tmp2+1);	if(alarm_hour[tmp3] == 0xff){	alarm_hour[tmp3] = 0x00;}
		alarm_min[tmp3] = READ_EEPROM(tmp2+2);	if(alarm_min[tmp3] == 0xff){	alarm_min[tmp3] = 0x00;}
		alarm_sec[tmp3] = READ_EEPROM(tmp2+3);	if(alarm_sec[tmp3] == 0xff){	alarm_sec[tmp3] = 0x00;}
		alarm_ch[tmp3] = READ_EEPROM(tmp2+4);	if(alarm_ch[tmp3] == 0xff){	alarm_ch[tmp3] = 0x01;}
	}
}

//�A���[�����e��ۑ�
void alarm_write(){

	int tmp1 = 0;
	int tmp2 = 0;

	for(tmp1=0;tmp1<30;++tmp1){

		tmp2 = (tmp1*5);
		WRITE_EEPROM(tmp2+0,alarm_week[tmp1+1]);
		WRITE_EEPROM(tmp2+1,alarm_hour[tmp1+1]);
		WRITE_EEPROM(tmp2+2,alarm_min[tmp1+1]);
		WRITE_EEPROM(tmp2+3,alarm_sec[tmp1+1]);
		WRITE_EEPROM(tmp2+4,alarm_ch[tmp1+1]);
	}
}

//�A���[���ݒ� ���ڏC��
void lcd_sw_process_alarm_setup(long end_address){
	int tmp1 = 0;
	int tmp_page_no = 0;
	int tmp_line_view_id[3] = {0x00,0x00};
	int tmp_line_cursor[3] = {0x20,0x20};
	//char tmp_line_week[3][4];
	char tmp_line1_ch[3];
	char tmp_line2_ch[3];
	//char tmp_i2week[10][4];
	int tmp_scrollmark = 0x20;
	int tmp_editid = 0;

	if(alarm_setup_step >= 1 && alarm_setup_step < 6){
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

		alarm_setup_step++;
	}else if(alarm_setup_step == 6){
		//������

		//����������
		alarm_setup_step = 1000;
		alarm_setup_cursor = 1;

		
	}else if(alarm_setup_step >= 11 && alarm_setup_step < 16){
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

		alarm_setup_step++;
	}else if(alarm_setup_step == 16){
		//�ݒ胂�[�h�I��

		alarm_setup_step = 0;
		//�ЂƂO�̃X�e�b�v�ɖ߂�
		main_step = end_address;

		//�A���[���̐ݒ��ۑ�
		alarm_write();
	}else if(alarm_setup_step >= 1000 && alarm_setup_step < 2000){
		
		//Enter�L�[
		if(sw_1 == 1){
			sw_1 = 0;

			if((alarm_setup_step%10) == 0){
				//ID�ԍ���I��

				if(alarm_setup_step == 1000 && alarm_setup_cursor == 1){
					//��ԏ�̃y�[�W�ŃJ�[�\������ɂ���ꍇ�߂�
					
					//�߂�O�ɕ��������X�N���[������X�e�b�v�Ɉڂ�
					alarm_setup_step = 11;
				}else{
					alarm_setup_step = alarm_setup_step+1;
				}
			}else if((alarm_setup_step%10) == 5){
				//���̃y�[�W����擪�y�[�W�ɖ߂�
				alarm_setup_step = alarm_setup_step-5;
			}else{
				alarm_setup_step = alarm_setup_step+1;
			}
		}

		//��ɏC������ID�ԍ���p�ӂ���
		tmp_editid = ((alarm_setup_step-1000)/10)+(alarm_setup_cursor-1);

		//UP/LEFT�L�[
		if(sw_2 == 1){
			sw_2 = 0;

			if((alarm_setup_step%10) == 0){
				//�eID�I��

				if(alarm_setup_step == 1000 && alarm_setup_cursor == 1){
					//��ԏ�̃y�[�W�ŃJ�[�\������ɂ���ꍇ�͖���
				}else if(alarm_setup_cursor == 1){
					//�J�[�\������ɂ���ꍇ
					alarm_setup_step = alarm_setup_step-10;
				}else if(alarm_setup_cursor == 2){
					//�J�[�\�������ɂ���ꍇ
					alarm_setup_cursor = 1;
				}
			}else if((alarm_setup_step%10) == 1){
				//���ݒ蒆
				alarm_hour[tmp_editid] = hval24_up(alarm_hour[tmp_editid]);;
			}else if((alarm_setup_step%10) == 2){
				//���ݒ蒆
				alarm_min[tmp_editid] = hval60_up(alarm_min[tmp_editid]);
			}else if((alarm_setup_step%10) == 3){
				//�b�ݒ蒆
				alarm_sec[tmp_editid] = hval60_up(alarm_sec[tmp_editid]);
			}else if((alarm_setup_step%10) == 4){
				//�j���ݒ蒆
				if(alarm_week[tmp_editid]<9){
					alarm_week[tmp_editid] = hval100_up(alarm_week[tmp_editid]);
				}else{
					alarm_week[tmp_editid] = 0;
				}
			}else if((alarm_setup_step%10) == 5){
				//�`�����l���ݒ蒆
				if(alarm_ch[tmp_editid]<8){
					alarm_ch[tmp_editid] = hval100_up(alarm_ch[tmp_editid]);
				}else{
					alarm_ch[tmp_editid] = 1;
				}
			}
		}

		//DOWN/RIGHT�L�[
		if(sw_3 == 1){
			sw_3 = 0;

			if((alarm_setup_step%10) == 0){
				//�eID�I��

				if(alarm_setup_step == 1290 && alarm_setup_cursor == 2){
					//��ԉ��̃y�[�W�ŃJ�[�\�������ɂ���ꍇ�͖���
				}else if(alarm_setup_cursor == 1){
					//�J�[�\������ɂ���ꍇ
					alarm_setup_cursor = 2;
				}else if(alarm_setup_cursor == 2){
					//�J�[�\�������ɂ���ꍇ
					alarm_setup_step = alarm_setup_step+10;
				}
			}else if((alarm_setup_step%10) == 1){
				//���ݒ蒆
				alarm_hour[tmp_editid] = hval24_down(alarm_hour[tmp_editid]);;
			}else if((alarm_setup_step%10) == 2){
				//���ݒ蒆
				alarm_min[tmp_editid] = hval60_down(alarm_min[tmp_editid]);
			}else if((alarm_setup_step%10) == 3){
				//�b�ݒ蒆
				alarm_sec[tmp_editid] = hval60_down(alarm_sec[tmp_editid]);
			}else if((alarm_setup_step%10) == 4){
				//�j���ݒ蒆
				
				if(alarm_week[tmp_editid]>0){
					alarm_week[tmp_editid] = hval100_down(alarm_week[tmp_editid]);
				}else{
					alarm_week[tmp_editid] = 9;
				}
			}else if((alarm_setup_step%10) == 5){
				//�`�����l���ݒ蒆
				if(alarm_ch[tmp_editid]>1){
					alarm_ch[tmp_editid] = hval100_down(alarm_ch[tmp_editid]);
				}else{
					alarm_ch[tmp_editid] = 8;
				}
			}
		}

		//�J�[�\���ʒu�ɃJ�[�\����\��
		if(alarm_setup_cursor == 1){
			tmp_line_cursor[1] = 0xfc;
			tmp_line_cursor[2] = 0x20;
		}else if(alarm_setup_cursor == 2){
			tmp_line_cursor[1] = 0x20;
			tmp_line_cursor[2] = 0xfc;
		}

		//�\������y�[�W�ԍ���p��
		if(alarm_setup_step<1010){
			tmp_page_no = 0;
		}else{
			tmp_page_no = ((alarm_setup_step-(alarm_setup_step%10))/10)-100;
		}

		//�\������ID�ԍ���p��
		tmp_line_view_id[1] = tmp_page_no;
		tmp_line_view_id[2] = tmp_page_no+1;

		//��ԉ��̍s���ǂ����𔻒�
		if(alarm_setup_step == 1290){
			//��ԉ��̃y�[�W�ɗ����牺���͕\�����Ȃ�
			tmp_scrollmark = 0x20;
		}else{
			//�ʏ�͉����
			tmp_scrollmark = 0x02;
		}

		//���\�������� �P�s�ڕ\��
		if((alarm_setup_step-(alarm_setup_step%10)) == 1000){
			sprintf(lcd_line1," %cEXIT          ",tmp_line_cursor[1]);
		}else{
			tmp1 = tmp_line_view_id[1];//�Ώۂ�ID�ԍ��̕ϐ����������̂ŕʂŗp��
			if((alarm_setup_step%10) < 4){
				sprintf(lcd_line1,"%c%c%02u %02x:%02x:%02x  %c",
					0x01,
					tmp_line_cursor[1],tmp1,alarm_hour[tmp1],alarm_min[tmp1],alarm_sec[tmp1],0x7e);
			}else{
				sprintf(lcd_line1,"%c%c%02u %c       CH "
					,0x01,
					tmp_line_cursor[1],tmp1,0x7f);

				sprintf(tmp_line1_ch,"%02u",alarm_ch[tmp_line_view_id[1]]);
				//�j����g�ݍ���
				tmp1 = alarm_week[tmp_line_view_id[1]];
				lcd_line1[6] = i2week[tmp1][0];
				lcd_line1[7] = i2week[tmp1][1];
				lcd_line1[8] = i2week[tmp1][2];
				lcd_line1[9] = i2week[tmp1][3];
				//�`�����l����g�ݍ���
				lcd_line1[11] = tmp_line1_ch[0];
				lcd_line1[12] = tmp_line1_ch[1];
			}
		}

		//���\�������� �Q�s�ڕ\��
		if((alarm_setup_step%10) < 4){

			tmp1 = tmp_line_view_id[2];
			sprintf(lcd_line2,"%c%c%02u %02x:%02x:%02x  %c",
				tmp_scrollmark,
				tmp_line_cursor[2],tmp1,alarm_hour[tmp1],alarm_min[tmp1],alarm_sec[tmp1],0x7e);

		}else{
			sprintf(lcd_line2,"%c%c%02u %c       CH "
				,tmp_scrollmark,tmp_line_cursor[2],tmp_line_view_id[2],0x7f);
			sprintf(tmp_line2_ch,"%02u",alarm_ch[tmp_line_view_id[2]]);
			//�j����g�ݍ���
			tmp1 = alarm_week[tmp_line_view_id[2]];
			lcd_line2[6] = i2week[tmp1][0];
			lcd_line2[7] = i2week[tmp1][1];
			lcd_line2[8] = i2week[tmp1][2];
			lcd_line2[9] = i2week[tmp1][3];
			//�`�����l����g�ݍ���
			lcd_line2[11] = tmp_line2_ch[0];
			lcd_line2[12] = tmp_line2_ch[1];
		}

		//���\���������@�ҏW���ڂ̓_��
		if((alarm_setup_step%10) == 0){
			//�s�I��
		}else if((alarm_setup_step%10) == 1 && flicker_low == 1){
			//1�̌���1�̏ꍇ
			//���ݒ蒆
			if(alarm_setup_cursor==1){
				lcd_line1[5] = 0x20;
				lcd_line1[6] = 0x20;
			}else if(alarm_setup_cursor==2){
				lcd_line2[5] = 0x20;
				lcd_line2[6] = 0x20;
			}
		}else if((alarm_setup_step%10) == 2 && flicker_low == 1){
			//1�̌���2�̏ꍇ
			//���ݒ蒆
			if(alarm_setup_cursor==1){
				lcd_line1[8] = 0x20;
				lcd_line1[9] = 0x20;
			}else if(alarm_setup_cursor==2){
				lcd_line2[8] = 0x20;
				lcd_line2[9] = 0x20;
			}
		}else if((alarm_setup_step%10) == 3 && flicker_low == 1){
			//1�̌���3�̏ꍇ
			//�b�ݒ蒆
			if(alarm_setup_cursor==1){
				lcd_line1[11] = 0x20;
				lcd_line1[12] = 0x20;
			}else if(alarm_setup_cursor==2){
				lcd_line2[11] = 0x20;
				lcd_line2[12] = 0x20;
			}
		}else if((alarm_setup_step%10)==4 && flicker_low == 1){
			//�j�����t���b�J�[����
			if(alarm_setup_cursor==1){
				lcd_line1[6] = 0x20;
				lcd_line1[7] = 0x20;
				lcd_line1[8] = 0x20;
				lcd_line1[9] = 0x20;
			}else if(alarm_setup_cursor==2){
				lcd_line2[6] = 0x20;
				lcd_line2[7] = 0x20;
				lcd_line2[8] = 0x20;
				lcd_line2[9] = 0x20;
			}
		}else if((alarm_setup_step%10)==5 && flicker_low == 1){
			//�`�����l�����t���b�J�[����
			if(alarm_setup_cursor==1){
				lcd_line1[11] = 0x20;
				lcd_line1[12] = 0x20;
			}else if(alarm_setup_cursor==2){
				lcd_line2[11] = 0x20;
				lcd_line2[12] = 0x20;
			}
		}

	}
}

//16�i���̐�����10�i���Ƃ��ăJ�E���g�_�E������i12�i�d�l�j
int hval12_down(int val){
	char *ans;
	int tmp_val;

	tmp_val = val;
	if(tmp_val == 0x01){
		tmp_val = 0x12;
	}else{
		if((tmp_val%16)==0){
			tmp_val = val - 6;
		}
		tmp_val--;
	}
	ans = tmp_val;

	return ans;
}

//16�i���̐�����10�i���Ƃ��ăJ�E���g�_�E������i24�i�d�l�j
int hval24_down(int val){
	char *ans;
	int tmp_val;

	tmp_val = val;
	if(tmp_val == 0x00){
		tmp_val = 0x23;
	}else{
		if((tmp_val%16)==0){
			tmp_val = val - 6;
		}
		tmp_val--;
	}
	ans = tmp_val;

	return ans;
}

//16�i���̐�����10�i���Ƃ��ăJ�E���g�_�E������i31�i�d�l�j
int hval31_down(int val){
	char *ans;
	int tmp_val;

	tmp_val = val;
	if(tmp_val == 0x01){
		tmp_val = 0x31;
	}else{
		if((tmp_val%16)==0){
			tmp_val = val - 6;
		}
		tmp_val--;
	}
	ans = tmp_val;

	return ans;
}

//16�i���̐�����10�i���Ƃ��ăJ�E���g�_�E������i60�i�d�l�j
int hval60_down(int val){
	char *ans;
	int tmp_val;

	tmp_val = val;
	if(tmp_val == 0x00){
		tmp_val = 0x59;
	}else{
		if((tmp_val%16)==0){
			tmp_val = val - 6;
		}
		tmp_val--;
	}
	ans = tmp_val;

	return ans;
}

//16�i���̐�����10�i���Ƃ��ăJ�E���g�_�E������i100�i�d�l�j
int hval100_down(int val){
	char *ans;
	int tmp_val;

	tmp_val = val;
	if(tmp_val == 0x00){
		tmp_val = 0x99;
	}else{
		if((tmp_val%16)==0){
			tmp_val = val - 6;
		}
		tmp_val--;
	}
	ans = tmp_val;

	return ans;
}

//16�i���̐�����10�i���Ƃ��ăJ�E���g�A�b�v����i12�i�d�l�j
int hval12_up(int val){
	int ans;
	int tmp_val;

	tmp_val = val;
	if(tmp_val == 0x12){
		tmp_val = 0x01;
	}else{
		if(((tmp_val-9)%16)==0){
			tmp_val = tmp_val + 6;	
		}
		tmp_val++;
	}
	ans = tmp_val;
	return ans;
}

//16�i���̐�����10�i���Ƃ��ăJ�E���g�A�b�v����i24�i�d�l�j
int hval24_up(int val){
	int ans;
	int tmp_val;

	tmp_val = val;
	if(tmp_val == 0x23){
		tmp_val = 0x00;
	}else{
		if(((tmp_val-9)%16)==0){
			tmp_val = tmp_val + 6;	
		}
		tmp_val++;
	}
	ans = tmp_val;
	return ans;
}

//16�i���̐�����10�i���Ƃ��ăJ�E���g�A�b�v����i31�i�d�l�j
int hval31_up(int val){
	int ans;
	int tmp_val;

	tmp_val = val;
	if(tmp_val == 0x31){
		tmp_val = 0x01;
	}else{
		if(((tmp_val-9)%16)==0){
			tmp_val = tmp_val + 6;	
		}
		tmp_val++;
	}
	ans = tmp_val;
	return ans;
}

//16�i���̐�����10�i���Ƃ��ăJ�E���g�A�b�v����i60�i�d�l�j
int hval60_up(int val){
	int ans;
	int tmp_val;

	tmp_val = val;
	if(tmp_val == 0x59){
		tmp_val = 0x00;
	}else{
		if(((tmp_val-9)%16)==0){
			tmp_val = tmp_val + 6;	
		}
		tmp_val++;
	}
	ans = tmp_val;
	return ans;
}

//16�i���̐�����10�i���Ƃ��ăJ�E���g�A�b�v����i100�i�d�l�j
int hval100_up(int val){
	int ans;
	int tmp_val;

	tmp_val = val;
	if(tmp_val == 0x99){
		tmp_val = 0x00;
	}else{
		if(((tmp_val-9)%16)==0){
			tmp_val = tmp_val + 6;	
		}
		tmp_val++;
	}
	ans = tmp_val;
	return ans;
}

