#ifndef RUN_H
#define RUN_H

extern void servo(int vel, int w);
extern void servo_backup(int acl, int str);
extern void l_encoder(void);
extern void r_encoder(void);

#define P_OM	1
#define D_OM	1
#define P_TH	1
#define D_TH	1

#define TV 30

#define GR 250
#define CPR 12
#define WHEEL_CIRCLE 45*3.141592
#define OPD  (45*3.141592)/(12*200)    //ONE_PULSE_DISTANSE

#define T 103 //�g���b�h��
#define Rw 22.6 //�ԗւ̔��a
#define ICPT_L 311.82001 //�ڕW�p���x����PWM�l���Z�o����ۂ̂��ؕЁi�����ߎ��j
#define SLP_L -2.46257 //�ڕW�p���x����PWM�l���Z�o����ۂ̔��萔�i�����ߎ��j
#define ICPT_R 313.69623 //�ڕW�p���x����PWM�l���Z�o����ۂ̂��ؕЁi�����ߎ��j
#define SLP_R -2.55123 //�ڕW�p���x����PWM�l���Z�o����ۂ̔��萔�i�����ߎ��j

#define P_W 5 //�ԗւ�PD�p�̒萔(P)
#define D_W 10 //�ԗւ�PD�p�̒萔(D)

extern short int move;
extern int last_dis;
extern int GA;
//extern int r_enc_def;
//extern int l_enc_def;
extern int distance;

extern void init_ENC();//init_MTU�̌�Ŏ��s����ƗL���ɂȂ�
unsigned int get_enc(short port);
extern int l_enc;
extern int r_enc;

#endif