#include "define_mouse.h"
#include "switch.h"

//wait関数
//ms	待つ時間[ms]
void wait_ms(long ms)
{
	unsigned volatile long count;
	const short clk_num = 1000;//1msにかかるクロック数
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
//チャタリング防止処理
//タクトスイッチが離されるまで待つ
void wait_sw_off(void)
{
	static short int i=0;
	for(i;i<20000;i++);	//50ms待つ
	//全てのスイッチがOFFになるまでループして待つ
	while(1){
		for(i;i<1000;i++);
		if((SW_MODE==SW_OFF) && (SW_START==SW_OFF))break;
	}
	for(i;i<20000;i++);	//50ms待つ
}

//return値	タクトスイッチの状態 押されていたら1，押されてなかったら0
/*char get_sw_state(char sw)
{
	switch(sw){
	case SW_START:return (SW_START == SW_ON);
	case SW_MODE :return (SW_MODE == SW_ON);
	}
	return 0;
}*/

//タクトスイッチ(EXEC)が押されるまで待つ
void wait_sw_on(void)
{

}