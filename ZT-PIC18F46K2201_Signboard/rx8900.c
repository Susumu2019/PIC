#define RX8900A_ADRS 0x64//0x64(8bit address),0x32(7bit address)

void rtc_date_set()
{
    i2c_start();
    i2c_write(RX8900A_ADRS); // 書き込みモード
    i2c_write(0x00); // 秒のアドレス
    i2c_write(sec);  // 秒の値 0-59
    i2c_write(min);  // 分の値 0-59
    i2c_write(hour); // 時の値 0-23
    i2c_write(week); // 曜の値 日月火水木金土 0123456 上位5bitはDon't Car
    i2c_write(day);  // 日の値 1-31
    i2c_write(month);// 月の値 (C:MSB)1-12   Cは1のとき21世紀
    i2c_write(year); // 年の値 00-99
    i2c_stop();
}

void rtc_date_read(){

	//スタートコンディションを発行する
	i2c_start();
	//LCDのアドレス
	i2c_write(RX8900A_ADRS);
	// data byte の送信
	i2c_write(0x00);//秒のアドレス
	//読み込み
	i2c_start();
	i2c_write(RX8900A_ADRS+1);
	sec = i2c_read(1);					//0x00
	min = i2c_read(1);					//0x01
	hour = i2c_read(1);					//0x02
	WEEK = i2c_read(1);					//0x03
	day = i2c_read(1);					//0x04
	month = i2c_read(1);				//0x05
	year = i2c_read(0);					//0x06
	// ストップコンディションを発行する
	i2c_stop();

}

//int bcd2dec(int bcd){
//	return 0;
//}
