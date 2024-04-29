//EEPROM関係
int chipadrs = 0xa0;//チップアドレス
long address;//アドレス

void eeprom_write(int chip, long adrs, char wdata);
int eeprom_read(int chip, long adrs);


//外付けEEPROM書込みサブ関数
void eeprom_write(int chip, long adrs, int wdata){
   i2c_start();						//I2Cスタート
   i2c_write(chip);					//書き込みモード送信
   i2c_write((adrs >> 8) & 0x7F);	//上位アドレス送信
   i2c_write(adrs);					//下位アドレス送信
   i2c_write(wdata);				//書き込みデータ送信
   i2c_stop();						//I2Cストップ
   delay_ms(5);						//書き込み待ち
}
//外付けEEPROM読出しサブ関数
int eeprom_read(int chip, long adrs){
   int value;

   i2c_start();						//I2Cスタート
   i2c_write(chip);					//書き込みモード
   i2c_write((adrs >> 8) & 0x7F);	//上位アドレス
   i2c_write(adrs);					//下位アドレス
   i2c_start();						//リピートスタート
   i2c_write(chip | 0x01);			//読み込みモード
   value = i2c_read(0);				//データ読み込み(ACK返送つき）
   i2c_stop();						//I2Cストップ
   return(value);					//データを返す
}

