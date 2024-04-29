#define A0 PIN_C0
#define E PIN_C1
#define _RES PIN_C2

/*****************************************************************************************
�O���t�B�b�NLCD���W���[��(TOPWAY LM13232AFW)�p���C�u���� 2016/10/29�쐬

���񋟌�
ZEATEC co.,ltd. 

�������p�ɂ���
�]�ځE���f�g�p�ł��B���̃��C�u�������g�p������ł̕s����Ɋւ��ẮA�����Ȃ���e�ɂ���
�Ă���؂̐ӔC��ǂ�Ȃ����̂Ƃ��܂��B

���g�p���@
lcd_ini()���X�^�[�g�A�b�v�ɓ���A�p�r�ɍ��킹�ĉ��L�����I�i0.1sec�P�ʂ��炢�j�Ɏ��s����B
�t�H���g�̃f�[�^��font_data.c���C�����邱�ƂŋL���Ȃǂ��ǉ��ł��܂��B�C���[�W�f�[�^�̏ꍇ�́A
image_data.c�f�[�^���C�����Ă��������B�P�o�C�g�P�ʂ̃f�[�^�쐬�͈ɓ� �N��l���LCD_Image��
�֗��ł��B
�P�o�C�g�P�ʂ̃r�b�g�}�b�v�f�[�^�̏ꍇ	lcd_bitmap8();
�S�o�C�g�P�ʂ̃r�b�g�}�b�v�f�[�^�̏ꍇ	lcd_bitmap32();
�e�L�X�g�̏ꍇ�͍s�ԍ����w�肵�Ă���e�L�X�g�𑗐M�B
�X�^�[�g�A�b�v�ɓ����\�[�X�͉��L�̒ʂ�B
	//�O���t�B�b�NLCD��\��
	//TOPWEY LM6029ACW����LM13232AFW
	//LCD������
	lcd_ini();


�܂��A�X�V���Ɏg�p����R�[�h�͉��L�̒ʂ�B

	//���S�\��
	lcd_bitmap8();	

	//�O���t�B�b�NLCD�̃e�L�X�g�\��
	//TOPWEY LM13232AFW
	lcd_start(0);
	printf(lcd_char,"ZEATEC co.,ltd. �ްï�");
	lcd_start(1);
	printf(lcd_char,"AN0 = %04lumV , RA1 = %u",analog_value,port1_ra1);
	lcd_start(2);
	printf(lcd_char,"TOPWEY LM13232AFW     ");
	lcd_start(3);
	printf(lcd_char,"Dot 132x32 , Text 22x4");
	lcd_start(3);
	printf(lcd_char,"%02x/%02x/%02x ",year,month,day);
	printf(lcd_char,c_week);
	printf(lcd_char," %02x:%02x:%02x ",hour,min,sec);

���ڑ����@
RE0 = DB0
RE1 = DB1
RE2 = DB2
RE3 = DB3
RE4 = DB4
RE5 = DB5
RE6 = DB6
RE7 = DB7
RF0 = A0 1:DisplayData 0:ControlData
RF1 = E Enable trigger
RF2 = /RES 1:Run 0:ini

��L�ȊO�̔z����LCD���W���[����܂��̓n�[�l�X�ɂ����VDD�����VSS�Ɛڑ�����B
R/W = Vss
/CS1 = Vss
BLA = Vdd
P/S = Vdd
*****************************************************************************************/

void lcd_ini();
void lcd_cmd();
void lcd_data();
void lcd_write();

const int contrast_lavel = 0x18;

//LCD�ɃR�}���h���M
void lcd_cmd(int cmd){

	output_bit(A0,0);// A0 0:Control data.
	output_bit(E,0);// E Enable trigger
	output_a(cmd);// DB0-7
	output_bit(E,1);// E Enable trigger
	output_bit(E,0);// E Enable trigger
}

//LCD�Ƀf�[�^���M
void lcd_data(int cmd){

	output_bit(A0,1);// A0 0:Control data.
	output_bit(E,0);// E Enable trigger
	output_a(cmd);// DB0-7
	output_bit(E,1);// E Enable trigger
	output_bit(E,0);// E Enable trigger
}

//LCD��������
void lcd_ini(){

	//�����l
	output_bit(_RES,!0);// /RES 0:stop.
	output_bit(A0,1);// A0 1:Display data. 0:Control data.
	output_bit(E,0);// E Enable trigger

	//hardware reset LCD module
	output_bit(_RES,1);// /RES SET
	output_bit(_RES,0);// /RES RESET
	delay_ms(5);
	output_bit(_RES,1);// /RES SET
	delay_ms(10);
	
	lcd_cmd(0xaf);//Display ON
	lcd_cmd(0x40);//Set display Start line=0
	lcd_cmd(0xa0);//ADC = 0:Normal 1:Reverse
	lcd_cmd(0xa6);//Normal display
	lcd_cmd(0xa4);//Disible display all point
	lcd_cmd(0xa2);//Bais = 0xa2:1/9 0xa3:1/7
	lcd_cmd(0xc8);//SHL Select 0xc8:Flipped in y direction 0xc0:Normal display
	lcd_cmd(0x2f);//Power control = all on
	
	lcd_cmd(0x23);//RA/RB setting
	lcd_cmd(0xf8);//Booster Rabit = 4x
	lcd_cmd(0x00);//(tow byte command)
	lcd_cmd(0x81);//E-Vol setting
	lcd_cmd(contrast_lavel);//(tow byte command)
}

void lcd_start(int row){
	lcd_cmd(0xb0+row);//select page 0-3
	lcd_cmd(0x10);//start form colum 4
	lcd_cmd(0x00);//(tow byte command)
}

void lcd_char(char c){
	lcd_data(font[c-0x20][0]);
	lcd_data(font[c-0x20][1]);
	lcd_data(font[c-0x20][2]);
	lcd_data(font[c-0x20][3]);
	lcd_data(font[c-0x20][4]);
	lcd_data(0x00);
}

//�r�b�g�}�b�v�f�[�^���M
void lcd_bitmap8(){

	int tmp_data;
	long i,j;

	for(i=0;i<4;i++){
		lcd_cmd(0xb0 | i);//select page 0-3
		lcd_cmd(0x10);//start form colum 4
		lcd_cmd(0x00);//(tow byte command)
		for(j=0;j<132;j++){
			tmp_data = lcd_image8[(i*132)+j];
			lcd_data(tmp_data);
		}
	}
}

void lcd_bitmap32(){

	int32 tmp_data1;
	int tmp_data2;
	long i,j;

	for(i=0;i<4;i++){
		lcd_cmd(0xb0 | i);//select page 0-3
		lcd_cmd(0x10);//start form colum 4
		lcd_cmd(0x00);//(tow byte command)
		for(j=0;j<132;j++){
			tmp_data1 = lcd_image32[j];
			tmp_data2 = tmp_data1>>(8*i);
			lcd_data(tmp_data2);
		}
	}
}
