#include "trace.h"
#include "camera.h"
#include "run.h"

#include "define_mouse.h"
//#include "interrupt.h"
//#include "sensor.h"
//#include "switch.h"
//#include "sci.h"
//#include "mode.h"
//#include "output.h"

int end_cnt = 0;

int get_turn_amount(int tmp_pos, int target_pos){
	int turn_amount;
	
	turn_amount = 22*(target_pos-tmp_pos)/32;
	
	return turn_amount;
}

void general_control(){
	static int progress = 0;
	if(progress == 0){
		if((int)search_run == 1){
			progress = 1;
		}
	}else if(progress == 1){
		if((int)shortcut_run(10, 1) == 1){
			progress = 2;
		}
	}else if(progress == 2){
		if((int)shortcut_run(20, 2) == 1){
			progress = 3;
		}
	}
}

int search_run(int vel){
	static int sccess_flg = 0;
	int turn_amount;
	
	line_scan();
	turn_amount = get_turn_amount(cam_data.line,64);
	servo(vel,turn_amount);
	if(cam_data.mk_r_cnt>2){
		BUZZ_OUT = 0;
		LED_1 = CHIP_LED_OFF;
		LED_2 = CHIP_LED_OFF;
		servo(400,turn_amount);
		end_cnt++;
		if(end_cnt > 4000){
			move=0;
		}
	}
	//turn_amount = get_turn_amount(cam_data.line,64);
	//servo(vel,turn_amount);
	//move=1;
	if(sccess_flg == 1){
		return 1;
	}else{
		return 0;
	}
}

int shortcut_run(int vel_mode, int sc_mode){
	static int sccess_flg = 0;
	int turn_amount;
	
	line_scan();
	turn_amount = (int)get_turn_amount(cam_data.line,64);
	servo(vel_mode,turn_amount);
	
	if(sccess_flg == 1){
		return 1;
	}else{
		return 0;
	}
}