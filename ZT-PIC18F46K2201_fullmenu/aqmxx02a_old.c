/*****************************************************************************************
LCD���W���[��(aqmxx02a-RN-GBW)�p���C�u���� 2015/2/12�쐬

���񋟌�
ZEATEC co.,ltd. 

�������p�ɂ���
�]�ځE���f�g�p�ł��B���̃��C�u�������g�p������ł̕s����Ɋւ��ẮA�����Ȃ���e�ɂ���
�Ă���؂̐ӔC��ǂ�Ȃ����̂Ƃ��܂��B

���Ή��@��
AQM0802A-RN-GBW,AQM1602A-RN-GBW

���g�p���@
aqmxx02a_init(5)���X�^�[�g�A�b�v�ɓ���ĉ������B�J�b�R���͎g�p�d�����w�肵�܂��B3.3V�̎���
3�Ǝw�肵�Ă��������Bsetcursor_aqmxx02a(0,0)�ŕ\���ʒu���w�肵�Aaqmxx02a_puts("�����abc")
�ŔC�ӂ̕�������w�肷��B����printf(aqmxx02a_puts,"%02x:%02x:%02x",hour,min,sec);�Ƃ����
�ϐ��̒l���\���ł��܂��B
I2C�̓n�[�h�E�F�A����𐄏��Ƃ��A�錾��͉��L�̂Ƃ���B

#use i2c(MASTER, SDA=PIN_C4, SCL=PIN_C3,SLOW,FORCE_HW)//FAST=400kbps SLOW=100kbps
�s���A�T�C����PIC16F1947�Ŏg�p�����ꍇ�Ƃ��܂��B

*****************************************************************************************/
void aqmxx02a_init(int volt);
void aqmxx02a_setcursor(int col, int row);
void aqmxx02a_cmd(unsigned char c);
void aqmxx02a_puts(char s);
void aqmxx02a_clear();

void aqmxx02a_init(int volt){
	aqmxx02a_cmd(0x38) ;     // function set           : �f�[�^����8�{�E�\���͂Q�s�E�t�H���g��5x8�h�b�g
	aqmxx02a_cmd(0x39) ;     // function set           : �g���R�}���h�̐ݒ��L���ɂ���
	aqmxx02a_cmd(0x14) ;     // Internal OSC frequency : �o�C�A�X�̑I���Ɠ���OSC���g���̒���
	if(volt == 3){
		//�R���g���X�g����(3.3V)
		aqmxx02a_cmd(0x70) ;     // Contrast set           : �R���g���X�g�����f�[�^(����4�r�b�g)
		aqmxx02a_cmd(0x5e) ;     // Contrast set           : ������H�L���A�A�C�R���L���A�R���g���X�g�����f�[�^(����2�r�b�g)
		//aqmxx02a_cmd(0x56) ;     // Contrast set           : ������H�L���A�A�C�R�������A�R���g���X�g�����f�[�^(����2�r�b�g)
	}else{
		//�R���g���X�g����(5V)
		aqmxx02a_cmd(0x7A) ;     // Contrast set           : �R���g���X�g�����f�[�^(����4�r�b�g)
		aqmxx02a_cmd(0x5c) ;     // Contrast set           : ������H�L���A�A�C�R���L���A�R���g���X�g�����f�[�^(����2�r�b�g)	
		//aqmxx02a_cmd(0x54) ;     // Contrast set           : ������H�L���A�A�C�R�������A�R���g���X�g�����f�[�^(����2�r�b�g)	
	}
	aqmxx02a_cmd(0x6C) ;     // Follower control       : �t�H���A��H��ON�A�������̒������s��
	delay_ms(200) ;			// �d�͂����肷��܂ő҂�
	aqmxx02a_cmd(0x38) ;     // function set           : �g���R�}���h��ݒ�𖳌��ɂ���
	aqmxx02a_cmd(0x0C) ;     // display control        : ��ʕ\����ON�E�J�[�\���\����OFF�E�J�[�\���_�ł�OFF
	aqmxx02a_cmd(0x01) ;     // Clear Display : ��ʑS�̂�20H�̽�߰��ŕ\���A���ق�col=0,row=0�Ɉړ�
	delay_us(1100) ;  // LCD������(1.08ms)����̂�҂��܂�
}

void aqmxx02a_setcursor(int col, int row){
	int row_offsets[] = { 0x00, 0x40 };

	aqmxx02a_cmd(0x80 | (col + row_offsets[row])) ; // Set DDRAM Adddress : 00H-07H,40H-47H
}

void aqmxx02a_cmd(char c){
	//�X�^�[�g�R���f�B�V�����𔭍s����
	i2c_start();
	//LCD�̃A�h���X
	i2c_write(0x7c);
	// control byte �̑��M(�R�}���h���w��)
	i2c_write(0b10000000);
	// data byte �̑��M
	i2c_write(c);
	// �X�g�b�v�R���f�B�V�����𔭍s����
	i2c_stop();
	//delay_us(27);
}

void aqmxx02a_puts(char s){
	//�X�^�[�g�R���f�B�V�����𔭍s����
	i2c_start();
	//LCD�̃A�h���X
	i2c_write(0x7c);
	//control byte �̑��M(�f�[�^���w��)
	i2c_write(0b01000000);
	//data byte �̑��M
	i2c_write(s);
	//�X�g�b�v�R���f�B�V�����𔭍s����
	i2c_stop();
}

//�I���W�i���A�C�R���@��
char aqmxx02a_icon_1[7] = {
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b00100,
  0b00100,
  0b00000,
};

//�I���W�i���A�C�R���@��
char aqmxx02a_icon_2[7] = {
  0b00100,
  0b00100,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00000,
};

void LCD_CreateChar(int p,char *dt){

	int tmp1 = 0;

	i2c_start();     // �X�^�[�g�R���f�B�V�����𔭍s����
	//LCD�̃A�h���X
	i2c_write(0x7c);
	//  LCD�ɃL�����ۑ���̃A�h���X���w������
	i2c_write(0b10000000) ;             // control byte �̑��M(�R�}���h���w��)
	i2c_write(0x40 | (p << 3)) ;
	//delay_us(27) ;
	//  LCD�ɃL�����f�[�^�𑗐M����
	i2c_write(0b01000000) ;             // control byte �̑��M(�f�[�^���w��)
	for (tmp1=0; tmp1 < 7; tmp1++) {
	   i2c_write(*dt++) ;
	   //delay_us(27) ;
	}

	i2c_stop() ;                            // �X�g�b�v�R���f�B�V�����𔭍s����
}

