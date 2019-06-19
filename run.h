#ifndef RUN_H
#define RUN_H

extern void servo(int acl, int str);
extern int l_encoder(void);
extern int r_encoder(void);

#define P_OM	1
#define D_OM	1
#define P_TH	1
#define D_TH	1

#define TV 30

extern short int move;
extern int last_dis;
extern int GA;
//extern int r_enc_def;
//extern int l_enc_def;
extern int distance;

#endif