#include "define_mouse.h"
#include "interrupt.h"
#include "switch.h"
#include "sci.h"
#include "calib.h"
#include "run.h"
#include "output.h"
#include "mode.h"
#include "trace.h"

long int goal=0;

void mode(int num){
	switch(num){
		case 0:
			camera_calib();
			break;
		case 1:
			//general_control();
			line_scan();
			break;
		case 2:
			search_run();
			//servo(1200, -4);
			break;
		case 3:
			//shortcut_run(20,1);
			servo(600, -6);
			break;
		case 4:
			//shortcut_run(40,1);
			servo(1200, -8);
			break;
		case 5:
			//shortcut_run(50,2);
			servo(1200, -10);
			break;
		case 6:
			servo(1200, -12);
			break;
		case 7:
			servo(1200, -14);
			break;
		case 8:
			servo(1200, -18);
			break;
		case 9:
			servo(1200, -22);
			//sci_printf("l:%d r:%d \r\n",L_ENC,R_ENC);
			break;
	}
}