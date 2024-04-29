/*****************************************************************************************
温度・湿度センサー(AM2321)用ライブラリ 2015/2/10作成

□提供元
ZEATEC co.,ltd. 

□ご利用について
転載・無断使用可です。このライブラリを使用した上での不具合等に関しては、いかなる内容におい
ても一切の責任を追わないものとします。

□使用方法
read_am2321()を1.5msec以上のタイマーに入れ、２回に１回の割合でデータを取得する。
I2Cはハードウェア制御を推奨とし、宣言例は下記のとおり。

#use i2c(MASTER, SDA=PIN_C4, SCL=PIN_C3,SLOW,FORCE_HW)//FAST=400kbps SLOW=100kbps
ピンアサインはPIC16F1947で使用した場合で、スピードは100kbpsとする。400kbpsは使用不可。

温度はtemperature、湿度はhumidityに10倍の値で格納される。表記する際には1/10で表記すること。

*****************************************************************************************/
int i2c_buff_am2321[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
int am2321_status = 0;//0.待機　1.機能コード送信　2.応答値読み込み
long temperature = 0;//温度
long humidity = 0;//湿度
int am2321_read_one = 0;

void am2321_read();

//センサーからデータ読み込み
void am2321_read(){
	if(am2321_status == 1){
		am2321_status = 2;

		//スリープから起こす
		i2c_start();
		i2c_write(0xb8);
		//delay_us(800);//ハードウェアI2Cの場合は不要
		i2c_stop();

		//温度と湿度を読み込みコマンド送信
		i2c_start();
		i2c_write(0xb8);//書込みアドレス送信
		i2c_write(0x03);//機能コード送信
		i2c_write(0x00);//読み込みレジスタの先頭アドレス
		i2c_write(0x04);//読み込みレジスタ数
		i2c_stop();
	}else if(am2321_status == 2){
		am2321_status = 1;
		//機能コード送信されてから1.5msec以上経過していれば読み込み

		//読み込み開始
		i2c_start();
		i2c_write(0xb9);//読み込みアドレス送信
		//delay_us(30);//ハードウェアI2Cの場合は不要
		i2c_buff_am2321[0] = i2c_read(); //機能コードの返し
		i2c_buff_am2321[1] = i2c_read(); //返信してくるレジスタ数

		i2c_buff_am2321[2] = i2c_read(); //湿度の上位バイト
		i2c_buff_am2321[3] = i2c_read(); //湿度の下位バイト
		humidity = i2c_buff_am2321[2] << 8;//湿度
		humidity = humidity + i2c_buff_am2321[3];//湿度

		i2c_buff_am2321[4] = i2c_read(); //温度の上位バイト
		i2c_buff_am2321[5] = i2c_read(); //温度の下位バイト
		temperature = i2c_buff_am2321[4] << 8;//温度
		temperature = temperature + i2c_buff_am2321[5];//温度

		i2c_buff_am2321[6] = i2c_read(); //巡回冗長符号　下位バイト
		i2c_buff_am2321[7] = i2c_read(0); //巡回冗長符号　上位バイト
		i2c_stop();
	}else{
		am2321_status = 1;
	}
}