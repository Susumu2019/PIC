//
// I2C RTC EPSON RTC-8564 functions
//このライブラリはBy H.machida@MNCT-S 2003/10/9のファイルから修正を追加しています。

// global variables

int year,month,week,day,hour,min,sec;                // 現時刻
char h_year,h_month,h_week,h_day,h_hour,h_min,h_sec; // 現時刻文字2桁め
char l_year,l_month,l_week,l_day,l_hour,l_min,l_sec; // 現時刻文字1桁め

// 【<1>日付時刻設定】
void rtc_date_set()
{
    i2c_start();
    i2c_write(0xa2); // 書き込みモード
    i2c_write(0x02); // 秒のアドレス
    i2c_write(sec);  // 秒の値 0-59
    i2c_write(min);  // 分の値 0-59
    i2c_write(hour); // 時の値 0-23
    i2c_write(day);  // 日の値 1-31
    i2c_write(week); // 曜の値 日月火水木金土 0123456 上位5bitはDon't Car
    i2c_write(month);// 月の値 (C:MSB)1-12   Cは1のとき21世紀
    i2c_write(year); // 年の値 00-99
    i2c_stop();
}

// 【<2>日付時刻読み出し】
void rtc_date_read(){
	i2c_start();
	i2c_write(0xa2); // 書き込みモード
	i2c_write(0x02); // 秒のアドレス
	i2c_start();
	i2c_write(0xa3); // 読み込みモード
	sec=  i2c_read(1) & 0x7f; // 秒の値
	min=  i2c_read(1) & 0x7f; // 分の値
	hour= i2c_read(1) & 0x3f; // 時の値
	day=  i2c_read(1) & 0x3f; // 日の値
	week= i2c_read(1) & 0x7; // 曜の値
	month=i2c_read(1) & 0x1f; // 月の値
	year= i2c_read(0); // 年の値
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

// 【<3>アラームの設定】毎時10分にアラーム出力する。
void rtc_alarm_set(int aweek,int aday,int ahour,int amin)
{
    i2c_start();
    i2c_write(0xa2);  // 書き込みモード
    i2c_write(0x09);  // アラーム分のアドレス
    i2c_write(amin);  // アラーム分の値 0-59 MSBはAE
    i2c_write(ahour); // アラーム時の値 0-23
    i2c_write(aday);  // アラーム日の値 1-31
    i2c_write(aweek); // アラーム曜の値 日月火水木金土 0123456 
    i2c_stop();       //                上位5bitは Don't Care
}

// 【<4>タイマ、周波数の設定】周波数1Hz,タイマ10秒設定
void rtc_timer_set(int tfreq,int tcont,int timer)
{
    i2c_start();
    i2c_write(0xa2);  // 書き込みモード
    i2c_write(0x0D);  // クロック出力周波数のアドレス
    i2c_write(tfreq); // クロック出力周波数の値
    i2c_write(tcont); // タイマ制御値
    i2c_write(timer); // タイマ値
    i2c_stop();
}

