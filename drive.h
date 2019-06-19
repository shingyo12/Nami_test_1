#ifndef __DRIVE_HEADER__
#define __DRIVE_HEADER__

extern void set_vel();
extern void servo_l_mot(int wl_t);
extern void servo_r_mot(int wr_t);
//extern void encoder(void);

enum direction{
	MOT_FORWARD,
	MOT_BACKWARD
};

//extern int r_enc_def;
//extern int l_enc_def;
extern int distance;		//tranveled distance [mm]
extern int velocity;
extern int omega;

#endif

