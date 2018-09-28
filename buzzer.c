#include "buzzer.h"
#include "iodefine.h"

#define BUZ_CLOCK	(32 * 1000000 / 64)	//�u�U�[�p�^�C�}�̎��g��[Hz]
#define BUZ_TGR		MTU0.TGRB
#define BUZ_START	MTU.TSTR.BIT.CST0
#define BUZ_TCNT	MTU0.TCNT
#define BUZ_TIOR	MTU0.TIORH.BIT.IOB

volatile short buzzer_count = -1;	//�u�U�[��炷���Ԃ̃J�E���g�p
//���K���g��
const short scale[17] = {444, 494, 523, 587, 659, 698, 784, 880, 988, 1047, 1174, 1318, 1396, 1568, 1760, 1976, 2094};


//�u�U�[��炵�n�߂�֐�
//frequency		���g��[Hz]
//time			�炷����[ms]
//���Ԃ��߂����玩���I�ɉ����~��
//time��0���w�肷��Ɩ葱����
void start_buzzer(short frequency, short time)
{
	buzzer_count = time - 1;
	BUZ_TGR = BUZ_CLOCK / 2 / frequency;
	BUZ_START = 0;
	BUZ_TIOR = 3;
	BUZ_START = 1;
}

//�u�U�[�����Ă���ԁCwait�֐��ő҂�
//frequency		���g��[Hz]
//time			�҂���[ms]
void sound_buzzer(short frequency, short time)
{
	start_buzzer(frequency, 0);
	wait_ms(time);
	stop_buzzer();
}

//�u�U�[���荞�݊֐�
void buzzer_interrupt(void)
{
	if(buzzer_count < 0){return;}
	else if(buzzer_count > 0){
		buzzer_count--;
	}
	else{
		stop_buzzer();
	}
}

void stop_buzzer(void)
{
	BUZ_START = 0;
	BUZ_TCNT = 0;
	BUZ_TIOR = 0;	//�[�q�o�͂��֎~
	buzzer_count = -1;
}