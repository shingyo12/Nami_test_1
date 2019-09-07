#include "define_mouse.h"
#include "interrupt.h"
#include "switch.h"
#include "sci.h"
#include "mode.h"
#include "camera.h"
#include "run.h"
#include "buzzer.h"
//#include "drive.h"

static unsigned short mtu3_cnt = 0;
static unsigned int timer_ms = 0;
static unsigned int timer_sec = 0;



void mtu3_tgra(){
	static short int j=0;
	long int k=0;

	if(SW_MODE==SW_ON){
		BUZZ_OUT = 1;
		LED_1 = CHIP_LED_ON;
		//LED_2 = CHIP_LED_ON;
		wait_sw_off();
		j=j+1;
		if(j>9)j=0;
		BUZZ_OUT = 0;
		LED_1 = CHIP_LED_OFF;
		//LED_2 = CHIP_LED_OFF;
	}
	mode(j);
	seven_seg(j);
	if(SW_RESET==SW_ON){
		BUZZ_OUT = 1;
		wait_sw_off();
		BUZZ_OUT = 0;
	}
	//line_scan();
	//set_vel();
	if(SW_START==SW_ON){
		BUZZ_OUT = 1;
		wait_sw_off();
		BUZZ_OUT = 0;
		//if(move > 0){
			//move=0;
			//LED_2 = CHIP_LED_ON;
		//}else{
			move=0;
			LED_2 = CHIP_LED_OFF;
			//BUZZ_OUT = 0;
			for(k=0;k<6000000;k++);
			//BUZZ_OUT = 1;
			LED_2 = CHIP_LED_ON;
			for(k=0;k<6000000;k++);
			LED_2 = CHIP_LED_OFF;
			//BUZZ_OUT = 0;
			for(k=0;k<6000000;k++);
			//BUZZ_OUT = 1;
			LED_2 = CHIP_LED_ON;
			for(k=0;k<6000000;k++);
			LED_2 = CHIP_LED_OFF;
			//BUZZ_OUT = 0;
			move=1;
			LED_2 = CHIP_LED_ON;
			
		//}
		distance = 0;
	}
	l_encoder();
	r_encoder();
}

void mtu3_tgrb(){
	/*static short int j=0;
	long int k=0;

	if(SW_MODE==SW_ON){
		LED_1 = CHIP_LED_ON;
		LED_2 = CHIP_LED_ON;
		wait_sw_off();
		j=j+1;
		if(j>9)j=0;
		LED_1 = CHIP_LED_OFF;
		LED_2 = CHIP_LED_OFF;
	}
	mode(j);
	seven_seg(j);
	line_scan();
	//set_vel();
	if(SW_START==SW_ON){
		wait_sw_off();
		//if(move > 0){
			//move=0;
			//LED_2 = CHIP_LED_ON;
		//}else{
			move=0;
			LED_2 = CHIP_LED_OFF;
			for(k=0;k<6000000;k++);
			LED_2 = CHIP_LED_ON;
			for(k=0;k<6000000;k++);
			LED_2 = CHIP_LED_OFF;
			for(k=0;k<6000000;k++);
			LED_2 = CHIP_LED_ON;
			for(k=0;k<6000000;k++);
			LED_2 = CHIP_LED_OFF;
			move=1;
			LED_2 = CHIP_LED_ON;
			
		//}
		distance = 0;
	}*/
	
	/*if(clk >= 128){
		rokou++;
		if(rokou>5){
			rokou=0;
			clk=0;
			CAM_SI=1;
			CAM_CLK=1;
			CAM_SI=0;
			CAM_CLK=0;
			/*for(i=0;i<128;i++){
				//sci_printf("%d ",cam[i]);
					if(cam[i]>100){
						sci_printf("|");
					}else{
						sci_printf("_");
					}
			}
			sci_printf("\r\n");
			
			line_scan();
			sci_printf("interruput move:%d \r\n",move);
		}
	}else{
		CAM_CLK=1;
		S12AD.ADANSA.WORD = 0x01;			//AN000‚ð‘I‘ð
		while(S12AD.ADCSR.BIT.ADST == 1);		//ADST‚ª0‚É‚È‚é‚Ü‚Å‘Ò‚Â
		S12AD.ADCSR.BIT.ADST = 1;			//AD•ÏŠ·ŠJŽn
		cam[clk]=CAM_AOO;
		while(S12AD.ADCSR.BIT.ADST == 1);		//ADST‚ª0‚É‚È‚é‚Ü‚Å‘Ò‚Â
		//sci_printf("%d ", S12AD.ADDR0);
		CAM_CLK=0;
		clk++;
	}  */
	//encoder();
}

void mtu4_tgrb(){
	//set_vel();
	/*static int mtu4_temp=0;
	mtu4_temp ++;
	if(mtu4_temp >= 100000){
		LED_1 =~ LED_1;
		mtu4_temp = 0;
	}*/
}
void increment_timer_ms(){
	timer_ms ++;
	if(timer_ms >= 1000){
		increment_timer_sec();
		timer_ms = 0;
	}
	
}

void increment_timer_sec(){
	timer_sec++;
}

