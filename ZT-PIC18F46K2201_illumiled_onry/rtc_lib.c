//
// I2C RTC EPSON RTC-8564 functions
//���̃��C�u������By H.machida@MNCT-S 2003/10/9�̃t�@�C������C����ǉ����Ă��܂��B

// global variables

int year,month,week,day,hour,min,sec;                // ������
char h_year,h_month,h_week,h_day,h_hour,h_min,h_sec; // ����������2����
char l_year,l_month,l_week,l_day,l_hour,l_min,l_sec; // ����������1����

// �y<1>���t�����ݒ�z
void rtc_date_set()
{
    i2c_start();
    i2c_write(0xa2); // �������݃��[�h
    i2c_write(0x02); // �b�̃A�h���X
    i2c_write(sec);  // �b�̒l 0-59
    i2c_write(min);  // ���̒l 0-59
    i2c_write(hour); // ���̒l 0-23
    i2c_write(day);  // ���̒l 1-31
    i2c_write(week); // �j�̒l �����ΐ��؋��y 0123456 ���5bit��Don't Car
    i2c_write(month);// ���̒l (C:MSB)1-12   C��1�̂Ƃ�21���I
    i2c_write(year); // �N�̒l 00-99
    i2c_stop();
}

// �y<2>���t�����ǂݏo���z
void rtc_date_read(){
	i2c_start();
	i2c_write(0xa2); // �������݃��[�h
	i2c_write(0x02); // �b�̃A�h���X
	i2c_start();
	i2c_write(0xa3); // �ǂݍ��݃��[�h
	sec=  i2c_read(1) & 0x7f; // �b�̒l
	min=  i2c_read(1) & 0x7f; // ���̒l
	hour= i2c_read(1) & 0x3f; // ���̒l
	day=  i2c_read(1) & 0x3f; // ���̒l
	week= i2c_read(1) & 0x7; // �j�̒l
	month=i2c_read(1) & 0x1f; // ���̒l
	year= i2c_read(0); // �N�̒l
	i2c_stop();
	bit_clear(month,7);
	bit_clear(month,6);
	bit_clear(month,5);
	bit_clear(day,7);
	bit_clear(day,6);
	bit_clear(hour,7);
	bit_clear(hour,6);
	bit_clear(min,7);
	bit_clear(sec,7);

	h_sec=  ((  sec>>4)&0x07)|0x30; l_sec=  (  sec&0x0f)|0x30; 
	h_min=  ((  min>>4)&0x07)|0x30; l_min=  (  min&0x0f)|0x30; 
	h_hour= (( hour>>4)&0x03)|0x30; l_hour= ( hour&0x0f)|0x30; 
	h_day=  ((  day>>4)&0x03)|0x30; l_day=  (  day&0x0f)|0x30; 
	h_week=                      0; l_week= ( week&0x0f)|0x30; 
	h_month=((month>>4)&0x01)|0x30; l_month=(month&0x0f)|0x30; 
	h_year= (( year>>4)&0x0f)|0x30; l_year= ( year&0x0f)|0x30; 
}

// �y<3>�A���[���̐ݒ�z����10���ɃA���[���o�͂���B
void rtc_alarm_set(int aweek,int aday,int ahour,int amin)
{
    i2c_start();
    i2c_write(0xa2);  // �������݃��[�h
    i2c_write(0x09);  // �A���[�����̃A�h���X
    i2c_write(amin);  // �A���[�����̒l 0-59 MSB��AE
    i2c_write(ahour); // �A���[�����̒l 0-23
    i2c_write(aday);  // �A���[�����̒l 1-31
    i2c_write(aweek); // �A���[���j�̒l �����ΐ��؋��y 0123456 
    i2c_stop();       //                ���5bit�� Don't Care
}

// �y<4>�^�C�}�A���g���̐ݒ�z���g��1Hz,�^�C�}10�b�ݒ�
void rtc_timer_set(int tfreq,int tcont,int timer)
{
    i2c_start();
    i2c_write(0xa2);  // �������݃��[�h
    i2c_write(0x0D);  // �N���b�N�o�͎��g���̃A�h���X
    i2c_write(tfreq); // �N���b�N�o�͎��g���̒l
    i2c_write(tcont); // �^�C�}����l
    i2c_write(timer); // �^�C�}�l
    i2c_stop();
}

