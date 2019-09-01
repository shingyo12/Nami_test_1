#ifndef CAMERA_H
#define CAMERA_H

struct camera{
	int line;
	int mk_r;
	int mk_l;
	int stop;
	int mk_r_cnt;
	int mk_l_cnt;
};

extern void line_scan();
extern void init_cam(void);
extern void camera_calib(void);

extern struct camera cam_data;
extern int cam[128];
extern short int bin_cam[128];
extern int ref_cam[128];

extern int line_pos;

#define STD_CAM 0

#endif