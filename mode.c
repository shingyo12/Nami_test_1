#include "define_mouse.h"
#include "interrupt.h"
#include "switch.h"
#include "sci.h"
#include "calib.h"
#include "run.h"
#include "output.h"
#include "mode.h"

void mode(int num){
	switch(num){
		case 0:
			calib();
			break;
		case 1:
			memorize();
			break;
		case 2:
			mem_run(0.4);
			break;
		case 3:
			mem_run(0.7);
			break;
		case 4:
			mem_run(1);
			break;
		case 5:
			run(0.5);
			break;
		case 6:
			run(1);
			break;
		case 7:
			run_mcr(0.5);
			break;
		case 8:
			run_mcr(1);
			break;
		case 9:
			output_data();
			break;
	}
}