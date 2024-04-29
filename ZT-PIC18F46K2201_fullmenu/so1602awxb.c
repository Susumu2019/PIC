/*****************************************************************************************
LCD���W���[��(SO1602AWGB-UC-WB-U,SO1602AWWB-UC-WB-U,SO1602AWYB-UC-WB-U)
�p���C�u���� 2015/2/14�쐬

���񋟌�
ZEATEC co.,ltd. 

�������p�ɂ���
�]�ځE���f�g�p�ł��B���̃��C�u�������g�p������ł̕s����Ɋւ��ẮA�����Ȃ���e�ɂ���
�Ă���؂̐ӔC��ǂ�Ȃ����̂Ƃ��܂��B

���g�p���@
lcd_ini()���X�^�[�g�A�b�v�ɓ���Alcd_char("�����abc")�ŔC�ӂ̕������\����
��B����printf(lcd_char,"    %02x:%02x:%02x    ",hour,min,sec);��printf���g�p���ďo
�͂���B
I2C�̓n�[�h�E�F�A����𐄏��Ƃ��A�錾��͉��L�̂Ƃ���B

#use i2c(MASTER, SDA=PIN_C4, SCL=PIN_C3,SLOW,FORCE_HW)//FAST=400kbps SLOW=100kbps
�s���A�T�C����PIC16F1947�Ŏg�p�����ꍇ�Ƃ��܂��B

�X�^�[�g�A�b�v�ɓ����\�[�X
	//�L�@EL�L�����N�^���W���[���̏�����
	//lcd_ini();

	////�L�@EL�L�����N�^���W���[����\��
	////�P�s�ڕ\��
	//lcd_start(1);
	//lcd_char("ZEATEC co.,ltd. ");
	////�Q�s�ڕ\��
	//lcd_start(2);
	//printf(lcd_char,	"�)�ްï�        ");

*****************************************************************************************/

void lcd_cmd(char c){
	//�X�^�[�g�R���f�B�V�����𔭍s����
	i2c_start();
	//LCD�̃A�h���X
	i2c_write(0x78);
	// control byte �̑��M(�R�}���h���w��)
	i2c_write(0x00);
	// data byte �̑��M
	i2c_write(c);
	// �X�g�b�v�R���f�B�V�����𔭍s����
	i2c_stop();
}

void so1602awxb_contrast(int cont){
	lcd_cmd(0x2a);//RE=1
	lcd_cmd(0x79);//SD=1
	lcd_cmd(0x81);//�R���g���X�g�Z�b�g
	lcd_cmd(cont);//�R���g���X�g�l
	lcd_cmd(0x78);//SD��0�ɖ߂�
	lcd_cmd(0x28);//2C=�������@28=�m�[�}��
}

void lcd_ini(){
	lcd_cmd(0x01);//�N���A�[�f�B�X�v���C
	lcd_cmd(0x02);//�J�[�\���������l�ɂ���
	lcd_cmd(0x0c);//�f�B�X�v���C�I���i0x0c,0x0d�Ȃ�`���������j
	lcd_cmd(0x01);//�N���A�[�f�B�X�v���C

	//�R���g���X�g�ݒ�
	so1602awxb_contrast(0xff);
}

void lcd_start(int line){
	if(line == 1){
		lcd_cmd(0x80);//1�s�ڎw��
	}else if(line == 2){
		lcd_cmd(0xa0);//2�s�ڎw��
	}
}

void lcd_char(char s){

	//lcd_cmd(0x01);//�N���A�[�f�B�X�v���C

	//�X�^�[�g�R���f�B�V�����𔭍s����
	i2c_start();
	//LCD�̃A�h���X
	i2c_write(0x78);
	i2c_write(0x40);
	i2c_write(s) ;              // data byte �̑��M(�A�����M)

	i2c_stop();
}

