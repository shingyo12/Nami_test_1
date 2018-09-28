#ifndef __DRIVE_HEADER__
#define __DRIVE_HEADER__

extern void direction_r_mot(char direction);
extern void direction_l_mot(char direction);
extern void set_vel(float vl,float om);
extern void servo_l_mot(float wl_t);
extern void servo_r_mot(float wr_t);
extern void encoder(void);

enum direction{
	MOT_FORWARD,
	MOT_BACKWARD
};

extern int r_enc_def;
extern int l_enc_def;
extern float distance;		//tranveled distance [mm]

#endif

