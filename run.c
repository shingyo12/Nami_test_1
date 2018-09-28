#include "define_mouse.h"
#include "interrupt.h"
#include "switch.h"
#include "sci.h"
#include "mode.h"
#include "run.h"
#include "drive.h"
#include "camera.h"

struct marker_data{
	float	speed;
	float	odometer;
};

struct marker_data maker[50];

short int move=0;
float last_dis;

void run(float GA){
	float tv=0,om=0;
	static float d_om=0;
	static int r_mk_cnt=0,r_mk_flg=0;
	LED_2 = CHIP_LED_OFF;
	om = (cam_data.sg-64)*P_OM - (d_om - cam_data.sg - 64)*D_OM;
	tv = TV * P_TH - abs(om);
	if(cam_data.mk_r==1 && cam_data.mk_l==0)
		r_mk_flg = 1;
	if(r_mk_cnt>0){
		if(r_mk_cnt>500){
			r_mk_cnt=501;
		}else{
			r_mk_cnt++;
		}
	}
	if(cam_data.mk_r==1 && cam_data.mk_l==0 && r_mk_flg == 1)
		move=0;

	if(move == 0){
		set_vel(0,0);
	}else{
		set_vel(tv*GA,om);
	}
	d_om=om;
	//d_tv=tv;
	//sci_printf("run move:%d tv:%f om:%f \r\n",move,tv,om);
}

void mem_run(float GA){
	float tv=0,om=0;
	static float d_om=0,c_speed;
	static int r_mk_cnt=0,r_mk_flg=0,mk_num=1;
	LED_2 = CHIP_LED_OFF;
	om = (cam_data.sg-64)*P_OM - (d_om - cam_data.sg - 64)*D_OM;
	tv = TV * P_TH - abs(om);
	if(cam_data.mk_r==1 && cam_data.mk_l==0)
		r_mk_flg = 1;
	if(r_mk_cnt>0){
		if(r_mk_cnt>500){
			r_mk_cnt=501;
		}else{
			r_mk_cnt++;
		}
	}
	if((cam_data.mk_r==1 && cam_data.mk_l==0 && r_mk_flg == 1) || distance>last_dis+250)
		move=0;
		
	//remember marker
	if(cam_data.mk_l == 1 && (abs(maker[mk_num].odometer-distance)<30)){
		c_speed=maker[mk_num-1].speed;
	}
	
	if(move ==0){
		set_vel(0,0);
	}else{
		set_vel(tv*GA*c_speed,om);
	}
	sci_printf("mem_run move:%d tv:%f om:%f \r\n",move,tv,om);
}

void run_mcr(float GA){
	float tv=0,om=0;
	static float d_om=0;
	static int r_mk_cnt=0;
	LED_2 = CHIP_LED_OFF;
	om = (cam_data.sg-64)*P_OM - (d_om - cam_data.sg - 64)*D_OM;
	tv = TV * P_TH - abs(om);

	if(r_mk_cnt>0){
		if(r_mk_cnt>500){
			r_mk_cnt=501;
		}else{
			r_mk_cnt++;
		}
	}

	if(move == 0){
		set_vel(0,0);
	}else{
		set_vel(tv*GA,om);
	}
	d_om=om;
}

void memorize(){
	float tv=0,om=0;
	static float d_om=0,sum_om=0,cnt_om=0,ave_om,d_dis;
	static int r_mk_cnt=0,r_mk_flg=0,mk_num=0;
	om = (cam_data.sg-64)*P_OM - (d_om - cam_data.sg - 64)*D_OM;
	tv = TV * P_TH - abs(om);
	if(cam_data.mk_r==1 && cam_data.mk_l==0)
		r_mk_flg = 1;
	if(r_mk_cnt>0){
		if(r_mk_cnt>500){
			r_mk_cnt=501;
		}else{
			r_mk_cnt++;
		}
	}
	if(cam_data.mk_r==1 && cam_data.mk_l==0 && r_mk_flg == 1){
		move=0;
		last_dis=distance;
	}
		
	//memorize marker
	sum_om += abs(om);
	cnt_om++;
	if(cam_data.mk_l == 1 || cam_data.mk_r == 1){
		ave_om = sum_om/cnt_om;
		maker[mk_num].speed=(128-ave_om)/128;
		maker[mk_num].odometer=distance;
		if(d_dis-distance > 30){
			mk_num++;
			sum_om = 0;
			cnt_om = 0;
		}
		d_dis=distance;
	}

	if(move == 0){
		set_vel(0,0);
	}else{
		set_vel(tv*0.25,om);
	}
	d_om=om;
	//d_tv=tv;
	//sci_printf("memorize move:%d tv:%f om:%f\r\n",move,tv,om);
}
