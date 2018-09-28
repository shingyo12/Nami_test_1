#include "buzzer.h"
#include "iodefine.h"

#define BUZ_CLOCK	(32 * 1000000 / 64)	//ブザー用タイマの周波数[Hz]
#define BUZ_TGR		MTU0.TGRB
#define BUZ_START	MTU.TSTR.BIT.CST0
#define BUZ_TCNT	MTU0.TCNT
#define BUZ_TIOR	MTU0.TIORH.BIT.IOB

volatile short buzzer_count = -1;	//ブザーを鳴らす時間のカウント用
//音階周波数
const short scale[17] = {444, 494, 523, 587, 659, 698, 784, 880, 988, 1047, 1174, 1318, 1396, 1568, 1760, 1976, 2094};


//ブザーを鳴らし始める関数
//frequency		周波数[Hz]
//time			鳴らす時間[ms]
//時間が過ぎたら自動的に音が止む
//timeに0を指定すると鳴り続ける
void start_buzzer(short frequency, short time)
{
	buzzer_count = time - 1;
	BUZ_TGR = BUZ_CLOCK / 2 / frequency;
	BUZ_START = 0;
	BUZ_TIOR = 3;
	BUZ_START = 1;
}

//ブザーが鳴っている間，wait関数で待つ
//frequency		周波数[Hz]
//time			待つ時間[ms]
void sound_buzzer(short frequency, short time)
{
	start_buzzer(frequency, 0);
	wait_ms(time);
	stop_buzzer();
}

//ブザー割り込み関数
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
	BUZ_TIOR = 0;	//端子出力を禁止
	buzzer_count = -1;
}