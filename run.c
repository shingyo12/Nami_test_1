#include "define_mouse.h"
#include "interrupt.h"
#include "switch.h"
#include "sci.h"
#include "mode.h"
#include "run.h"
//#include "drive.h"
#include "camera.h"

//int r_enc_dif = 0;
//int l_enc_dif = 0;
int velocity = 0;
int omega = 0;
int GA;
long int distance = 0;

struct marker_data{
	int	speed;
	int	odometer;
};

struct marker_data maker[50];

short int move=0;
int last_dis;
static float d_dif_l=0,d_dif_r=0;

void servo(int acl, int str){
	float p_acl,p_str;
	static int out_pwm_r,out_pwm_l;
	static int pwm_r,pwm_l;
	int l_enc,r_enc;
	float f_kp,f_kd;
	float dif_r,dif_l;
	float l_str,r_str;
	static int r_mk_cnt=0,enc_cnt=0;
	
	LED_2 = CHIP_LED_OFF;

	if(move==1){
		MOT_STBY = 1;
		LED_2 = CHIP_LED_OFF;
		//if(enc_cnt==0){
			//d_dif_l=d_dif_r=1663-5.4*320*(1-(acl*0.01));
			//enc_cnt=1;
		//}
	}else{
		MOT_STBY = 0;
	}
	
	f_kp=0.1;
	f_kd=0.1;
	
	if(acl>100)acl=100;
	if(acl<-100)acl=-100;
	if(str>100)str=100;
	if(str<-100)str=-100;
	
	p_acl=1-(acl*0.01);
	p_str=str*0.01;
	
	if(str<0){
		r_str=0;
		l_str=-p_str;
	}else if(str>0){
		r_str=p_str;
		l_str=0;
	}else{
		r_str=0;
		l_str=0;
	}
	
	MOT_L_IN1 = 0;
	MOT_L_IN2 = 1;
	out_pwm_l=320*p_acl+(320*(1-p_acl)*l_str);
	
	MOT_R_IN1 = 0;
	MOT_R_IN2 = 1;
	out_pwm_r=320*p_acl+(320*(1-p_acl)*r_str);
	
	l_enc = l_encoder();
	r_enc = r_encoder();
	
	dif_l=(1663-5.4*out_pwm_l)-l_enc;
	dif_r=(1663-5.4*out_pwm_r)-r_enc;
	//pwm_l=out_pwm_l-(int)(f_kp*((1663-5.4*out_pwm_l)-l_enc))+(int)(f_kd*(dif_l-d_dif_l));
	//pwm_r=out_pwm_r-(int)(f_kp*((1663-5.4*out_pwm_r)-r_enc))+(int)(f_kd*(dif_r-d_dif_r));
	pwm_l=out_pwm_l-(int)(f_kp*dif_l);
	pwm_r=out_pwm_r-(int)(f_kp*dif_r);
	
	//sci_printf("L_pwm:%d R_pwm:%d p %d %d d %d %d l_enc:%d r_enc:%d\r\n",pwm_l,pwm_r,dif_l,dif_r,d_dif_l,d_dif_r,l_enc,r_enc);
	
	if(pwm_l>320)pwm_l=320;
	if(pwm_l<0)pwm_l=0;
	if(pwm_r>320)pwm_r=320;
	if(pwm_r<0)pwm_r=0;
	
	MTU4.TGRC=pwm_l;
	MTU4.TGRA=pwm_r;
	
	d_dif_l=dif_l;
	d_dif_r=dif_r;
}

int l_encoder(void){
	static int d_l_enc=0, d_l_enc_dif=0;
	static int l_enc_dif;
	l_enc_dif = L_ENC - d_l_enc;
	if(abs(l_enc_dif)>10000)l_enc_dif = d_l_enc_dif;

	d_l_enc = L_ENC;
	d_l_enc_dif = l_enc_dif;
	
	return l_enc_dif;
}

int r_encoder(void){
	static int d_r_enc=0,d_r_enc_dif=0;
	static int r_enc_dif;
	r_enc_dif = R_ENC - d_r_enc;
	if(abs(r_enc_dif)>10000)r_enc_dif = d_r_enc_dif;
	
	d_r_enc = R_ENC;
	d_r_enc_dif = r_enc_dif;
	
	return r_enc_dif;
}
