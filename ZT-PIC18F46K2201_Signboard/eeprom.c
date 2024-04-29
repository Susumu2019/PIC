//EEPROM�֌W
int chipadrs = 0xa0;//�`�b�v�A�h���X
long address;//�A�h���X

void eeprom_write(int chip, long adrs, char wdata);
int eeprom_read(int chip, long adrs);


//�O�t��EEPROM�����݃T�u�֐�
void eeprom_write(int chip, long adrs, int wdata){
   i2c_start();						//I2C�X�^�[�g
   i2c_write(chip);					//�������݃��[�h���M
   i2c_write((adrs >> 8) & 0x7F);	//��ʃA�h���X���M
   i2c_write(adrs);					//���ʃA�h���X���M
   i2c_write(wdata);				//�������݃f�[�^���M
   i2c_stop();						//I2C�X�g�b�v
   delay_ms(5);						//�������ݑ҂�
}
//�O�t��EEPROM�Ǐo���T�u�֐�
int eeprom_read(int chip, long adrs){
   int value;

   i2c_start();						//I2C�X�^�[�g
   i2c_write(chip);					//�������݃��[�h
   i2c_write((adrs >> 8) & 0x7F);	//��ʃA�h���X
   i2c_write(adrs);					//���ʃA�h���X
   i2c_start();						//���s�[�g�X�^�[�g
   i2c_write(chip | 0x01);			//�ǂݍ��݃��[�h
   value = i2c_read(0);				//�f�[�^�ǂݍ���(ACK�ԑ����j
   i2c_stop();						//I2C�X�g�b�v
   return(value);					//�f�[�^��Ԃ�
}

