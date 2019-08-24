/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c or Main.cpp                                    */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#include "iodefine.h"
#include "define_mouse.h"
#include "sci.h"
#include "interrupt.h"
#include "switch.h"
#include "buzzer.h"
#include "camera.h"
#include "calib.h"
#include "run.h"
//#include "drive.h"


#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif
//short int j=0,i=0;
//short int tmp_cnt=0,nw_cnt=0;

void main(void)
{
	init_rx220();
	init_calib();
	init_cam();
	init_ENC();
	//flash_led();
	//motor_drive_test();
	//motor_drive_test();
	//start_buzzer(988, 3);
	while(1){
		
		//encoder_test1();
		//motor_drive_test();
		//static short int j=0;
		/*if(SW_MODE==SW_ON){
			j=j+1;
			//LED_1 = CHIP_LED_ON;
			//LED_2 = CHIP_LED_OFF;
			wait_sw_off();
			//j=j+1;
		}
		if(SW_START==SW_ON){
			j=j-1;
			//LED_1 = CHIP_LED_OFF;
			//LED_2 = CHIP_LED_ON;
			wait_sw_off();
			//j=j-1;
		}
		if(j>9)j=0;
		seven_seg(j);
		//sci_printf("test = %d\r\n" , j);
		for(i=0;i<10000;i++);*/		
	}
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
