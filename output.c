#include "define_mouse.h"
#include "interrupt.h"
#include "sensor.h"
#include "switch.h"
#include "sci.h"
#include "mode.h"
#include "output.h"
#include "camera.h"

void output_data(){
	short int j;
	for(j=0;j<128;++j){
		sci_printf("%d ",cam[j]);
		/*
		if(cam_arr[i]>ref_cam[i]){
			sci_printf("|");
		}else{
			sci_printf("_");
		}*/
	}
	sci_printf("\r\n");
}
