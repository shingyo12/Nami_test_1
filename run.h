#ifndef RUN_H
#define RUN_H

extern void run(float GA);
extern void mem_run(float GA);
extern void run_mcr(float GA);
extern void memorize(void);

#define P_OM	1
#define D_OM	1
#define P_TH	1
#define D_TH	1

#define TV 30

extern short int move;
extern float last_dis;

#endif