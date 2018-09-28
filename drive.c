#include "drive.h"
#include "define_mouse.h"

int r_enc_dif = 0;
int l_enc_dif = 0;
float distance = 0;

void direction_r_mot(char direction){
	if(direction == MOT_FORWARD){
		MOT_R_IN1 = 1;
		MOT_R_IN2 = 0;
	}
	else if(direction == MOT_BACKWARD){
		MOT_R_IN1 = 0;
		MOT_R_IN2 = 1;
		
	}
}

void direction_l_mot(char direction){

	if(direction == MOT_FORWARD){
		MOT_L_IN1 = 1;
		MOT_L_IN2 = 0;
	}
	else if(direction == MOT_BACKWARD){
		MOT_L_IN1 = 1;
		MOT_L_IN2 = 0;
	}
}

//angular velocity of wheels
void set_vel(float vl,float om){
	float t_wr,t_wl;
	//float den=pow(WB,2)+RR*RL;
	t_wr=(WB*vl+2*om)/2*RR;
	t_wl=(WB*vl-2*om)/2*RL;
	servo_l_mot(t_wl);
	servo_r_mot(t_wr);
	//sci_printf("set_vel t_wl:%f t_er:%f \r\n",t_wl,t_wr);
}

void servo_l_mot(float wl_t){
	static short int out_pwm;
	float tmp_wl_t;
	MOT_L_IN1 = 1;
	MOT_L_IN2 = 0;
	tmp_wl_t = (3.6 * PI/180) * l_enc_dif * GEAR;
	out_pwm = wl_t * KP + (wl_t - tmp_wl_t)*KD;
	if(out_pwm>320)out_pwm = 400;
	if(out_pwm<0)out_pwm = 0;
	MTU4.TGRD = 320 - out_pwm;
	tmp_wl_t = wl_t;
	//sci_printf("servo_l_mot wl_t:%f L out_pwm:%d \r\n",wl_t,out_pwm);
}

void servo_r_mot(float wr_t){
	int out_pwm;
	float tmp_wr_t;
	MOT_R_IN1 = 1;
	MOT_R_IN2 = 0;
	tmp_wr_t = (3.6 * DEG2RAD) * r_enc_dif * GEAR;
	out_pwm = wr_t * KP + (wr_t - tmp_wr_t)*KD;
	if(out_pwm>320)out_pwm = 400;
	if(out_pwm<0)out_pwm = 0;
	MTU4.TGRB = 320 - out_pwm;
	tmp_wr_t = wr_t;
	//sci_printf("servo_r_mot wr_t:%f R out_pwm:%d \r\n",wr_t,out_pwm);
}

void encoder(){
	static int d_r_enc=0, d_l_enc=0, d_r_enc_dif=0, d_l_enc_dif=0;
	float tire;
	r_enc_dif = R_ENC - d_r_enc;
	l_enc_dif = L_ENC - d_l_enc;
	if(abs(r_enc_dif)>1000)r_enc_dif = d_r_enc_dif;
	if(abs(l_enc_dif)>1000)l_enc_dif = d_l_enc_dif;
	
	tire = 3.6 * DEG2RAD; 
	distance = (r_enc_dif + l_enc_dif)*0.5 * tire * GEAR;
	
	d_r_enc = R_ENC;
	d_l_enc = L_ENC;
	d_r_enc_dif = r_enc_dif;
	d_l_enc_dif = l_enc_dif;
	sci_printf("encoder distance:%f l_enc_dif:%d r_enc_dif:%d \r\n",distance,l_enc_dif,r_enc_dif);
}
