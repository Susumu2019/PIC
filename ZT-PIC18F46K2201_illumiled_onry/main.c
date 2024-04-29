#include <18f46k22.h>
#device ADC=10
#include <string.h>
#include <stdlib.h>
#fuses INTRC_IO,WDT_SW,WDT64,MCLR,PROTECT,NOBROWNOUT	//WDT�L��	MCLR�L��
//#fuses INTRC_IO,WDT_SW,WDT64,NOMCLR,PROTECT,NOBROWNOUT	//WDT�L��	MCLR����
//#fuses INTRC_IO,NOWDT,NOMCLR,PROTECT,NOBROWNOUT				//WDT����	MCLR����
//#fuses INTRC_IO,NOWDT,NOLVP,NOMCLR,PROTECT,NOBROWNOUT
#use delay(CLOCK=64MHz)//,RESTART_WDT
#use rs232(baud=19200,xmit=PIN_C6,rcv=PIN_C7,parity=N,bits=8,stream=c1,ERRORS)//main connect.
//#use rs232(baud=19200,xmit=PIN_D6,rcv=PIN_D7,parity=N,bits=8,stream=c2,ERRORS)//
#use i2c(MASTER, SDA=PIN_C4, SCL=PIN_C3,SLOW,FORCE_HW)//FAST=400kbps SLOW=100kbps 

//�ϐ����蓖��
//�V�X�e���֌W
char ver[]="202005240517";
//char ver[]="201910311211";
int cpu_startup_case = 0;
//�t���b�J�[�֌W
int flicker_10 = 0,		flicker_10_one = 0;
int flicker_100 = 0,	flicker_100_one = 0;
int flicker_500 = 0,	flicker_500_one = 0;
int flicker_1000 = 0,	flicker_1000_one = 0;
long counttimer1 = 0;
long counttimer2 = 0;
long counttimer3 = 0;
long counttimer4 = 0;
//int output_process_one = 0;
int lcd_process_one = 0;
int rtc_process_one = 0;
int input_process_one = 0;
int servo_process_one = 0;
int illumiled_process_one = 0;

//�A�i���O�v���֌W
int analog_index = 0;
long analog_value[2] = {0,0};

//�f�W�^�����͊֌W
int ra0 = 0,ra1 = 0,ra2 = 0,ra3 = 0,ra4 = 0,ra5 = 0,ra6 = 0,ra7 = 0;
//int ra0_re = 0,ra1_re = 0,ra2_re = 0,ra3_re = 0,ra4_re = 0,ra5_re = 0,ra6_re = 0,ra7_re = 0;
int rb0 = 0,rb1 = 0,rb2 = 0,rb3 = 0,rb4 = 0,rb5 = 0;//,rb6 = 0,rb7 = 0;
//int rb0_re = 0,rb1_re = 0,rb2_re = 0,rb3_re = 0,rb4_re = 0,rb5_re = 0;//,rb6_re = 0,rb7_re = 0;
long re0 = 0,re1 = 0;
int sw1 = 0,sw1_re = 0,sw2 = 0,sw2_re = 0,sw3 = 0,sw3_re = 0,sw4 = 0,sw4_re = 0;
long sw1_count_longpress = 0;//�X�C�b�`�̒������J�E���g

//�V���A���֌W
char punctuate[] = ",";//�V���A������̕�����̋�؂�
int get_command_value_c1 = 0;
char get_command_c1[160] = {
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};//��M����R�}���h���ɂ��킹��
char compare_command_c1[10] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
//int get_command_value_c2 = 0;
//char get_command_c2[10] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
//char compare_command_c2[10] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

//���̑�
long mode = 10;
//long mode_test = 0;
//long lcd_mode = 10;
//int lcd_row = 0;
long cursor_index = 0;
//int step = 0;
int step_one = 0;
//long step_count = 0;
int servo_angle = 0;
int servo_target_angle = 0;
int servo_step_angle = 0;
int servo_turn = 0;
int servo_position = 0;
int servo_start = 0;
long servo_time = 0;
int servo_rotation = 0;
//int32 test_val = 0;
//long step = 0;//���C���X�e�b�v
//int led_index = 0;
long timer0_value = 0;
int life_index = 0;
char life_char[] = "/-_.";
//int mode = 10;
//int weight = 0;
//long return_count = 0;

//�֐��錾
void startup();
void serial_process1();
void serial_process2();
void input_process();
void output_process();
void sw_process();
void modeselect_process();
void modeselect_lcd_process();
void hardwareinfo_process();
void hardwareinfo_lcd_process();
void io_process();
void io_lcd_process();
void illumiled_process();
void illumiled_lcd_process();
void lcd_process();
void servo_process();
void uart_process();
void graphic_process();

void save_mode(long save_mode);

//�`�F�b�NOK
#include "process_rtc.c"//�g�p����ꍇ�̓t�@�C�����Ŏg�p���郂�W���[���̃��C�u�������L���ɂ��鎖�B
#include "process_illumiled.c"//�g�p����ꍇ�̓t�@�C�����Ŏg�p���郂�W���[���̃��C�u�������L���ɂ��鎖�B
#include "process_lcd128x64dot.c"//�g�p����ꍇ�̓t�@�C�����Ŏg�p���郂�W���[���̃��C�u�������L���ɂ��鎖�B
//#include "process_lcd16x2txt.c"

//#include "am2321.c"//���x�E���x�Z���T�[
//#include "lm13232afw.c"//�O���t�B�b�NLCD���W���[��
//#include "tc74hc595.c"//�o�̓��W�b�NIC
//#include "sw_process.c"//�X�C�b�`����
//#include "tc74hc165.c"//���̓��W�b�NIC

//�V���A���̊��荞��
#int_rda
void isr_rcv(){
	while(kbhit(c1)){  // �}�C�R���̎�M�o�b�t�@���̃f�[�^��S�Ď�M����
		get_command_c1[get_command_value_c1] = fgetc(c1);//��M�f�[�^���o��
		fprintf(c1,"%c",get_command_c1[get_command_value_c1]);
		get_command_value_c1++;//�C���f�b�N�X�{�P
	}
}

#int_timer0
void int_timer(){
	//��10msec����

	set_timer0(timer0_value);//�^�C�}�[�l�Z�b�g
	
	flicker_10 = !flicker_10;
	flicker_10_one = 1;	
	step_one = 1;
	input_process_one = 1;
	//ws2812_process_one = 1;
	servo_process_one = 1;

	lcd_process_one++;
	if(lcd_process_one == 1){
		//��LCD��������{�I��20msec�Ƃ��ALCD�Ƃ͂��Ԃ�Ȃ��悤�ɂ��鏈���͂����ɓ����

		//�C���~�l�[�V���������p�t���O
		//��10msec�X�V�ɂ���ꍇ��LCD�ȂǃC���~�ȊO�𖳌��Ƃ��鎖
		illumiled_process_one++;

	}

	//�A�i���O�l�擾
	//analog_value[analog_index] = read_adc()*4;
	if(analog_index == 0){
		re0 = read_adc()*2;

		analog_index = 1;
		set_adc_channel(6);//RE1(AN6)
	}else if(analog_index == 1){
		re1 = read_adc()*2;
		
		analog_index = 0;
		set_adc_channel(5);//RE0(AN5)
	}
	
	if(counttimer1 > 9){
		//100msec�P�ʂ̎��s

		counttimer1 = 0;//�J�E���^�[�����Z�b�g

		flicker_100 = !flicker_100;
		flicker_100_one = 1;

		//���C�t�\���p����
		if(life_index == 3){
			life_index = 0;
		}else{
			life_index++;
		}
	}
	counttimer1++;

	if(counttimer2 > 49){
		//��500msec�P�ʂ̎��s
		
		counttimer2 = 0;//�J�E���^�[�����Z�b�g

		flicker_500 = !flicker_500;
		flicker_500_one = 1;
	}
	counttimer2++;

	if(counttimer3 > 99){
		//��1sec�P�ʂ̎��s
		
		counttimer3 = 0;//�J�E���^�[�����Z�b�g
		
		flicker_1000 = !flicker_1000;
		flicker_1000_one = 1;

		//���x�E���x���v��
		//���Q��ɂP��̊����Ōv��
		//am2321_read_one = 1;

		//rtc
		rtc_process_one = 1;
	}
	counttimer3++;
	counttimer4++;

}

//�X�^�[�g�A�b�v
void startup(){

	//�N���b�N�ݒ�
	setup_oscillator(OSC_16MHZ|OSC_PLL_ON);

	setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256 );//
	enable_interrupts(INT_TIMER0); //�^�C�}���荞�݋���
	enable_interrupts(INT_RDA); //�V���A�����荞�݋���
	enable_interrupts(GLOBAL); //�S���荞�݋���
	setup_wdt(WDT_ON);

	//�^�C�}�[���荞�ݐݒ�
	timer0_value = 0xffff - 627;
	set_timer0(timer0_value);
	//set_timer1(0);

	//�A�i���O�|�[�g�̐ݒ�
	setup_vref(VREF_2v048);
	//setup_vref(VREF_4v096);
	//setup_vref(VREF_ON|VREF_ADC_4v096);
	setup_adc(ADC_CLOCK_INTERNAL);
	setup_adc_ports(sAN5|sAN6,VSS_FVR);//RE0(AN5),RE1(AN6)
	//setup_adc_ports(sAN12,VSS_FVR);//RB0

	//�|�[�g�ݒ� 0:OUT 1:IN
	set_tris_a(0b00001111);//PORT1
	set_tris_b(0b00000000);//PORT
	set_tris_c(0b10011000);//UART,I2C,PORT
	set_tris_d(0b00000000);//PORT
	set_tris_e(0b00000011);//PORT

	cpu_startup_case = restart_cause();//�N�����[�h���L�^

	//�ݒ�l�ǂݍ���
	//���C�����[�h
	mode = READ_EEPROM(0x00) << 8;	mode = mode + READ_EEPROM(0x01);//
	if(mode == 0xffff){		mode = 701;}//���ݒ�Ȃ�f�t�H���g�l�ݒ�
	//�C���~�l�[�V����
	Illumi_color_set = 	READ_EEPROM(0x10);	if(Illumi_color_set == 0xff){Illumi_color_set = 3;}
	Illumi_mode_set = 	READ_EEPROM(0x11);	if(Illumi_mode_set == 0xff){Illumi_mode_set = 1;}
	Illumi_speed = 		READ_EEPROM(0x12);	if(Illumi_speed == 0xff){Illumi_speed = 1;}
	Illumi_signaltype = READ_EEPROM(0x13);	if(Illumi_signaltype == 0xff){Illumi_signaltype = 0;}//0=grb,1=rgb
	Illumi_timing = 	READ_EEPROM(0x14);	if(Illumi_timing == 0xff){Illumi_timing = 0;}//�^�C�~���O�p�^�[��
	Illumi_brightness = READ_EEPROM(0x15);	if(Illumi_brightness == 0xff){Illumi_brightness = 10;}//���邳

	//���C�����[�h��LCD���[�h�ɔ��f
	//lcd_mode = mode;

	if(mode == 0){
		cursor_index = 1;
	}else if(mode == 10){					cursor_index = 1;
	}else if(mode >= 100 && mode < 199){	cursor_index = 1;
	}else if(mode == 600){					cursor_index = 1;
	}else if(mode == 701){
		cursor_index = 1;
		ws281x_color_set(Illumi_color_set);
	}else if(mode == 702){
		cursor_index = 6;
		ws281x_color_set(Illumi_color_set);
	}else{				cursor_index = 0;}

	//�d��������A�e���W���[���N����������̂�҂�
	//delay_ms(100);
	//delay_ms(500);

	//�E�H�b�`�h�b�O�^�C�}�[���Z�b�g
	restart_wdt();

	//LCD������
	lcd_ini();
	//��AQM1602A����AQM0802A�̏ꍇ�͎g�p�d�����w��
	
	//�X�^�[�g�X�e�b�v
	//step = 0;
	//delay_ms(100);
	//lcd_start(1);	printf(lcd_char,"Hello!               -");
	//lcd_start(0);	printf(lcd_char,"Mode=%03lu             -",mode);
	//lcd_start(2);	printf(lcd_char,"cursor_index=%03lu     -",cursor_index);
	//lcd_start(3);	printf(lcd_char,"                     -");
	//lcd_start(4);	printf(lcd_char,"                     -");
	//lcd_start(5);	printf(lcd_char,"                     -");
	//lcd_start(6);	printf(lcd_char,"                     -");
	//lcd_start(7);	printf(lcd_char,"                     -");
	//delay_ms(500);
}

//���C������
void main(){

	//�X�^�[�g�A�b�v
	startup();

	while(true){

		restart_wdt();// WDT timer reset

		//���͏���
		if(input_process_one != 0){
			input_process_one = 0;

			input_process();
		}

		//�����x�v��
		//if(am2321_read_one != 0){
		//	am2321_read_one = 0;

		//	am2321_read();
		//}

		if(mode >= 700 && mode < 800){
			//modeselect_process();

			//�C���~�l�[�V����LED����
			illumiled_process();
			if(lcd_process_one > 1){	lcd_process_one = 0;	illumiled_lcd_process();}
		}else{
			//LCD����
			//��LCD����������Ă��Ȃ������炱���ŏ���
			if(lcd_process_one > 0){	lcd_process_one = 0;	lcd_process();	}
		}

		//�o�͏���
		output_process();
	}
}

//void lcd_process(){
//
//	aqmxx02a_setcursor(0,0);
//	printf(aqmxx02a_puts,"20191006 2132  e");
//	aqmxx02a_setcursor(0,1);
//	printf(aqmxx02a_puts,"zeatec.jp      %c",life_char[life_index]);
//
//	//AQM1602A�̏ꍇ
//	//aqmxx02a_setcursor(0,0);
//	//printf(aqmxx02a_puts,"analog=%04lu,%04lu",analog_value[0],analog_value[1]);
//	//aqmxx02a_setcursor(0,1);
//	//printf(aqmxx02a_puts,"zeatec.jp      %c",life_char[life_index]);
//
//	//lcd_start(1);printf(lcd_char,"ZEATEC co.,ltd. ");
//	//lcd_start(2);printf(lcd_char," PIC16F46K22+LCD");
//
//	//float f;
//	//f=12.345;
//	//printf(tostring,"\f%6.3f",f);
//}

//���̓|�[�g���擾
void input_process(){
	//��10msec�ŏ���

	ra0 = !input(PIN_A0);sw1 = ra0;
	ra1 = !input(PIN_A1);sw2 = ra1;
	ra2 = !input(PIN_A2);sw3 = ra2;
	ra3 = !input(PIN_A3);sw4 = ra3;
	//ra4 = !input(PIN_A4);
	//ra5 = !input(PIN_A5);
	//ra6 = !input(PIN_A6);
	//ra7 = !input(PIN_A7);

	//�I���^�l�C�g����ɂ���ꍇ
	//ra0 = !input(PIN_A0);	if(ra0_re != ra0 && ra0 == 0){	sw1 = 1;}	ra1_re = ra1;
	//ra1 = !input(PIN_A1);	if(ra1_re != ra1 && ra1 == 0){	sw2 = 1;}	ra2_re = ra2;
	//ra2 = !input(PIN_A2);	if(ra2_re != ra2 && ra2 == 0){	sw3 = 1;}	ra3_re = ra3;	
	//ra3 = !input(PIN_A3);	if(ra3_re != ra3 && ra3 == 0){	sw4 = 1;}	ra4_re = ra4;

	if(sw1 == 0){
		sw1_count_longpress = 0;
	}else if(sw1 == 1 && sw1_count_longpress < 100){
		sw1_count_longpress++;
	}else if(sw1 == 1 && sw1_count_longpress == 100){
		mode = 10;
		
	}

}

//�o�̓|�[�g����
void output_process(){

	//output_bit(PIN_A0,!0);
	//output_bit(PIN_A1,!0);
	//output_bit(PIN_A2,!0);
	//output_bit(PIN_A3,!0);
	ra4 = 0;output_bit(PIN_A4,!ra4);
	ra5 = 0;output_bit(PIN_A5,!ra5);
	//ra6 = 0;output_bit(PIN_A6,!ra6);//�C���~�l�[�V����LED�Ŏg�p�ׂ̈����ł͖���
	//output_bit(PIN_A7,!flicker_10);//�T�[�{�v���Z�X�Ŏg�p�ׂ̈����ł͖���

	output_bit(PIN_B0,!0);//rb0//flicker_500
	output_bit(PIN_B1,!0);//rb1//flicker_500
	output_bit(PIN_B2,!flicker_100);//rb2
	output_bit(PIN_B3,!flicker_500);//rb3
	output_bit(PIN_B4,!flicker_1000);//rb4
	output_bit(PIN_B5,!flicker_10);//rb5

	//
	//output_bit(PIN_C0,!flicker_10);
	//output_bit(PIN_C1,!flicker_1000);
	//output_bit(PIN_C2,!flicker_10);
	//output_bit(PIN_C5,flicker_1000);

	//output_bit(PIN_D0,!flicker_10);
	//output_bit(PIN_D1,!flicker_1000);
	//output_bit(PIN_D2,!flicker_10);
	//output_bit(PIN_D3,!flicker_1000);
	//output_bit(PIN_D4,!flicker_10);
	//output_bit(PIN_D5,!flicker_1000);
	//output_bit(PIN_D6,!flicker_10);
	//output_bit(PIN_D7,!flicker_1000);

	//output_bit(PIN_E0,!flicker_100);//flicker_1000
	//output_bit(PIN_E1,!flicker_500);//flicker_1000
	//output_bit(PIN_E2,!flicker_100);

	switch(cpu_startup_case){
		//restart list.
		//WDT_TIMEOUT       7
		//MCLR_FROM_SLEEP  11
		//MCLR_FROM_RUN    15
		//NORMAL_POWER_UP  12
		//BROWNOUT_RESTART 14
		//WDT_FROM_SLEEP    3
		//RESET_INSTRUCTION 0
		case WDT_TIMEOUT:		output_bit(PIN_E2,!flicker_10);		break;
		case MCLR_FROM_SLEEP:	output_bit(PIN_E2,!flicker_100);	break;
		case MCLR_FROM_RUN:		output_bit(PIN_E2,!flicker_100);	break;		
		case NORMAL_POWER_UP:	output_bit(PIN_E2,!flicker_1000);	break;
		case BROWNOUT_RESTART:	output_bit(PIN_E2,!flicker_100);	break;		
		case WDT_FROM_SLEEP:	output_bit(PIN_E2,!flicker_100);	break;
		case RESET_INSTRUCTION:	output_bit(PIN_E2,!flicker_100);	break;
		default:				output_bit(PIN_E2,!flicker_500);	break;
	}

	//PORT1

	//PORT2

	//PORT3

}

//�X�C�b�`����(�e���v���[�g)
/* void sw_process(){
	int tmp_sw1_true = 0,tmp_sw2_true = 0,tmp_sw3_true = 0,tmp_sw4_true = 0;

	//�X�C�b�`����
	if(sw1_re != sw1 && sw1 == 0){	tmp_sw1_true = 1;}	sw1_re = sw1;
	if(sw2_re != sw2 && sw2 == 0){	tmp_sw2_true = 1;}	sw2_re = sw2;
	if(sw3_re != sw3 && sw3 == 0){	tmp_sw3_true = 1;}	sw3_re = sw3;
	if(sw4_re != sw4 && sw4 == 0){	tmp_sw4_true = 1;}	sw4_re = sw4;

	//enter
	if(tmp_sw1_true != 0){tmp_sw1_true = 0;
	}

	//cansel
	if(tmp_sw2_true != 0){tmp_sw2_true = 0;
	}

	//
	if(tmp_sw3_true != 0){tmp_sw3_true = 0;
	}
	//
	if(tmp_sw4_true != 0){tmp_sw4_true = 0;
	}
}
 */

//���[�h�Z���N�g
/* void modeselect_process(){
	int tmp_sw1_true = 0,tmp_sw2_true = 0,tmp_sw3_true = 0,tmp_sw4_true = 0;

	//�X�C�b�`����
	if(sw1_re != sw1 && sw1 == 0){	tmp_sw1_true = 1;}	sw1_re = sw1;
	if(sw2_re != sw2 && sw2 == 0){	tmp_sw2_true = 1;}	sw2_re = sw2;
	if(sw3_re != sw3 && sw3 == 0){	tmp_sw3_true = 1;}	sw3_re = sw3;
	if(sw4_re != sw4 && sw4 == 0){	tmp_sw4_true = 1;}	sw4_re = sw4;

	//enter
	if(tmp_sw1_true != 0){tmp_sw1_true = 0;

		if(mode <10){
			//�I�[�v�j���O���

			mode = 10;	
			if(cursor_index == 1){
				//������\��
				save_mode(0);
			}else if(cursor_index == 2){
				//����͕\�����Ȃ����������[�ɕۑ�
				save_mode(mode);
			}
			cursor_index = 1;
		}else if(mode >= 10 && mode <100){
			//���[�h�I�����

			mode = cursor_index*100;
			//lcd_mode = mode;

			save_mode(mode);//�������[�ɕۑ�

			//���y�[�W�ւ̃J�[�\������
			if(cursor_index == 1){			cursor_index = 1;//�n�[�h�E�F�A�C���t�H
			}else if(cursor_index == 2){
			}else if(cursor_index == 6){	cursor_index = 1;//�T�[�{
			}else if(cursor_index == 7){	cursor_index = 1;//�C���~�l�[�V����LED��
			}else{							cursor_index = 0;	}
		}

	}

	//cansel
	if(tmp_sw2_true != 0){tmp_sw2_true = 0;	}

	//up
	if(tmp_sw3_true != 0){tmp_sw3_true = 0;
		//�J�[�\�������
		if(mode < 10){						if(cursor_index > 1){	cursor_index--;	}
		}else if(mode >= 10 && mode <100){	if(cursor_index > 1){	cursor_index--;	}}
	}
	//down
	if(tmp_sw4_true != 0){tmp_sw4_true = 0;
		//�J�[�\��������
		if(mode <10){						if(cursor_index < 2){	cursor_index++;	}
		}else if(mode >= 10 && mode <100){	if(cursor_index < 12){	cursor_index++;	}}
	}
} */

//�V���A���|�[�g����̃R�}���h������
void serial_process1(){
	int tmp1 = 0;
	char *command;
	//long long_param1;
	//char *str_param1;
	//char *str_param2;
	//int rdata = 0;
	//int tmp_alarm_id = 0;

	if(get_command_c1[get_command_value_c1-1]==0xd){
		//�R�}���h�ƃp�����[�^�𕪊�
		command = strtok(get_command_c1,punctuate);
		//param1 = atoi(strtok(null,punctuate));
		//param1 = strtok(null,punctuate);

		fprintf(c1,"\r\n");

		//�w���v
		strcpy(compare_command_c1,"?\r");
		if(strcmp(command,compare_command_c1)==0){
			fprintf(c1,"ZT-****-SOFT Ver.0.90 UART1\r\n");
			fprintf(c1,"analog_value=%04ldmV\r\n",analog_value);
			fprintf(c1,"ra1=%d\r\n",ra1);
			//fprintf(c1,"temperature=%ld\r\n",temperature);//���x
			//fprintf(c1,"humidity=%ld\r\n",humidity);//���x
			//fprintf(c1,"0=SUN,1=mon,2=tue,3=wed,4=thu,5=fri,6=sat,7=all,8=week,9=off,\r\n");
			
		}

		//strcpy(compare_command_c1,"p");
		//if(strcmp(command,compare_command_c1)==0){
		//	//�p�����[�^�̎擾
		//	//��up,1�v��1��param1�ɓ���
		//	fprintf(c1,"param1_u=%u ok.\r\n",param1);
		//}

		//�S�ăN���A
		for(tmp1=0;tmp1<80;++tmp1){
			get_command_c1[tmp1] = 0x0;
		}
		get_command_value_c1 = 0;

		//�Q�Ɨp�̃R�}���h���N���A
		for(tmp1=0;tmp1<10;++tmp1){
			compare_command_c1[tmp1] = 0x0;
		}
	}
}

//�������[�Ƀ��[�h��ۑ�
void save_mode(long save_mode){
	long tmp_mode = 10;
	tmp_mode = save_mode;
	write_eeprom(0x01,tmp_mode);	tmp_mode = tmp_mode >> 8;
	write_eeprom(0x00,tmp_mode);
}