#include "define_mouse.h"
#include "interrupt.h"
#include "switch.h"
#include "sci.h"
#include "calib.h"
#include "run.h"
#include "output.h"
#include "mode.h"

long int goal=0;

void mode(int num){
	switch(num){
		case 0:
			camera_calib2();
			break;
		case 1:
			line_scan2(10);
			break;
		case 2:
			line_scan2(15);
			break;
		case 3:
			line_scan2(20);
			break;
		case 4:
			line_scan2(15);
			goal++;
			if(goal>500000)move=0;
			break;
		case 5:
			line_scan3(15);
			break;
		case 6:
			line_scan3(20);
			break;
		case 7:

			break;
		case 8:

			break;
		case 9:
			sci_printf("enc l:%d r:%d \r\n",l_encoder(),r_encoder());
			break;
	}
}