/*****************************************************************************************
���x�E���x�Z���T�[(AM2321)�p���C�u���� 2015/2/10�쐬

���񋟌�
ZEATEC co.,ltd. 

�������p�ɂ���
�]�ځE���f�g�p�ł��B���̃��C�u�������g�p������ł̕s����Ɋւ��ẮA�����Ȃ���e�ɂ���
�Ă���؂̐ӔC��ǂ�Ȃ����̂Ƃ��܂��B

���g�p���@
read_am2321()��1.5msec�ȏ�̃^�C�}�[�ɓ���A�Q��ɂP��̊����Ńf�[�^���擾����B
I2C�̓n�[�h�E�F�A����𐄏��Ƃ��A�錾��͉��L�̂Ƃ���B

#use i2c(MASTER, SDA=PIN_C4, SCL=PIN_C3,SLOW,FORCE_HW)//FAST=400kbps SLOW=100kbps
�s���A�T�C����PIC16F1947�Ŏg�p�����ꍇ�ŁA�X�s�[�h��100kbps�Ƃ���B400kbps�͎g�p�s�B

���x��temperature�A���x��humidity��10�{�̒l�Ŋi�[�����B�\�L����ۂɂ�1/10�ŕ\�L���邱�ƁB

*****************************************************************************************/
int i2c_buff_am2321[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
int am2321_status = 0;//0.�ҋ@�@1.�@�\�R�[�h���M�@2.�����l�ǂݍ���
long temperature = 0;//���x
long humidity = 0;//���x
int am2321_read_one = 0;

void am2321_read();

//�Z���T�[����f�[�^�ǂݍ���
void am2321_read(){
	if(am2321_status == 1){
		am2321_status = 2;

		//�X���[�v����N����
		i2c_start();
		i2c_write(0xb8);
		//delay_us(800);//�n�[�h�E�F�AI2C�̏ꍇ�͕s�v
		i2c_stop();

		//���x�Ǝ��x��ǂݍ��݃R�}���h���M
		i2c_start();
		i2c_write(0xb8);//�����݃A�h���X���M
		i2c_write(0x03);//�@�\�R�[�h���M
		i2c_write(0x00);//�ǂݍ��݃��W�X�^�̐擪�A�h���X
		i2c_write(0x04);//�ǂݍ��݃��W�X�^��
		i2c_stop();
	}else if(am2321_status == 2){
		am2321_status = 1;
		//�@�\�R�[�h���M����Ă���1.5msec�ȏ�o�߂��Ă���Γǂݍ���

		//�ǂݍ��݊J�n
		i2c_start();
		i2c_write(0xb9);//�ǂݍ��݃A�h���X���M
		//delay_us(30);//�n�[�h�E�F�AI2C�̏ꍇ�͕s�v
		i2c_buff_am2321[0] = i2c_read(); //�@�\�R�[�h�̕Ԃ�
		i2c_buff_am2321[1] = i2c_read(); //�ԐM���Ă��郌�W�X�^��

		i2c_buff_am2321[2] = i2c_read(); //���x�̏�ʃo�C�g
		i2c_buff_am2321[3] = i2c_read(); //���x�̉��ʃo�C�g
		humidity = i2c_buff_am2321[2] << 8;//���x
		humidity = humidity + i2c_buff_am2321[3];//���x

		i2c_buff_am2321[4] = i2c_read(); //���x�̏�ʃo�C�g
		i2c_buff_am2321[5] = i2c_read(); //���x�̉��ʃo�C�g
		temperature = i2c_buff_am2321[4] << 8;//���x
		temperature = temperature + i2c_buff_am2321[5];//���x

		i2c_buff_am2321[6] = i2c_read(); //����璷�����@���ʃo�C�g
		i2c_buff_am2321[7] = i2c_read(0); //����璷�����@��ʃo�C�g
		i2c_stop();
	}else{
		am2321_status = 1;
	}
}