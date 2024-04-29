//アラーム設定関係
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
//曜日を文字で用意
char i2week[10][5]={"SUN ","MON ","TUE ","WED ","THU ","FRI ","SAT ","WEEK","ALL ","OFF "};

//アラーム処理
void alarm_process(){
	int tmp1 = 0;
	int tmp_alarm_ok = 0;

	for(tmp1=1;tmp1<=30;tmp1++){

		//曜日としてＯＫかどうか確認
		if(alarm_week[tmp1]==7){
			//週の設定がWEEKの場合
			if(week>=0 && week<=6){
				tmp_alarm_ok = 1;
			}
		}else if(alarm_week[tmp1]==8){
			//週の設定がALLの場合
			tmp_alarm_ok = 1;
		}else if(alarm_week[tmp1]==9){
			//週の設定がOFFの場合
		}else{
			//曜日指定があるとき
			if(alarm_week[tmp1] == week){
				tmp_alarm_ok = 1;
			}
		}

		//時間としてＯＫかどうか確認
		if(alarm_hour[tmp1] == hour && alarm_min[tmp1] == min && alarm_sec[tmp1] == sec){
			//全ての時間が一致した場合
			tmp_alarm_ok++;
		}else if(alarm_hour[tmp1] == 0x60 && alarm_min[tmp1] == min && alarm_sec[tmp1] == sec){
			//毎時の時
			tmp_alarm_ok++;
		}else if(alarm_hour[tmp1] == hour && alarm_min[tmp1] == 0x60 && alarm_sec[tmp1] == sec){
			//毎分の時
			tmp_alarm_ok++;
		}else if(alarm_hour[tmp1] == hour && alarm_min[tmp1] == min && alarm_sec[tmp1] == 0x60){
			//毎秒の時
			tmp_alarm_ok++;
		}

		////チャンネルとしてＯＫかどうか
		//if(alarm_ch[tmp1] != 0){
		//	//チャンネルの指定がある場合
		//	tmp_alarm_ok++;
		//}

		//アラーム実行
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

	//アラーム設定
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

//アラーム内容を保存
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

//アラーム設定 項目修正
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
		//今表示している文字を左に逃がす
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
		//初期化

		//初期化完了
		alarm_setup_step = 1000;
		alarm_setup_cursor = 1;

		
	}else if(alarm_setup_step >= 11 && alarm_setup_step < 16){
		//今表示している文字を左に逃がす
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
		//設定モード終了

		alarm_setup_step = 0;
		//ひとつ前のステップに戻る
		main_step = end_address;

		//アラームの設定を保存
		alarm_write();
	}else if(alarm_setup_step >= 1000 && alarm_setup_step < 2000){
		
		//Enterキー
		if(sw_1 == 1){
			sw_1 = 0;

			if((alarm_setup_step%10) == 0){
				//ID番号を選択中

				if(alarm_setup_step == 1000 && alarm_setup_cursor == 1){
					//一番上のページでカーソルが上にある場合戻る
					
					//戻る前に文字を左スクロールするステップに移る
					alarm_setup_step = 11;
				}else{
					alarm_setup_step = alarm_setup_step+1;
				}
			}else if((alarm_setup_step%10) == 5){
				//次のページから先頭ページに戻る
				alarm_setup_step = alarm_setup_step-5;
			}else{
				alarm_setup_step = alarm_setup_step+1;
			}
		}

		//先に修正するID番号を用意する
		tmp_editid = ((alarm_setup_step-1000)/10)+(alarm_setup_cursor-1);

		//UP/LEFTキー
		if(sw_2 == 1){
			sw_2 = 0;

			if((alarm_setup_step%10) == 0){
				//各ID選択中

				if(alarm_setup_step == 1000 && alarm_setup_cursor == 1){
					//一番上のページでカーソルが上にある場合は無視
				}else if(alarm_setup_cursor == 1){
					//カーソルが上にある場合
					alarm_setup_step = alarm_setup_step-10;
				}else if(alarm_setup_cursor == 2){
					//カーソルが下にある場合
					alarm_setup_cursor = 1;
				}
			}else if((alarm_setup_step%10) == 1){
				//時設定中
				alarm_hour[tmp_editid] = hval24_up(alarm_hour[tmp_editid]);;
			}else if((alarm_setup_step%10) == 2){
				//分設定中
				alarm_min[tmp_editid] = hval60_up(alarm_min[tmp_editid]);
			}else if((alarm_setup_step%10) == 3){
				//秒設定中
				alarm_sec[tmp_editid] = hval60_up(alarm_sec[tmp_editid]);
			}else if((alarm_setup_step%10) == 4){
				//曜日設定中
				if(alarm_week[tmp_editid]<9){
					alarm_week[tmp_editid] = hval100_up(alarm_week[tmp_editid]);
				}else{
					alarm_week[tmp_editid] = 0;
				}
			}else if((alarm_setup_step%10) == 5){
				//チャンネル設定中
				if(alarm_ch[tmp_editid]<8){
					alarm_ch[tmp_editid] = hval100_up(alarm_ch[tmp_editid]);
				}else{
					alarm_ch[tmp_editid] = 1;
				}
			}
		}

		//DOWN/RIGHTキー
		if(sw_3 == 1){
			sw_3 = 0;

			if((alarm_setup_step%10) == 0){
				//各ID選択中

				if(alarm_setup_step == 1290 && alarm_setup_cursor == 2){
					//一番下のページでカーソルが下にある場合は無視
				}else if(alarm_setup_cursor == 1){
					//カーソルが上にある場合
					alarm_setup_cursor = 2;
				}else if(alarm_setup_cursor == 2){
					//カーソルが下にある場合
					alarm_setup_step = alarm_setup_step+10;
				}
			}else if((alarm_setup_step%10) == 1){
				//時設定中
				alarm_hour[tmp_editid] = hval24_down(alarm_hour[tmp_editid]);;
			}else if((alarm_setup_step%10) == 2){
				//分設定中
				alarm_min[tmp_editid] = hval60_down(alarm_min[tmp_editid]);
			}else if((alarm_setup_step%10) == 3){
				//秒設定中
				alarm_sec[tmp_editid] = hval60_down(alarm_sec[tmp_editid]);
			}else if((alarm_setup_step%10) == 4){
				//曜日設定中
				
				if(alarm_week[tmp_editid]>0){
					alarm_week[tmp_editid] = hval100_down(alarm_week[tmp_editid]);
				}else{
					alarm_week[tmp_editid] = 9;
				}
			}else if((alarm_setup_step%10) == 5){
				//チャンネル設定中
				if(alarm_ch[tmp_editid]>1){
					alarm_ch[tmp_editid] = hval100_down(alarm_ch[tmp_editid]);
				}else{
					alarm_ch[tmp_editid] = 8;
				}
			}
		}

		//カーソル位置にカーソルを表示
		if(alarm_setup_cursor == 1){
			tmp_line_cursor[1] = 0xfc;
			tmp_line_cursor[2] = 0x20;
		}else if(alarm_setup_cursor == 2){
			tmp_line_cursor[1] = 0x20;
			tmp_line_cursor[2] = 0xfc;
		}

		//表示するページ番号を用意
		if(alarm_setup_step<1010){
			tmp_page_no = 0;
		}else{
			tmp_page_no = ((alarm_setup_step-(alarm_setup_step%10))/10)-100;
		}

		//表示するID番号を用意
		tmp_line_view_id[1] = tmp_page_no;
		tmp_line_view_id[2] = tmp_page_no+1;

		//一番下の行かどうかを判定
		if(alarm_setup_step == 1290){
			//一番下のページに来たら下矢印は表示しない
			tmp_scrollmark = 0x20;
		}else{
			//通常は下矢印
			tmp_scrollmark = 0x02;
		}

		//■表示処理■ １行目表示
		if((alarm_setup_step-(alarm_setup_step%10)) == 1000){
			sprintf(lcd_line1," %cEXIT          ",tmp_line_cursor[1]);
		}else{
			tmp1 = tmp_line_view_id[1];//対象のID番号の変数名が長いので別で用意
			if((alarm_setup_step%10) < 4){
				sprintf(lcd_line1,"%c%c%02u %02x:%02x:%02x  %c",
					0x01,
					tmp_line_cursor[1],tmp1,alarm_hour[tmp1],alarm_min[tmp1],alarm_sec[tmp1],0x7e);
			}else{
				sprintf(lcd_line1,"%c%c%02u %c       CH "
					,0x01,
					tmp_line_cursor[1],tmp1,0x7f);

				sprintf(tmp_line1_ch,"%02u",alarm_ch[tmp_line_view_id[1]]);
				//曜日を組み込む
				tmp1 = alarm_week[tmp_line_view_id[1]];
				lcd_line1[6] = i2week[tmp1][0];
				lcd_line1[7] = i2week[tmp1][1];
				lcd_line1[8] = i2week[tmp1][2];
				lcd_line1[9] = i2week[tmp1][3];
				//チャンネルを組み込む
				lcd_line1[11] = tmp_line1_ch[0];
				lcd_line1[12] = tmp_line1_ch[1];
			}
		}

		//■表示処理■ ２行目表示
		if((alarm_setup_step%10) < 4){

			tmp1 = tmp_line_view_id[2];
			sprintf(lcd_line2,"%c%c%02u %02x:%02x:%02x  %c",
				tmp_scrollmark,
				tmp_line_cursor[2],tmp1,alarm_hour[tmp1],alarm_min[tmp1],alarm_sec[tmp1],0x7e);

		}else{
			sprintf(lcd_line2,"%c%c%02u %c       CH "
				,tmp_scrollmark,tmp_line_cursor[2],tmp_line_view_id[2],0x7f);
			sprintf(tmp_line2_ch,"%02u",alarm_ch[tmp_line_view_id[2]]);
			//曜日を組み込む
			tmp1 = alarm_week[tmp_line_view_id[2]];
			lcd_line2[6] = i2week[tmp1][0];
			lcd_line2[7] = i2week[tmp1][1];
			lcd_line2[8] = i2week[tmp1][2];
			lcd_line2[9] = i2week[tmp1][3];
			//チャンネルを組み込む
			lcd_line2[11] = tmp_line2_ch[0];
			lcd_line2[12] = tmp_line2_ch[1];
		}

		//■表示処理■　編集項目の点滅
		if((alarm_setup_step%10) == 0){
			//行選択中
		}else if((alarm_setup_step%10) == 1 && flicker_low == 1){
			//1の桁が1の場合
			//時設定中
			if(alarm_setup_cursor==1){
				lcd_line1[5] = 0x20;
				lcd_line1[6] = 0x20;
			}else if(alarm_setup_cursor==2){
				lcd_line2[5] = 0x20;
				lcd_line2[6] = 0x20;
			}
		}else if((alarm_setup_step%10) == 2 && flicker_low == 1){
			//1の桁が2の場合
			//分設定中
			if(alarm_setup_cursor==1){
				lcd_line1[8] = 0x20;
				lcd_line1[9] = 0x20;
			}else if(alarm_setup_cursor==2){
				lcd_line2[8] = 0x20;
				lcd_line2[9] = 0x20;
			}
		}else if((alarm_setup_step%10) == 3 && flicker_low == 1){
			//1の桁が3の場合
			//秒設定中
			if(alarm_setup_cursor==1){
				lcd_line1[11] = 0x20;
				lcd_line1[12] = 0x20;
			}else if(alarm_setup_cursor==2){
				lcd_line2[11] = 0x20;
				lcd_line2[12] = 0x20;
			}
		}else if((alarm_setup_step%10)==4 && flicker_low == 1){
			//曜日をフリッカーする
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
			//チャンネルをフリッカーする
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

//16進数の数字を10進数としてカウントダウンする（12進仕様）
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

//16進数の数字を10進数としてカウントダウンする（24進仕様）
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

//16進数の数字を10進数としてカウントダウンする（31進仕様）
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

//16進数の数字を10進数としてカウントダウンする（60進仕様）
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

//16進数の数字を10進数としてカウントダウンする（100進仕様）
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

//16進数の数字を10進数としてカウントアップする（12進仕様）
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

//16進数の数字を10進数としてカウントアップする（24進仕様）
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

//16進数の数字を10進数としてカウントアップする（31進仕様）
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

//16進数の数字を10進数としてカウントアップする（60進仕様）
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

//16進数の数字を10進数としてカウントアップする（100進仕様）
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

