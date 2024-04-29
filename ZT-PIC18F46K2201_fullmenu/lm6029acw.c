/*****************************************************************************************
�O���t�B�b�NLCD���W���[��(TOPWAY LM6029acw)�p���C�u���� 2016/4/1�쐬

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
lcd_start(0);
printf(lcd_char,"///////////////////// ");
lcd_start(1);
printf(lcd_char,"/  ZEATEC co.,ltd.  / ");
lcd_start(2);
printf(lcd_char,"/      �ްï�       / ");
lcd_start(3);
printf(lcd_char,"/                   / ");
lcd_start(4);
printf(lcd_char,"/AN0=%04lumV , RA1=%u / ",analog_value,port1_ra1);
lcd_start(5);
printf(lcd_char,"/TOPWEY LM6029ACW   / ");
lcd_start(6);
printf(lcd_char,"/Dot128x64,Text21x8 / ");
lcd_start(7);
printf(lcd_char,"///////////////////// ");

���ڑ����@
RE0 = DB0
RE1 = DB1
RE2 = DB2
RE3 = DB3
RE4 = DB4
RE5 = DB5
RE6 = DB6
RE7 = DB7
RF0 = /WR
RF1 = RS
RF2 = /RES

��L�ȊO�̔z����LCD���W���[����܂��̓n�[�l�X�ɂ����VDD�����VSS�Ɛڑ�����B
/CS1 = Vss
/RD = VDD
BLA = Vdd

*****************************************************************************************/

#define _WR PIN_C0
#define RS PIN_C1
#define _RES PIN_C2
#define BLA PIN_C5

int contrast_lavel = 0x16;
void lcd_start(int row);
void lcd_char(char c);

//LCD�ɃR�}���h���M
void lcd_cmd(int cmd){

	output_bit(RS,0);// RS 0:Control data.
	output_d(cmd);// DB0-7
	output_bit(_WR,!1);// /WR 0:Write enable input active.
	//delay_us(1);//
	output_bit(_WR,!0);// /WR 0:Write enable input active.
}

//LCD�Ƀf�[�^���M
void lcd_data(int cmd){

	output_bit(RS,1);// RS 1:Display data.
	output_d(cmd);// DB0-7
	output_bit(_WR,!1);// /WR 0:Write enable input active.
	//delay_us(1);//
	output_bit(_WR,!0);// /WR 0:Write enable input active.
}

//LCD��������
void lcd_ini(){

	//�����l
	output_bit(_RES,!0);// /RES 0:stop.
	output_bit(RS,1);// RS 1:Display data. 0:Control data.
	output_bit(_WR,!0);// /WR 0:Write enable input active.

	//hardware reset LCD module
	output_bit(_RES,1);// /RES SET
	output_bit(_RES,0);// /RES RESET
	delay_ms(5);
	output_bit(_RES,1);// /RES SET
	delay_us(3);
	
	//contrast_lavel = 0x20;//Default Contrast Level

	lcd_cmd(0xaf);//1.Display ON
	lcd_cmd(0x40);//2.Set display Start line=0
	lcd_cmd(0xa0);//3.ADC = 0:Normal 1:Reverse
	lcd_cmd(0xa6);//4.Normal display
	lcd_cmd(0xa4);//5.Disible display all point
	lcd_cmd(0xa2);//6.Bais = 0xa2:1/9 0xa3:1/7
	lcd_cmd(0xc8);//7.SHL Select 0xc8:Flipped in y direction 0xc0:Normal display
	lcd_cmd(0x2f);//8.Power control = all on
	
	lcd_cmd(0x25);//9.RA/RB setting
	//lcd_cmd(0xf8);//Booster Rabit = 4x
	//lcd_cmd(0x00);//(tow byte command)
	lcd_cmd(0x81);//10.E-Vol setting
	lcd_cmd(contrast_lavel);//11.(tow byte command)

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

//�\���f�[�^���M
void lcd_bitmap8(){

	int tmp_data;
	long i,j;

	for(i=0;i<8;i++){
		lcd_cmd(0xb0 | i);//select page 0-7
		lcd_cmd(0x10);//start form colum 4
		lcd_cmd(0x00);//(tow byte command)
		for(j=0;j<128;j++){
			tmp_data = lcd_image8[(i*128)+j];
			lcd_data(tmp_data);
		}
	}
}
void lcd_bla(int on){
	output_bit(BLA,on);//
}





