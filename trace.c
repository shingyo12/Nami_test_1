#include "trace.h"
#include "camera.h"
#include "run.h"

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

int search_run(){
	static int sccess_flg = 0;
	int turn_amount;
	
	line_scan();
	turn_amount = get_turn_amount(cam_data.line,64);
	servo(800,turn_amount);
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