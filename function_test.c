#include "define_mouse.h"
#include "function_test.h"
#include "sci.h"
#include "drive.h"

void seven_seg(int n){
	if(n>10)n=n%10;
	switch(n){
		case 0:
			LED_7SEG_1 = CHIP_LED_OFF;
			LED_7SEG_2 = CHIP_LED_ON;
			LED_7SEG_3 = CHIP_LED_ON;
			LED_7SEG_4 = CHIP_LED_ON;
			LED_7SEG_5 = CHIP_LED_ON;
			LED_7SEG_6 = CHIP_LED_ON;
			LED_7SEG_7 = CHIP_LED_ON;
			break;
		case 1:
			LED_7SEG_1 = CHIP_LED_OFF;
			LED_7SEG_2 = CHIP_LED_OFF;
			LED_7SEG_3 = CHIP_LED_OFF;
			LED_7SEG_4 = CHIP_LED_OFF;
			LED_7SEG_5 = CHIP_LED_ON;
			LED_7SEG_6 = CHIP_LED_ON;
			LED_7SEG_7 = CHIP_LED_OFF;
			break;
		case 2:
			LED_7SEG_1 = CHIP_LED_ON;
			LED_7SEG_2 = CHIP_LED_OFF;
			LED_7SEG_3 = CHIP_LED_ON;
			LED_7SEG_4 = CHIP_LED_ON;
			LED_7SEG_5 = CHIP_LED_OFF;
			LED_7SEG_6 = CHIP_LED_ON;
			LED_7SEG_7 = CHIP_LED_ON;
			break;
		case 3:
			LED_7SEG_1 = CHIP_LED_ON;
			LED_7SEG_2 = CHIP_LED_OFF;
			LED_7SEG_3 = CHIP_LED_OFF;
			LED_7SEG_4 = CHIP_LED_ON;
			LED_7SEG_5 = CHIP_LED_ON;
			LED_7SEG_6 = CHIP_LED_ON;
			LED_7SEG_7 = CHIP_LED_ON;
			break;
		case 4:
			LED_7SEG_1 = CHIP_LED_ON;
			LED_7SEG_2 = CHIP_LED_ON;
			LED_7SEG_3 = CHIP_LED_OFF;
			LED_7SEG_4 = CHIP_LED_OFF;
			LED_7SEG_5 = CHIP_LED_ON;
			LED_7SEG_6 = CHIP_LED_ON;
			LED_7SEG_7 = CHIP_LED_OFF;
			break;
		case 5:
			LED_7SEG_1 = CHIP_LED_ON;
			LED_7SEG_2 = CHIP_LED_ON;
			LED_7SEG_3 = CHIP_LED_OFF;
			LED_7SEG_4 = CHIP_LED_ON;
			LED_7SEG_5 = CHIP_LED_ON;
			LED_7SEG_6 = CHIP_LED_OFF;
			LED_7SEG_7 = CHIP_LED_ON;
			break;
		case 6:
			LED_7SEG_1 = CHIP_LED_ON;
			LED_7SEG_2 = CHIP_LED_ON;
			LED_7SEG_3 = CHIP_LED_ON;
			LED_7SEG_4 = CHIP_LED_ON;
			LED_7SEG_5 = CHIP_LED_ON;
			LED_7SEG_6 = CHIP_LED_OFF;
			LED_7SEG_7 = CHIP_LED_ON;
			break;
		case 7:
			LED_7SEG_1 = CHIP_LED_OFF;
			LED_7SEG_2 = CHIP_LED_OFF;
			LED_7SEG_3 = CHIP_LED_OFF;
			LED_7SEG_4 = CHIP_LED_OFF;
			LED_7SEG_5 = CHIP_LED_ON;
			LED_7SEG_6 = CHIP_LED_ON;
			LED_7SEG_7 = CHIP_LED_ON;
			break;
		case 8:
			LED_7SEG_1 = CHIP_LED_ON;
			LED_7SEG_2 = CHIP_LED_ON;
			LED_7SEG_3 = CHIP_LED_ON;
			LED_7SEG_4 = CHIP_LED_ON;
			LED_7SEG_5 = CHIP_LED_ON;
			LED_7SEG_6 = CHIP_LED_ON;
			LED_7SEG_7 = CHIP_LED_ON;
			break;
		case 9:
			LED_7SEG_1 = CHIP_LED_ON;
			LED_7SEG_2 = CHIP_LED_ON;
			LED_7SEG_3 = CHIP_LED_OFF;
			LED_7SEG_4 = CHIP_LED_OFF;
			LED_7SEG_5 = CHIP_LED_ON;
			LED_7SEG_6 = CHIP_LED_ON;
			LED_7SEG_7 = CHIP_LED_ON;
			break;
	}
}

void flash_led(void){
	short i=0;
	while(1){	
		LED_1 = CHIP_LED_OFF;
		LED_2 = CHIP_LED_ON;	//LED点灯
		for(i=0; i<10000; i++);	//待つ
		LED_2 = CHIP_LED_OFF;	//LED消灯
		LED_1 = CHIP_LED_ON;
		for(i=0;i<10000;i++);	//待つ
	}
}

void sci_put_test(){
	short i=0;
	while(1){
		sci_put_1byte(0x88);
		for(i=0;i<10000;i++);	//待つ
	}
	
}

void sci_put_string_test(){
	volatile char cnt;
	while(1){
		cnt ++;
		sci_printf("count = %d \r\n",cnt);
	}
}

void adc_test1(void){
	int temp_ADdata = 0;
	
	//左前センサ
	S12AD.ADANSA.WORD = 0x01;		//AN000を選択
	S12AD.ADCSR.BIT.ADST = 1;		//AD変換開始
	while(S12AD.ADCSR.BIT.ADST == 1);	//ADSTが0になるまで待つ
	temp_ADdata = S12AD.ADDR0;
	sci_printf("AN000 = %d \r\n",temp_ADdata);
}

void encoder_test1(void){
	sci_printf("R_ENC = %d L_ENC = %d \r\n",MTU1.TCNT,MTU2.TCNT);
}
