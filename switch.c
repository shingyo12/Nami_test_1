#include "define_mouse.h"
#include "switch.h"

//wait�֐�
//ms	�҂���[ms]
void wait_ms(long ms)
{
	unsigned volatile long count;
	const short clk_num = 1000;//1ms�ɂ�����N���b�N��
	const short one_time = 0xffff / clk_num;

	while(ms > 0){
		if(ms > one_time){
			count = one_time;
		}
		else{
			count = ms;
		}
		ms -= one_time;
		count = count * clk_num - 1;
		CMT0.CMCNT = 0;
		CMT.CMSTR0.BIT.STR0 = 1;
		while(CMT0.CMCNT < count);
		CMT.CMSTR0.BIT.STR0 = 0;
	}
}
//�`���^�����O�h�~����
//�^�N�g�X�C�b�`���������܂ő҂�
void wait_sw_off(void)
{
	static short int i=0;
	for(i;i<20000;i++);	//50ms�҂�
	//�S�ẴX�C�b�`��OFF�ɂȂ�܂Ń��[�v���đ҂�
	while(1){
		for(i;i<1000;i++);
		if((SW_MODE==SW_OFF) && (SW_START==SW_OFF))break;
	}
	for(i;i<20000;i++);	//50ms�҂�
}

//return�l	�^�N�g�X�C�b�`�̏�� ������Ă�����1�C������ĂȂ�������0
/*char get_sw_state(char sw)
{
	switch(sw){
	case SW_START:return (SW_START == SW_ON);
	case SW_MODE :return (SW_MODE == SW_ON);
	}
	return 0;
}*/

//�^�N�g�X�C�b�`(EXEC)���������܂ő҂�
void wait_sw_on(void)
{

}