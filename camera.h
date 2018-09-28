#ifndef CAMERA_H
#define CAMERA_H

struct camera{
	float sg;
	int mk_r;
	int mk_l;
	int stop;
};

extern void line_scan();
extern void init_cam(void);
extern void scan(void);

extern struct camera cam_data;
extern int cam[128];
extern short int bin_cam[128];
extern float ref_cam[128];

#define STD_CAM 1000

#endif