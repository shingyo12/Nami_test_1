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

int l_enc,r_enc;

//about encoder
#define KEY_PRCR 0xA500	//�v���e�N�g�����Ŏg�p����L�[ 0xA5 �Œ�
#define PRCR_CLK 0x0001	//�N���b�N�֘A
#define PRCR_MOD 0x0002	//���샂�[�h�A����d�͒ጸ�@�\�A�\�t�g�E�F�A���Z�b�g�֘A
#define PRCR_LVD 0x0008	//LVD�֘A
#define PRCR_ALL (PRCR_CLK|PRCR_MOD|PRCR_LVD)	//�S��

#define PROTECTOFF		(KEY_PRCR|PRCR_CLK|PRCR_MOD)
#define PROTECTON		(KEY_PRCR)
#define PFSPROTECTOFF	0x40u
#define PFSPROTECTON	0x80u

struct marker_data{
	int	speed;
	int	odometer;
};

struct marker_data maker[50];

short int move=0;
int last_dis;

static float pst_op_wl=0,pst_op_wr=0; //�ߋ���p����̑����

//acl:m/s str:degrees
void servo(int vel, int w){
	static int pwm_r,pwm_l;
	//int l_enc,r_enc;
	float tgt_wl,tgt_wr; //�ԗւ̖ڕW�p���x
	float crt_wl,crt_wr; //�ԗւ̌��݂̊p���x
	float op_wl,op_wr; //p����̑����
	
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
	//MOT_STBY = 1;
	
	//omega �ڕW�p���x
	tgt_wl = vel/Rw - (w*T)/(2*Rw);
	tgt_wr = vel/Rw + (w*T)/(2*Rw);
	//tgt_wl = vel;
	//tgt_wr = vel;
	
	//���݂̊p���x
	crt_wl = (l_enc * 2*PI * 1000)/(CPR*GR);
	crt_wr = (r_enc * 2*PI * 1000)/(CPR*GR);
	
	//���݂̑����
	op_wl=tgt_wl - crt_wl;
	op_wr=tgt_wr - crt_wr;
	
	//PWM�l PD����
	pwm_l = (ICPT_L + SLP_L * tgt_wl) - P_W * op_wl;// + D_W * (op_wl - pst_op_wl);
	pwm_r = (ICPT_R + SLP_R * tgt_wr) - P_W * op_wr;// + D_W * (op_wr - pst_op_wr);
	
	//�ߋ��̑����
	pst_op_wl = op_wl;
	pst_op_wr = op_wr;
	
	//pwm_l=out_pwm_l-(int)(f_kp*((1663-5.4*out_pwm_l)-l_enc))+(int)(f_kd*(dif_l-d_dif_l));
	//pwm_r=out_pwm_r-(int)(f_kp*((1663-5.4*out_pwm_r)-r_enc))+(int)(f_kd*(dif_r-d_dif_r));
	//pwm_l = vel;
	//pwm_r = vel;
	
	if(pwm_l>320)pwm_l=320;
	if(pwm_l<0)pwm_l=0;
	if(pwm_r>320)pwm_r=320;
	if(pwm_r<0)pwm_r=0;
	
	MOT_L_IN1 = 0;
	MOT_L_IN2 = 1;
	MTU4.TGRC=pwm_l;
	
	MOT_R_IN1 = 0;
	MOT_R_IN2 = 1;
	MTU4.TGRA=pwm_r;
	
	sci_printf("vel:%d l_pwm:%d r_pwm:%d\r\n",vel,(int)pwm_l,(int)pwm_r);
}


void servo_backup(int acl, int str){
	float p_acl,p_str;
	static int out_pwm_r,out_pwm_l;
	static int pwm_r,pwm_l;
	float f_kp,f_kd;
	float dif_r,dif_l;
	float l_str,r_str;
	
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
	//MOT_STBY = 1;
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
	
	//l_enc = l_encoder();
	//r_enc = r_encoder();
	
	dif_l=(1663-5.4*out_pwm_l);
	dif_r=(1663-5.4*out_pwm_r);
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
	

}

//�G���R�[�_���͏�����
void init_ENC()	//init_MTU�̌�Ŏ��s����ƗL���ɂȂ�
{
	PORTA.PDR.BIT.B4 = 0x00;	//PA4 ����
	PORTA.PDR.BIT.B6 = 0x00;	//PA6 ����
	PORTA.PDR.BIT.B1 = 0x00;	//PC4 ����
	PORTA.PDR.BIT.B3 = 0x00;	//PC5 ����
	
	PORTA.PMR.BIT.B4 = 1;	//���Ӌ@�\�Ƃ��Ďg�p
	PORTA.PMR.BIT.B6 = 1;	//���Ӌ@�\�Ƃ��Ďg�p
	PORTA.PMR.BIT.B1 = 1;	//���Ӌ@�\�Ƃ��Ďg�p
	PORTA.PMR.BIT.B3 = 1;	//���Ӌ@�\�Ƃ��Ďg�p

	//�v���A�b�v�@�\ 2019/02/25�ǉ�
	//PORTA.PCR.BIT.B4=1;				//PA4���v���A�b�v
	//PORTA.PCR.BIT.B6=1;				//PA6���v���A�b�v
	//PORTC.PCR.BIT.B4=1;				//PC4���v���A�b�v
	//PORTC.PCR.BIT.B5=1;				//PC5���v���A�b�v
	
	//����@�\�ݒ�
	MPC.PWPR.BIT.B0WI = 0;			//PFS���W�X�^�̕ی����������PFSWE���W�X�^�̕ی����
	MPC.PWPR.BIT.PFSWE = 1;			//PFS���W�X�^�̕ی����

	//Pxx �[�q�@�\���䃌�W�X�^�iPxxPFS�j
	MPC.PA4PFS.BIT.PSEL = 0x2u;		//MTCLKA
	MPC.PA6PFS.BIT.PSEL = 0x2u;		//MTCLKB
	MPC.PC4PFS.BIT.PSEL = 0x2u;		//MTCLKC
	MPC.PC5PFS.BIT.PSEL = 0x2u;		//MTCLKD

	MPC.PWPR.BIT.PFSWE = 0;			//PFS���W�X�^�̕ی�;
	MPC.PWPR.BIT.B0WI = 1;			//PFS���W�X�^�̕ی������PFSWE���W�X�^�̕ی�

	SYSTEM.PRCR.WORD = PROTECTOFF;	//�v���e�N�g����
	MSTP(MTU) = 0x0;	// MTU���W���[����~����
	SYSTEM.PRCR.WORD = PROTECTON;	//�v���e�N�g

	//�J�E���g��~
	MTU.TSTR.BIT.CST1 = 0x0u;
	MTU.TSTR.BIT.CST2 = 0x0u;

	//�J�E���^�̓�������
	MTU.TSYR.BIT.SYNC1=0;
	MTU.TSYR.BIT.SYNC2=0;

	//�ʑ��v�����[�h
	MTU1.TMDR.BYTE = 0x04u;	
	MTU2.TMDR.BYTE = 0x04u;	

	//�����N���A�֎~
	MTU1.TCR.BIT.CCLR = 0;
	MTU2.TCR.BIT.CCLR = 0;

	//�J�E���g�l�N���A
	MTU1.TCNT = 0;
	MTU2.TCNT = 0;

	//�J�E���g�J�n
	MTU.TSTR.BIT.CST1 = 0x1u;
	MTU.TSTR.BIT.CST2 = 0x1u;

}

//�G���R�[�_�̒l�擾
/*unsigned int get_enc(short port)
{
	if(port==0)
		return MTU1.TCNT;
	else if(port==1)
		return MTU2.TCNT;
	else
		return -1;
}*/

int d_l_enc=0, l_enc_dif=0;
//int l_enc_dif=0;
void l_encoder(void){
	l_enc_dif = L_ENC - d_l_enc;
	
	d_l_enc = L_ENC;
	//sci_printf("l_enc_dif:%d \r\n",l_enc_dif);
	
	l_enc = l_enc_dif;
}

int d_r_enc=0,r_enc_dif=0;
void r_encoder(void){
	r_enc_dif = R_ENC - d_r_enc;
	
	d_r_enc = R_ENC;
	//sci_printf("r_enc_dif:%d \r\n",r_enc_dif);
	
	r_enc = r_enc_dif;
}
