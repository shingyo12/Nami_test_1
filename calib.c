#include "define_mouse.h"
#include "interrupt.h"
#include "sci.h"
#include "mode.h"
#include "calib.h"
#include "run.h"
#include "camera.h"
#include "drive.h"

float sum_cam[128];

void init_calib(void){
	int j;
	for(j=0;j<128;j++){
		sum_cam[j]=0;
	}
}

void calib(){
	static int calib_cnt=0,j;

	//set_vel(0,0);
	
	for(j=0;j<128;j++){
		sum_cam[j] += cam[j];
	}
	calib_cnt++;
	
	if(distance>100){
		//set_vel(0,0);
		move=0;
		for(j=0;j<128;j++){
			ref_cam[j] = sum_cam[j]/calib_cnt;
		}
		sci_printf("calib ");
		for(j=0;j<128;j++){
			ref_cam[j] = sum_cam[j]/calib_cnt;
			sci_printf(" %f",ref_cam[j]);
		}
		LED_2 = CHIP_LED_ON;
		sci_printf("\r\n");
	}
}