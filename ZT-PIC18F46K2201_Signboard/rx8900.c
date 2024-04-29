#define RX8900A_ADRS 0x64//0x64(8bit address),0x32(7bit address)

void rtc_date_set()
{
    i2c_start();
    i2c_write(RX8900A_ADRS); // �������݃��[�h
    i2c_write(0x00); // �b�̃A�h���X
    i2c_write(sec);  // �b�̒l 0-59
    i2c_write(min);  // ���̒l 0-59
    i2c_write(hour); // ���̒l 0-23
    i2c_write(week); // �j�̒l �����ΐ��؋��y 0123456 ���5bit��Don't Car
    i2c_write(day);  // ���̒l 1-31
    i2c_write(month);// ���̒l (C:MSB)1-12   C��1�̂Ƃ�21���I
    i2c_write(year); // �N�̒l 00-99
    i2c_stop();
}

void rtc_date_read(){

	//�X�^�[�g�R���f�B�V�����𔭍s����
	i2c_start();
	//LCD�̃A�h���X
	i2c_write(RX8900A_ADRS);
	// data byte �̑��M
	i2c_write(0x00);//�b�̃A�h���X
	//�ǂݍ���
	i2c_start();
	i2c_write(RX8900A_ADRS+1);
	sec = i2c_read(1);					//0x00
	min = i2c_read(1);					//0x01
	hour = i2c_read(1);					//0x02
	WEEK = i2c_read(1);					//0x03
	day = i2c_read(1);					//0x04
	month = i2c_read(1);				//0x05
	year = i2c_read(0);					//0x06
	// �X�g�b�v�R���f�B�V�����𔭍s����
	i2c_stop();

}

//int bcd2dec(int bcd){
//	return 0;
//}
