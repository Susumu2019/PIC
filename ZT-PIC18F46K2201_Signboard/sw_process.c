/*****************************************************************************************
�X�C�b�`�p���C�u���� 2015/4/7�쐬

���񋟌�
ZEATEC co.,ltd. 

�������p�ɂ���
�]�ځE���f�g�p�ł��B���̃��C�u�������g�p������ł̕s����Ɋւ��ẮA�����Ȃ���e�ɂ���
�Ă���؂̐ӔC��ǂ�Ȃ����̂Ƃ��܂��B

���g�p���@
#include "sw_process.c"���w�b�_�[�ɒǉ�����sw_process()�����C�����[�v�ɓ���Ă��������B���L
�̊֐���ǉ����邱�Ƃł��ꂼ��̓���̎��ɌĂяo����܂��B���L�T���v����RA1�݂̂ł��B�����
�O�̓��͂Ɋւ��Ă�RA1���Q�l�ɒǋL���Ă��������B

sw_a1�ɂ͌��ݒl���擾���Ă��܂��B

//��������
void sw_a1_push(){
	fprintf(c1,"Push ok.\r\n");
}

//��������
void sw_a1_release(){
	fprintf(c1,"Release ok.\r\n");
}

//������
void sw_a1_hold(){
	fprintf(c1,"Hold ok.\r\n");
}

//�_�u���N���b�N
void sw_a1_wpush(){
	fprintf(c1,"Double click ok.\r\n");
}

�܂��A50msec�P�ʂ̊��荞�݂�flicker_hi_one��1�ɂ���t���O�ƒ��������Ƃ̑g�ݍ��킹�ŁA�ݒ�{
�^���𒷉�������Ɛݒ�l�������ŕς��Ƃ������g�������o���܂��B

//��������
void sw_a1_holdon(){
    if(flicker_hi_one == 1){
        flicker_hi_one = 0;
        //�t���b�J�[�n�C�̃^�C�~���O�ł����������Ȃ�
        test_val++;
        
        fprintf(c1,"Holdon ok. test_val=%u\r\n",test_val);
    }
}

*****************************************************************************************/

int sw_a1 = 0;//RA1�̌��ݒl
long holdup_a1 = 30000;//����������l
long holdcount_a1 = 0;//�����Ă���J�E���g�l
long releaseup_a1 = 10000;//�����Ă��鎞�̔���l
long releasecount_a1 = 0;//�����Ă���J�E���g�l
long history_a1 = 0;//�X�C�b�`�̗���

//��������
void sw_a1_push();
//��������
void sw_a1_release();
//���������肵���Ƃ�
void sw_a1_hold();
//��������
void sw_a1_holdon();
//�_�u���N���b�N
void sw_a1_wpush();

//�X�C�b�`����
void sw_process(){

	//********************
	//PIN_A1�p�\�[�X
	//********************
	//���ݒl���擾
	//ON��GND�̏ꍇ
	sw_a1 = !input(PIN_A1);
	//ON��VCC�̏ꍇ
	//sw_a1 = input(PIN_A1);

	//������
	if(sw_a1==1){
		if(holdcount_a1<holdup_a1){
			holdcount_a1++;
		}else if(holdcount_a1==holdup_a1){
			holdcount_a1++;
			//�X�C�b�`�𒷉��������ꍇ
			sw_a1_hold();
		}else{
			//�_�u���N���b�N�p�ϐ����蓖��
			releasecount_a1 = releaseup_a1;
			//�����������܂܂̏ꍇ
			sw_a1_holdon();
		}
	}else{
		holdcount_a1 = 0;
	}

	//��������
	if(history_a1 == 0 && history_a1 != holdcount_a1){
		sw_a1_push();

		//�_�u���N���b�N�p�ϐ����蓖��
		releasecount_a1 = releasecount_a1 + releaseup_a1;
	}

	//��������
	if(history_a1 != 0 && holdcount_a1 == 0){
		sw_a1_release();
	}

	//�_�u���N���b�N�����Ƃ�
	if(releasecount_a1>0){
		releasecount_a1--;
		if(releasecount_a1>releaseup_a1){
			sw_a1_wpush();
			releasecount_a1 = 0;
		}
	}

	history_a1 = holdcount_a1;
	//********************
	//���̂ق��̓��͂͏�L���Q�l�ɒǋL���Ă��������B

}
