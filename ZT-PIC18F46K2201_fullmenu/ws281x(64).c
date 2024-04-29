#define LED_OUT    PIN_A6

#include "led_pattern(64).c"//�p�^�[���ݒ�l
//��		ff 00 00
//�I�����W	ff b7 4c
//��		ff ff 00
//��		ff ff ff
//��		00 ff 00
//��		00 00 ff
//��		c4 00 cc
//�s���N	ef 8f 9c

//�`�F�b�N�p
int ws281x_buffer_r[64] = {//64=0x40 255=0xff
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//1
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//2
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//3
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//4
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//5
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//6
	0x00,0x00,0x00,0x00};//

int ws281x_buffer_g[64] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//1
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//2
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//3
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//4
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//5
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//6
	0x00,0x00,0x00,0x00};//

int ws281x_buffer_b[64] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//1
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//2
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//3
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//4
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//5
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//6
	0x00,0x00,0x00,0x00};//

//���̑��̕ϐ�
long ws281x_led_value = 64;
int ws281x_pattern_value = 30;//
int ws281x_process_one = 0;
//long ws281x_step = 0;
int ws281x_pattern_index = 0;//
int ws281x_mode = 0;//
int ws281x_mode_weight = 0;//100�Ȃ��~
int ws281x_mode_weight_count = 0;//
//int ws281x_powereffect = 100;//�o�͒l float�������H
int ws281x_status = 0;//mode=30�ԑ䂾���Ŏg�p
int ws281x_checkcount = 0;//mode=30�ԑ䂾���Ŏg�p
int ws281x_timing = 0;//

//�J���[�p�^�[�����o�b�t�@�ɓ����
void ws281x_color_set(int set_index){
	
	long tmp1 = 0;
	
	ws281x_buffer_r = ws281x_color_r[set_index];
	ws281x_buffer_g = ws281x_color_g[set_index];
	ws281x_buffer_b = ws281x_color_b[set_index];

	//for(tmp1=0;tmp1<ws281x_led_value;tmp1++){
	//	ws281x_buffer_r[tmp1] = ws281x_color_r[set_index][tmp1];
	//	ws281x_buffer_g[tmp1] = ws281x_color_g[set_index][tmp1];
	//	ws281x_buffer_b[tmp1] = ws281x_color_b[set_index][tmp1];
	//}
}

//�J���[��ω�������
void ws281x_mode_set(int mode){
	long tmp1 = 0;
	int tmp_r = 0;
	int tmp_g = 0;
	int tmp_b = 0;

	if(ws281x_mode_weight == ws281x_mode_weight_count){
		ws281x_mode_weight_count = 0;

		if(mode == 0){
			//�������Ȃ�

		}else if(mode == 1){
			//down(�ォ�牺�����ĉ��)
			tmp_r = ws281x_buffer_r[0];
			tmp_g = ws281x_buffer_g[0];
			tmp_b = ws281x_buffer_b[0];
			for(tmp1=0;tmp1<ws281x_led_value-1;tmp1++){
				ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1+1];
				ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1+1];
				ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1+1];
			}
			ws281x_buffer_r[ws281x_led_value-1] = tmp_r;
			ws281x_buffer_g[ws281x_led_value-1] = tmp_g;
			ws281x_buffer_b[ws281x_led_value-1] = tmp_b;
		}else if(mode == 2){
			//up(������オ���ĉ��)
			tmp_r = ws281x_buffer_r[ws281x_led_value-1];
			tmp_g = ws281x_buffer_g[ws281x_led_value-1];
			tmp_b = ws281x_buffer_b[ws281x_led_value-1];
			for(tmp1=0;tmp1<ws281x_led_value-1;tmp1++){
				ws281x_buffer_r[ws281x_led_value-tmp1-1] = ws281x_buffer_r[ws281x_led_value-tmp1-2];
				ws281x_buffer_g[ws281x_led_value-tmp1-1] = ws281x_buffer_g[ws281x_led_value-tmp1-2];
				ws281x_buffer_b[ws281x_led_value-tmp1-1] = ws281x_buffer_b[ws281x_led_value-tmp1-2];
			}
			ws281x_buffer_r[0] = tmp_r;
			ws281x_buffer_g[0] = tmp_g;
			ws281x_buffer_b[0] = tmp_b;
		}else if(mode == 3){
			//�ォ�牺����A��̏�Ԃ��R�s�[�Ŏc��
			for(tmp1=0;tmp1<ws281x_led_value-1;tmp1++){
				ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1+1];
				ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1+1];
				ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1+1];
			}
		}else if(mode == 4){
			//������オ��A���̏�Ԃ��R�s�[�Ŏc��
			for(tmp1=0;tmp1<ws281x_led_value-1;tmp1++){
				ws281x_buffer_r[ws281x_led_value-tmp1-1] = ws281x_buffer_r[ws281x_led_value-tmp1-2];
				ws281x_buffer_g[ws281x_led_value-tmp1-1] = ws281x_buffer_g[ws281x_led_value-tmp1-2];
				ws281x_buffer_b[ws281x_led_value-tmp1-1] = ws281x_buffer_b[ws281x_led_value-tmp1-2];
			}
		}else if(mode == 5){
			//�^�񒆂���O�։��

			//�^�񒆂����ɉ��
			tmp_r = ws281x_buffer_r[0];
			tmp_g = ws281x_buffer_g[0];
			tmp_b = ws281x_buffer_b[0];
			for(tmp1=0;tmp1<(ws281x_led_value/2)-1;tmp1++){
				ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1+1];
				ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1+1];
				ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1+1];
			}
			ws281x_buffer_r[(ws281x_led_value/2)-1] = tmp_r;
			ws281x_buffer_g[(ws281x_led_value/2)-1] = tmp_g;
			ws281x_buffer_b[(ws281x_led_value/2)-1] = tmp_b;

			//�^�񒆂��牺�ɉ��
			tmp_r = ws281x_buffer_r[ws281x_led_value-1];
			tmp_g = ws281x_buffer_g[ws281x_led_value-1];
			tmp_b = ws281x_buffer_b[ws281x_led_value-1];
			for(tmp1=0;tmp1<(ws281x_led_value/2)-1;tmp1++){
				ws281x_buffer_r[ws281x_led_value-tmp1-1] = ws281x_buffer_r[ws281x_led_value-tmp1-2];
				ws281x_buffer_g[ws281x_led_value-tmp1-1] = ws281x_buffer_g[ws281x_led_value-tmp1-2];
				ws281x_buffer_b[ws281x_led_value-tmp1-1] = ws281x_buffer_b[ws281x_led_value-tmp1-2];
			}
			ws281x_buffer_r[(ws281x_led_value/2)] = tmp_r;
			ws281x_buffer_g[(ws281x_led_value/2)] = tmp_g;
			ws281x_buffer_b[(ws281x_led_value/2)] = tmp_b;
		}else if(mode == 6){
			//�O����^�񒆂ɉ�遚�r��

			//
			tmp_r = ws281x_buffer_r[(ws281x_led_value/2)-1];
			tmp_g = ws281x_buffer_g[(ws281x_led_value/2)-1];
			tmp_b = ws281x_buffer_b[(ws281x_led_value/2)-1];
			for(tmp1=0;tmp1<(ws281x_led_value/2)-1;tmp1++){
				ws281x_buffer_r[(ws281x_led_value/2)-tmp1-1] = ws281x_buffer_r[ws281x_led_value-tmp1-2];
				ws281x_buffer_g[ws281x_led_value-tmp1-1] = ws281x_buffer_g[ws281x_led_value-tmp1-2];
				ws281x_buffer_b[ws281x_led_value-tmp1-1] = ws281x_buffer_b[ws281x_led_value-tmp1-2];
			}
			ws281x_buffer_r[(ws281x_led_value/2)] = tmp_r;
			ws281x_buffer_g[(ws281x_led_value/2)] = tmp_g;
			ws281x_buffer_b[(ws281x_led_value/2)] = tmp_b;

			//
			tmp_r = ws281x_buffer_r[0];
			tmp_g = ws281x_buffer_g[0];
			tmp_b = ws281x_buffer_b[0];
			for(tmp1=0;tmp1<(ws281x_led_value/2)-1;tmp1++){
				ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1+1];
				ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1+1];
				ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1+1];
			}
			ws281x_buffer_r[(ws281x_led_value/2)-1] = tmp_r;
			ws281x_buffer_g[(ws281x_led_value/2)-1] = tmp_g;
			ws281x_buffer_b[(ws281x_led_value/2)-1] = tmp_b;

		}else if(mode == 7){
			//�S�̂��Â����ď����ɂȂ�
			for(tmp1=0;tmp1<ws281x_led_value;tmp1++){

				if(ws281x_buffer_r[tmp1] > 9){	ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]-10;}else{	ws281x_buffer_r[tmp1] = 0;}
				if(ws281x_buffer_g[tmp1] > 9){	ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]-10;}else{	ws281x_buffer_g[tmp1] = 0;}
				if(ws281x_buffer_b[tmp1] > 9){	ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]-10;}else{	ws281x_buffer_b[tmp1] = 0;}
			}
		}else if(mode == 8){
			//�S�̂𖾂邭���Ĕ��ɂȂ�
			for(tmp1=0;tmp1<ws281x_led_value;tmp1++){
				if(ws281x_buffer_r[tmp1] < 246){ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]+10;}else{ws281x_buffer_r[tmp1] = 255;}
				if(ws281x_buffer_g[tmp1] < 246){ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]+10;}else{ws281x_buffer_g[tmp1] = 255;}
				if(ws281x_buffer_b[tmp1] < 246){ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]+10;}else{ws281x_buffer_b[tmp1] = 255;}
			}
		}else if(mode == 9){
			//23 �d���F�ɂ���
			for(tmp1=0;tmp1<ws281x_led_value;tmp1++){
				//�Ԃ�157�ɂ���
				if(ws281x_buffer_r[tmp1] < (157-9)){
					ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]+10;
				}else if(ws281x_buffer_r[tmp1] > (157+9)){
					ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]-10;
				}else{
					ws281x_buffer_r[tmp1] = 157;
				}

				//�΂�98�ɂ���
				if(ws281x_buffer_g[tmp1] < (98-9)){
					ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]+10;
				}else if(ws281x_buffer_g[tmp1] > (98+9)){
					ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]-10;
				}else{
					ws281x_buffer_g[tmp1] = 98;
				}

				//��0�ɂ���
				if(ws281x_buffer_b[tmp1] > 9){	ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]-10;}else{	ws281x_buffer_b[tmp1] = 0;}
			}
		}else if(mode == 10){
			//�S�̂𓯐F�Ōv���œ��F�ω�����B

			if(ws281x_status == 0){
				//�S�Ă�^���Ԃɂ���
				ws281x_checkcount = 0;
				for(tmp1=0;tmp1<ws281x_led_value;tmp1++){
					if(ws281x_buffer_r[tmp1] < 246){ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]+10;}else{ws281x_buffer_r[tmp1] = 255;}//		��+
					//if(ws281x_buffer_g[tmp1] < 246){ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]+10;}else{ws281x_buffer_g[tmp1] = 255;}//	��+
					//if(ws281x_buffer_b[tmp1] < 246){ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]+10;}else{ws281x_buffer_b[tmp1] = 255;}//	��+
					//if(ws281x_buffer_r[tmp1] > 9){	ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]-10;}else{	ws281x_buffer_r[tmp1] = 0;}//	��-
					if(ws281x_buffer_g[tmp1] > 9){	ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]-10;}else{	ws281x_buffer_g[tmp1] = 0;}//		��-
					if(ws281x_buffer_b[tmp1] > 9){	ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]-10;}else{	ws281x_buffer_b[tmp1] = 0;}//		��-

					//�ԂɂȂ�Ύ�
					if(ws281x_buffer_r[tmp1] == 255 && ws281x_buffer_g[tmp1] == 0 && ws281x_buffer_b[tmp1] == 0){
						ws281x_checkcount++;
					}
				}
				if(ws281x_checkcount == ws281x_led_value){
					ws281x_status = 1;
				}

			}else if(ws281x_status == 1){
				//�΂ɂ���
				ws281x_checkcount = 0;
				for(tmp1=0;tmp1<ws281x_led_value;tmp1++){
					//if(ws281x_buffer_r[tmp1] < 246){ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]+10;}else{ws281x_buffer_r[tmp1] = 255;}//	��+
					if(ws281x_buffer_g[tmp1] < 246){ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]+10;}else{ws281x_buffer_g[tmp1] = 255;}//		��+
					//if(ws281x_buffer_b[tmp1] < 246){ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]+10;}else{ws281x_buffer_b[tmp1] = 255;}//	��+
					if(ws281x_buffer_r[tmp1] > 9){	ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]-10;}else{	ws281x_buffer_r[tmp1] = 0;}//		��-
					//if(ws281x_buffer_g[tmp1] > 9){	ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]-10;}else{	ws281x_buffer_g[tmp1] = 0;}//	��-
					if(ws281x_buffer_b[tmp1] > 9){	ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]-10;}else{	ws281x_buffer_b[tmp1] = 0;}//		��-

					//�΂ɂȂ�Ύ�
					if(ws281x_buffer_r[tmp1] == 0 && ws281x_buffer_g[tmp1] == 255 && ws281x_buffer_b[tmp1] == 0){
						ws281x_checkcount++;
					}
				}

				if(ws281x_checkcount == ws281x_led_value){
					ws281x_status = 2;
				}

			}else if(ws281x_status == 2){
				//�ɂ���
				ws281x_checkcount = 0;
				for(tmp1=0;tmp1<ws281x_led_value;tmp1++){
					//if(ws281x_buffer_r[tmp1] < 246){ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]+10;}else{ws281x_buffer_r[tmp1] = 255;}//	��+
					//if(ws281x_buffer_g[tmp1] < 246){ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]+10;}else{ws281x_buffer_g[tmp1] = 255;}//	��+
					if(ws281x_buffer_b[tmp1] < 246){ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]+10;}else{ws281x_buffer_b[tmp1] = 255;}//		��+
					if(ws281x_buffer_r[tmp1] > 9){	ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]-10;}else{	ws281x_buffer_r[tmp1] = 0;}//		��-
					if(ws281x_buffer_g[tmp1] > 9){	ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]-10;}else{	ws281x_buffer_g[tmp1] = 0;}//		��-
					//if(ws281x_buffer_b[tmp1] > 9){	ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]-10;}else{	ws281x_buffer_b[tmp1] = 0;}//	��-

					//�ɂȂ�Ύ�
					if(ws281x_buffer_r[tmp1] == 0 && ws281x_buffer_g[tmp1] == 0 && ws281x_buffer_b[tmp1] == 255){
						ws281x_checkcount++;
					}
				}
				if(ws281x_checkcount == ws281x_led_value){
					ws281x_status = 0;
				}

			}
		}else if(mode == 11){
			//�S�̂𓯐F�Ōv���œ��F(����)�ω�����B

			if(ws281x_status == 0){
				//�S�Ă�^���Ԃɂ���
				ws281x_checkcount = 0;
				for(tmp1=0;tmp1<ws281x_led_value;tmp1++){
					if(ws281x_buffer_r[tmp1] < 246){ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]+10;}else{ws281x_buffer_r[tmp1] = 255;}//		��+
					//if(ws281x_buffer_g[tmp1] < 246){ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]+10;}else{ws281x_buffer_g[tmp1] = 255;}//	��+
					//if(ws281x_buffer_b[tmp1] < 246){ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]+10;}else{ws281x_buffer_b[tmp1] = 255;}//	��+
					//if(ws281x_buffer_r[tmp1] > 9){	ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]-10;}else{	ws281x_buffer_r[tmp1] = 0;}//	��-
					if(ws281x_buffer_g[tmp1] > 9){	ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]-10;}else{	ws281x_buffer_g[tmp1] = 0;}//		��-
					if(ws281x_buffer_b[tmp1] > 9){	ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]-10;}else{	ws281x_buffer_b[tmp1] = 0;}//		��-

					//�ԂɂȂ�Ύ�
					if(ws281x_buffer_r[tmp1] == 255 && ws281x_buffer_g[tmp1] == 0 && ws281x_buffer_b[tmp1] == 0){
						ws281x_checkcount++;
					}
				}
				if(ws281x_checkcount == ws281x_led_value){
					ws281x_status = 1;
				}

			}else if(ws281x_status == 1){
				//�΂ɂ���
				ws281x_checkcount = 0;
				for(tmp1=0;tmp1<ws281x_led_value;tmp1++){
					//if(ws281x_buffer_r[tmp1] < 246){ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]+10;}else{ws281x_buffer_r[tmp1] = 255;}//	��+
					if(ws281x_buffer_g[tmp1] < 246){ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]+10;}else{ws281x_buffer_g[tmp1] = 255;}//		��+
					//if(ws281x_buffer_b[tmp1] < 246){ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]+10;}else{ws281x_buffer_b[tmp1] = 255;}//	��+
					if(ws281x_buffer_r[tmp1] > 9){	ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]-10;}else{	ws281x_buffer_r[tmp1] = 0;}//		��-
					//if(ws281x_buffer_g[tmp1] > 9){	ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]-10;}else{	ws281x_buffer_g[tmp1] = 0;}//	��-
					if(ws281x_buffer_b[tmp1] > 9){	ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]-10;}else{	ws281x_buffer_b[tmp1] = 0;}//		��-

					//�΂ɂȂ�Ύ�
					if(ws281x_buffer_r[tmp1] == 0 && ws281x_buffer_g[tmp1] == 255 && ws281x_buffer_b[tmp1] == 0){
						ws281x_checkcount++;
					}
				}

				if(ws281x_checkcount == ws281x_led_value){
					ws281x_status = 0;
				}


			}
		}else if(mode == 12){
			//�S�̂𓯐F�Ōv���œ��F�ω�����B

			if(ws281x_status == 0){
				//�S�Ă�^���Ԃɂ���
				ws281x_checkcount = 0;
				for(tmp1=0;tmp1<ws281x_led_value;tmp1++){
					if(ws281x_buffer_r[tmp1] < 246){ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]+10;}else{ws281x_buffer_r[tmp1] = 255;}//		��+
					//if(ws281x_buffer_g[tmp1] < 246){ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]+10;}else{ws281x_buffer_g[tmp1] = 255;}//	��+
					//if(ws281x_buffer_b[tmp1] < 246){ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]+10;}else{ws281x_buffer_b[tmp1] = 255;}//	��+
					//if(ws281x_buffer_r[tmp1] > 9){	ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]-10;}else{	ws281x_buffer_r[tmp1] = 0;}//	��-
					if(ws281x_buffer_g[tmp1] > 9){	ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]-10;}else{	ws281x_buffer_g[tmp1] = 0;}//		��-
					if(ws281x_buffer_b[tmp1] > 9){	ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]-10;}else{	ws281x_buffer_b[tmp1] = 0;}//		��-

					//�ԂɂȂ�Ύ�
					if(ws281x_buffer_r[tmp1] == 255 && ws281x_buffer_g[tmp1] == 0 && ws281x_buffer_b[tmp1] == 0){
						ws281x_checkcount++;
					}
				}
				if(ws281x_checkcount == ws281x_led_value){
					ws281x_status = 1;
				}

			}else if(ws281x_status == 1){
				//�΂ɂ���
				ws281x_checkcount = 0;
				for(tmp1=0;tmp1<ws281x_led_value;tmp1++){
					//if(ws281x_buffer_r[tmp1] < 246){ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]+10;}else{ws281x_buffer_r[tmp1] = 255;}//	��+
					if(ws281x_buffer_g[tmp1] < 246){ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]+10;}else{ws281x_buffer_g[tmp1] = 255;}//		��+
					//if(ws281x_buffer_b[tmp1] < 246){ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]+10;}else{ws281x_buffer_b[tmp1] = 255;}//	��+
					if(ws281x_buffer_r[tmp1] > 9){	ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]-10;}else{	ws281x_buffer_r[tmp1] = 0;}//		��-
					//if(ws281x_buffer_g[tmp1] > 9){	ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]-10;}else{	ws281x_buffer_g[tmp1] = 0;}//	��-
					if(ws281x_buffer_b[tmp1] > 9){	ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]-10;}else{	ws281x_buffer_b[tmp1] = 0;}//		��-

					//�΂ɂȂ�Ύ�
					if(ws281x_buffer_r[tmp1] == 0 && ws281x_buffer_g[tmp1] == 255 && ws281x_buffer_b[tmp1] == 0){
						ws281x_checkcount++;
					}
				}

				if(ws281x_checkcount == ws281x_led_value){
					ws281x_status = 2;
				}

			}else if(ws281x_status == 2){
				//�G�������h�O���[���ɂ���
				ws281x_checkcount = 0;
				for(tmp1=0;tmp1<ws281x_led_value;tmp1++){
					//if(ws281x_buffer_r[tmp1] < 246){ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]+10;}else{ws281x_buffer_r[tmp1] = 255;}//	��+
					//if(ws281x_buffer_g[tmp1] < 246){ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]+10;}else{ws281x_buffer_g[tmp1] = 255;}//	��+
					if(ws281x_buffer_b[tmp1] < 49){ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]+10;}else{ws281x_buffer_b[tmp1] = 48;}//		��+
					if(ws281x_buffer_r[tmp1] > 9){	ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]-10;}else{	ws281x_buffer_r[tmp1] = 0;}//		��-
					if(ws281x_buffer_g[tmp1] > 9){	ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]-10;}else{	ws281x_buffer_g[tmp1] = 0;}//		��-
					//if(ws281x_buffer_b[tmp1] > 9){	ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]-10;}else{	ws281x_buffer_b[tmp1] = 0;}//	��-

					//�ɂȂ�Ύ�
					if(ws281x_buffer_b[tmp1] == 48){
						ws281x_checkcount++;
					}
				}
				if(ws281x_checkcount == ws281x_led_value){
					ws281x_status = 0;
				}

			}
		}else if(mode == 13){
			//�S�̂�Ԃ���I�����W�ŕω�����B

			if(ws281x_status == 0){
				//�S�Ă�^���Ԃɂ���
				ws281x_checkcount = 0;
				for(tmp1=0;tmp1<ws281x_led_value;tmp1++){
					if(ws281x_buffer_r[tmp1] < 246){ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]+10;}else{ws281x_buffer_r[tmp1] = 255;}//		��+
					//if(ws281x_buffer_g[tmp1] < 246){ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]+10;}else{ws281x_buffer_g[tmp1] = 255;}//	��+
					//if(ws281x_buffer_b[tmp1] < 246){ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]+10;}else{ws281x_buffer_b[tmp1] = 255;}//	��+
					//if(ws281x_buffer_r[tmp1] > 9){	ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]-10;}else{	ws281x_buffer_r[tmp1] = 0;}//	��-
					if(ws281x_buffer_g[tmp1] > 9){	ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]-10;}else{	ws281x_buffer_g[tmp1] = 0;}//		��-
					if(ws281x_buffer_b[tmp1] > 9){	ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]-10;}else{	ws281x_buffer_b[tmp1] = 0;}//		��-

					//�ԂɂȂ�Ύ�
					if(ws281x_buffer_r[tmp1] == 255 && ws281x_buffer_g[tmp1] == 0 && ws281x_buffer_b[tmp1] == 0){
						ws281x_checkcount++;
					}
				}
				if(ws281x_checkcount == ws281x_led_value){
					ws281x_status = 1;
				}

			}else if(ws281x_status == 1){
				//�΂ɂ���
				ws281x_checkcount = 0;
				for(tmp1=0;tmp1<ws281x_led_value;tmp1++){
					//if(ws281x_buffer_r[tmp1] < 246){ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]+10;}else{ws281x_buffer_r[tmp1] = 255;}//	��+
					if(ws281x_buffer_g[tmp1] < 81){ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]+10;}else{ws281x_buffer_g[tmp1] = 90;}//		��+
					//if(ws281x_buffer_b[tmp1] < 246){ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]+10;}else{ws281x_buffer_b[tmp1] = 255;}//	��+
					if(ws281x_buffer_r[tmp1] > 209){	ws281x_buffer_r[tmp1] = ws281x_buffer_r[tmp1]-10;}else{	ws281x_buffer_r[tmp1] = 200;}//		��-
					//if(ws281x_buffer_g[tmp1] > 9){	ws281x_buffer_g[tmp1] = ws281x_buffer_g[tmp1]-10;}else{	ws281x_buffer_g[tmp1] = 0;}//	��-
					if(ws281x_buffer_b[tmp1] > 9){	ws281x_buffer_b[tmp1] = ws281x_buffer_b[tmp1]-10;}else{	ws281x_buffer_b[tmp1] = 0;}//		��-

					//�΂ɂȂ�Ύ�
					if(ws281x_buffer_r[tmp1] == 200 && ws281x_buffer_g[tmp1] == 90 && ws281x_buffer_b[tmp1] == 0){
						ws281x_checkcount++;
					}
				}

				if(ws281x_checkcount == ws281x_led_value){
					ws281x_status = 0;
				}

			}
		}else if(mode == 99){

		}else if(mode == 100){

		}
	}else{
		ws281x_mode_weight_count++;
	}

}

//�J���[�v���Z�X
//�e�Fbuffer��������LED_OUT����o�͂���B
void ws281x_process(){
	long tmp1 = 0;

	int tmp_buffer_1 = 0xff;
	int tmp_buffer_2 = 0xff;
	int tmp_buffer_3 = 0xff;
	int tmp_t1 = 1;
	int tmp_t2 = 5;
	int tmp_t3 = 4;

	//�^�C�~���O�����蓖��
	if(ws281x_timing == 0){

		//LED �@
		//RESET
		output_bit(LED_OUT,1);//DEF
		output_bit(LED_OUT,0);//RESET
		delay_ms(1);//���t�@�����X�ł�50uSec�������̒ʂ肾�ƂP�Y���邱�Ƃ�����B
		//delay_ms(20);//���t�@�����X�ł�50uSec�������̒ʂ肾�ƂP�Y���邱�Ƃ�����B

		//�f�[�^���M
		for(tmp1 = 0;tmp1<ws281x_led_value;tmp1++){
			if(Illumi_pintype == 0){
				//0=grb
				tmp_buffer_1 = ws281x_buffer_g[tmp1];
				tmp_buffer_2 = ws281x_buffer_r[tmp1];
				tmp_buffer_3 = ws281x_buffer_b[tmp1];
			}else if(Illumi_pintype == 1){
				//1=rgb
				tmp_buffer_1 = ws281x_buffer_r[tmp1];
				tmp_buffer_2 = ws281x_buffer_g[tmp1];
				tmp_buffer_3 = ws281x_buffer_b[tmp1];
			}

			//��ڂ̃f�[�^�𑗐M
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_1,7));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_1,6));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_1,5));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_1,4));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_1,3));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_1,2));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_1,1));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_1,0));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);

			//��ڂ̃f�[�^�𑗐M
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_2,7));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_2,6));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_2,5));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_2,4));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_2,3));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_2,2));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_2,1));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_2,0));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);

			//�O�ڂ̃f�[�^�𑗐M
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_3,7));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_3,6));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_3,5));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_3,4));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_3,3));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_3,2));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_3,1));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			output_bit(LED_OUT,1);delay_cycles(1);			output_bit(LED_OUT,bit_test(tmp_buffer_3,0));delay_cycles(5);			output_bit(LED_OUT,0);delay_cycles(4);
			
			delay_us(1);delay_cycles(3);
		}

		//end
		output_bit(LED_OUT,1);//
	}else if(ws281x_timing == 1){

		//RESET
		output_bit(LED_OUT,1);//DEF
		output_bit(LED_OUT,0);//RESET
		delay_us(55);//���t�@�����X�ł�50uSec�������̒ʂ肾�ƂP�Y���邱�Ƃ�����B

		//�f�[�^���M
		for(tmp1 = 0;tmp1<ws281x_led_value;tmp1++){
			if(Illumi_pintype == 0){
				//0=grb
				tmp_buffer_1 = ws281x_buffer_g[tmp1];
				tmp_buffer_2 = ws281x_buffer_r[tmp1];
				tmp_buffer_3 = ws281x_buffer_b[tmp1];
			}else if(Illumi_pintype == 1){
				//1=rgb
				tmp_buffer_1 = ws281x_buffer_r[tmp1];
				tmp_buffer_2 = ws281x_buffer_g[tmp1];
				tmp_buffer_3 = ws281x_buffer_b[tmp1];
			}

			//��ڂ̃f�[�^�𑗐M
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_1,7));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_1,6));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_1,5));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_1,4));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_1,3));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_1,2));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_1,1));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_1,0));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);

			//��ڂ̃f�[�^�𑗐M
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_2,7));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_2,6));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_2,5));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_2,4));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_2,3));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_2,2));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_2,1));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_2,0));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);

			//�O�ڂ̃f�[�^�𑗐M
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_3,7));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_3,6));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_3,5));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_3,4));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_3,3));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_3,2));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_3,1));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			output_bit(LED_OUT,1);delay_cycles(2);			output_bit(LED_OUT,bit_test(tmp_buffer_3,0));delay_cycles(2);			output_bit(LED_OUT,0);delay_cycles(6);
			
			//delay_us(1);delay_cycles(3);
		}
		//end
		output_bit(LED_OUT,1);//
	}



}



