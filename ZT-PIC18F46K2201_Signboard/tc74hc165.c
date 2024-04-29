/*****************************************************************************************
74HC165�p���C�u���� 2015/4/10�쐬

���񋟌�
ZEATEC co.,ltd. 

�������p�ɂ���
�]�ځE���f�g�p�ł��B���̃��C�u�������g�p������ł̕s����Ɋւ��ẮA�����Ȃ���e�ɂ���
�Ă���؂̐ӔC��ǂ�Ȃ����̂Ƃ��܂��B

���g�p���@
#include "tc74hc165.c"���w�b�_�[�ɒǉ����Ă��������B
�擾�������ϐ���p�ӂ���test_val = tc74hc595();�Ƃ����悤�Ɏg���܂��B

*****************************************************************************************/
//�V���A���p�������ϊ�
long tc74hc165(){
	//PIN_E3	QH �V���A���f�[�^
	//PIN_E4	CK �N���b�N
	//PIN_E5	S/L �f�[�^���M�J�n�w��

	int tmp1 = 0;
	long tmp_ans = 0;

	//S/L
	output_bit(PIN_E5,0);
	output_bit(PIN_E5,1);

	for(tmp1=0;tmp1<16;tmp1++){
		//CK
		output_bit(PIN_E4,0);
		
		if(input(PIN_E3) == 0){
			bit_set(tmp_ans,15-tmp1);
		}else{
			bit_clear(tmp_ans,15-tmp1);
		}

		//CK
		output_bit(PIN_E4,1);

	}

	return tmp_ans;
}

