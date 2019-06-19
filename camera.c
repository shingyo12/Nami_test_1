#include "define_mouse.h"
#include "interrupt.h"
#include "sensor.h"
#include "switch.h"
#include "sci.h"
#include "mode.h"
#include "output.h"
#include "camera.h"
#include "run.h"

#define CAM_CLB 50
#define CAM_FRC 20
#define RK 120

int cam[128];
int ref_cam[128];
short int bin_cam[128];
int up_cam[128],lw_cam[128];
int h=0,stp_cnt=0;
short int clk = 0, rokou=0;

struct camera cam_data;

void init_cam(){
	short int i;
	for(i=0;i<128;++i){
		ref_cam[i]=STD_CAM;
	}
}

void line_scan(int spd){
	short int i;
	short int sg_0,sg_1;
	static int d_sg;
	int w_area[50][3];
	int w_num;
	int str;
	
	//LED_1 = CHIP_LED_OFF;
	
	h=0;
	//sci_printf("\r\n");
	for(i=0;i<128;++i){
		CAM_CLK=1;
		S12AD.ADANSA.WORD = 0x01;			//AN000を選択
		while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
		S12AD.ADCSR.BIT.ADST = 1;			//AD変換開始
		cam[i]=CAM_AOO;
		//if(cam[i]>500){
		if(cam[i]>ref_cam[i]){
			bin_cam[i]=1;
			//sci_printf("|");
		}else{
			bin_cam[i]=0;
			//sci_printf(".");
		}
		//sci_printf(" %d", cam[i]);
		while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
		//sci_printf("*");
		CAM_CLK=0;
	}
	//sci_printf("\r\n");
	CAM_SI=1;
	CAM_CLK=1;
	CAM_SI=0;
	CAM_CLK=0; 
		
	w_num=0;
	for(i=0;i<127;++i){
		if(bin_cam[i+1]>bin_cam[i]){
			sci_printf("%d[",bin_cam[i]);
			w_area[w_num][0]=i;
		}else if(bin_cam[i+1]<bin_cam[i]){
			sci_printf("%d]",bin_cam[i]);
			w_area[w_num][1]=i;
			w_area[w_num][2]=(w_area[w_num][0]+w_area[w_num][1])/2;
			w_num++;
		}else{
			sci_printf("%d",bin_cam[i]);
		}
	}
	sci_printf("   ");
	/*sci_printf("     h:%d ", h);
	for(i=0;i<w_num;++i){
		sci_printf("%d ", w_area[i][2]);
	}
	sci_printf("\r\n");
	*/
	if(w_num==2){
		LED_2 = CHIP_LED_ON;
		sg_0=abs(w_area[0][2] - d_sg);
		sg_1=abs(w_area[1][2] - d_sg);
		if(sg_0 > sg_1){
			cam_data.sg=w_area[1][2];
			cam_data.mk_l = 1;
			cam_data.mk_r = 0;
			cam_data.stop = 1;
		}else{
			cam_data.sg=w_area[0][2];
			cam_data.mk_l = 0;
			cam_data.mk_r = 1;
			if(stp_cnt>5000){
				cam_data.stop = 0;
			}else{
				cam_data.stop = 1;
			}
		}
	}else if(h==3){
		if(move > 0)
			LED_2 = CHIP_LED_OFF;
		cam_data.sg = w_area[1][2];
		cam_data.mk_l = 0;
		cam_data.mk_r = 0;
		cam_data.stop = 1;
	}else if(h==0){
		if(move > 0)
			LED_2 = CHIP_LED_OFF;
		cam_data.stop = 0;
	}else{
		if(move > 0)
			LED_2 = CHIP_LED_OFF;
		cam_data.sg = w_area[0][2];
		cam_data.mk_l = 0;
		cam_data.mk_r = 0;
		cam_data.stop = 1;
	}
	d_sg = cam_data.sg;
	stp_cnt++;
	if(stp_cnt>5010)stp_cnt=5010;
	
	str = 120*(cam_data.sg-64)/64;
	if(cam_data.stop == 0){
		//move=0;
	}
	servo(spd,str);
	sci_printf("camera move:%d sg:%d str:%d spd:%d \r\n",move,cam_data.sg,str,spd);
}
	
int str;	
void line_scan2(int spd){
	short int i;
	short int sg_0,sg_1;
	static int d_sg;
	int w_area[50][3];
	int w_num;
	
	//LED_1 = CHIP_LED_OFF;
	
	if(clk >= 128){
		rokou++;
		if(rokou>RK){
			rokou=0;
			clk=0;
			CAM_SI=1;
			CAM_CLK=1;
			CAM_SI=0;
			CAM_CLK=0; 
		
			w_num=0;
			for(i=20;i<107;++i){
				if(bin_cam[i+1]>bin_cam[i]){
					sci_printf("%d[",bin_cam[i]);
					w_area[w_num][0]=i;
				}else if(bin_cam[i+1]<bin_cam[i]){
					sci_printf("%d]",bin_cam[i]);
					w_area[w_num][1]=i;
					w_area[w_num][2]=(w_area[w_num][0]+w_area[w_num][1])/2;
					w_num++;
				}else{
					sci_printf("%d",bin_cam[i]);
				}
			}
			sci_printf("\r\n");
			//sci_printf("   ");
			/*sci_printf("     h:%d ", h);
			for(i=0;i<w_num;++i){
				sci_printf("%d ", w_area[i][2]);
			}
				sci_printf("\r\n");
			*/
			if(w_num==2){
				LED_2 = CHIP_LED_ON;
				sg_0=abs(w_area[0][2] - d_sg);
				sg_1=abs(w_area[1][2] - d_sg);
				if(sg_0 > sg_1){
					cam_data.sg=w_area[1][2];
					cam_data.mk_l = 1;
					cam_data.mk_r = 0;
					cam_data.stop = 1;
				}else{
					cam_data.sg=w_area[0][2];
					cam_data.mk_l = 0;
					cam_data.mk_r = 1;
					if(stp_cnt>10000){
						cam_data.stop = 0;
					}else{
						cam_data.stop = 1;
					}
				}
			}else if(w_num==3){
				if(move > 0)
					LED_2 = CHIP_LED_OFF;
				cam_data.sg = w_area[1][2];
				cam_data.mk_l = 0;
				cam_data.mk_r = 0;
				cam_data.stop = 1;
			}else if(w_num==0){
				//if(move > 0)
				//	LED_2 = CHIP_LED_OFF;
				//cam_data.stop = 0;
			}else{
				if(move > 0)
					LED_2 = CHIP_LED_OFF;
				cam_data.sg = w_area[0][2];
				cam_data.mk_l = 0;
				cam_data.mk_r = 0;
				cam_data.stop = 1;
			}
			d_sg = cam_data.sg;
			stp_cnt++;
			if(stp_cnt>10010)stp_cnt=10010;
	
			str = 130*(cam_data.sg-64)/64;
			if(cam_data.stop == 0){
				move=0;
			}
			//servo(spd,str);
			//sci_printf("camera move:%d sg:%d str:%d spd:%d \r\n",move,cam_data.sg,str,spd);
		}
	}else{
		CAM_CLK=1;
		S12AD.ADANSA.WORD = 0x01;			//AN000を選択
		while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
		S12AD.ADCSR.BIT.ADST = 1;			//AD変換開始
		cam[clk]=CAM_AOO;
		//if(cam[i]>500){
		if(cam[clk]>ref_cam[clk]){
			bin_cam[clk]=1;
			//sci_printf("|");
		}else{
			bin_cam[clk]=0;
			//sci_printf(".");
		}
		//sci_printf(" %d", cam[i]);
		while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
		//sci_printf("*");
		CAM_CLK=0;
		clk++;
	}
	servo(spd,str);
}
	
void line_scan3(int spd){
	short int i;
	short int sg_0,sg_1;
	static int d_sg;
	int w_area[50][3];
	int w_num;
	
	//LED_1 = CHIP_LED_OFF;
	
	if(clk >= 128){
		rokou++;
		if(rokou>RK){
			rokou=0;
			clk=0;
			CAM_SI=1;
			CAM_CLK=1;
			CAM_SI=0;
			CAM_CLK=0; 
		
			w_num=0;
			for(i=20;i<107;++i){
				if(bin_cam[i+1]>bin_cam[i]){
					sci_printf("%d[",bin_cam[i]);
					w_area[w_num][0]=i;
				}else if(bin_cam[i+1]<bin_cam[i]){
					sci_printf("%d]",bin_cam[i]);
					w_area[w_num][1]=i;
					w_area[w_num][2]=(w_area[w_num][0]+w_area[w_num][1])/2;
					w_num++;
				}else{
					sci_printf("%d",bin_cam[i]);
				}
			}
			sci_printf("\r\n");
			//sci_printf("   ");
			/*sci_printf("     h:%d ", h);
			for(i=0;i<w_num;++i){
				sci_printf("%d ", w_area[i][2]);
			}
				sci_printf("\r\n");
			*/
			if(w_num==2){
				LED_2 = CHIP_LED_ON;
				sg_0=abs(w_area[0][2] - d_sg);
				sg_1=abs(w_area[1][2] - d_sg);
				if(sg_0 > sg_1){
					cam_data.sg=w_area[1][2];
					cam_data.mk_l = 1;
					cam_data.mk_r = 0;
					cam_data.stop = 1;
				}else{
					cam_data.sg=w_area[0][2];
					cam_data.mk_l = 0;
					cam_data.mk_r = 1;
					if(stp_cnt>10000){
						cam_data.stop = 0;
					}else{
						cam_data.stop = 1;
					}
				}
			}else if(w_num==3){
				if(move > 0)
					LED_2 = CHIP_LED_OFF;
				cam_data.sg = w_area[1][2];
				cam_data.mk_l = 0;
				cam_data.mk_r = 0;
				cam_data.stop = 1;
			}else if(w_num==0){
				//if(move > 0)
				//	LED_2 = CHIP_LED_OFF;
				//cam_data.stop = 0;
			}else{
				if(move > 0)
					LED_2 = CHIP_LED_OFF;
				cam_data.sg = w_area[0][2];
				cam_data.mk_l = 0;
				cam_data.mk_r = 0;
				cam_data.stop = 1;
			}
			d_sg = cam_data.sg;
			stp_cnt++;
			if(stp_cnt>10010)stp_cnt=10010;
	
			str = 120*(cam_data.sg-64)/64;
			if(cam_data.stop == 0){
				move=0;
			}
			//servo(spd,str);
			//sci_printf("camera move:%d sg:%d str:%d spd:%d \r\n",move,cam_data.sg,str,spd);
		}
	}else{
		CAM_CLK=1;
		S12AD.ADANSA.WORD = 0x01;			//AN000を選択
		while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
		S12AD.ADCSR.BIT.ADST = 1;			//AD変換開始
		cam[clk]=CAM_AOO;
		//if(cam[i]>500){
		if(cam[clk]>ref_cam[clk]){
			bin_cam[clk]=1;
			//sci_printf("|");
		}else{
			bin_cam[clk]=0;
			//sci_printf(".");
		}
		//sci_printf(" %d", cam[i]);
		while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
		//sci_printf("*");
		CAM_CLK=0;
		clk++;
	}
	servo(spd,str);
}

void camera_calib(){
	short int i;
	
	if(move==1){
		h++;
		//sci_printf("\r\n");
		for(i=0;i<128;++i){
			CAM_CLK=1;
			S12AD.ADANSA.WORD = 0x01;			//AN000を選択
			while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
			S12AD.ADCSR.BIT.ADST = 1;			//AD変換開始
			cam[i]=CAM_AOO;
		
			//sci_printf("%d", bin_cam[i]);
			while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
			//sci_printf("%d", cam[i]);
			CAM_CLK=0;
			
			if(h==1){
				//ref_cam[i]=cam[i];
				up_cam[i]=lw_cam[i]=cam[i];
			}else{
				//ref_cam[i]=ref_cam[i]+cam[i];
				if(up_cam[i]<cam[i])up_cam[i]=cam[i];
				//if(lw_cam[i]>cam[i] && cam[i]>100)lw_cam[i]=cam[i];
			}
			//sci_printf(" %d", cam[i]);
		}
		CAM_SI=1;
		CAM_CLK=1;
		CAM_SI=0;
		CAM_CLK=0; 
		//sci_printf("\r\n");
		if(h>CAM_CLB){
			move=0;
			servo(0, 0);
			sci_printf("ref");
			for(i=0;i<128;++i){
				ref_cam[i]=(up_cam[i]+lw_cam[i])*0.8;
				sci_printf(" %d", ref_cam[i]);
			}
			sci_printf("\r\n");
			
			sci_printf("max");
			for(i=0;i<128;++i){
				sci_printf(" %d", up_cam[i]);
			}
			sci_printf("\r\n");
			
			sci_printf("min");
			for(i=0;i<128;++i){
				sci_printf(" %d", lw_cam[i]);
			}
			sci_printf("\r\n");
		}
		servo(10, 0);
	}
}

void camera_calib2(){
	short int i;
	
	if(move==1){
		if(clk>=128){
			rokou++;
			if(rokou>RK){
				rokou=0;
				h++;
				clk=0;
				CAM_SI=1;
				CAM_CLK=1;
				CAM_SI=0;
				CAM_CLK=0; 
				//sci_printf("\r\n");
				if(h>CAM_CLB){
					move=0;
					servo(0, 0);
					sci_printf("ref");
					for(i=0;i<128;++i){
						ref_cam[i]=lw_cam[i]+(up_cam[i]-lw_cam[i])*0.8;
						sci_printf(" %d", ref_cam[i]);
					}
					sci_printf("\r\n");
			
					sci_printf("max");
					for(i=0;i<128;++i){
						sci_printf(" %d", up_cam[i]);
					}
					sci_printf("\r\n");
			
					sci_printf("min");
					for(i=0;i<128;++i){
						sci_printf(" %d", lw_cam[i]);
					}
					sci_printf("\r\n");
				}
				servo(10, 0);
			}
		}else{
			CAM_CLK=1;
			S12AD.ADANSA.WORD = 0x01;			//AN000を選択
			while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
			S12AD.ADCSR.BIT.ADST = 1;			//AD変換開始
			cam[clk]=CAM_AOO;
		
			//sci_printf("%d", bin_cam[i]);
			while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
			//sci_printf("%d", cam[i]);
			CAM_CLK=0;
			
			if(h==1){
				//ref_cam[i]=cam[i];
				up_cam[clk]=lw_cam[clk]=cam[clk];
			}else{
				//ref_cam[i]=ref_cam[i]+cam[i];
				if(up_cam[clk]<cam[clk])up_cam[clk]=cam[clk];
				//if(lw_cam[i]>cam[i] && cam[i]>100)lw_cam[i]=cam[i];
			}
			//sci_printf(" %d", cam[i]);
			clk++;
		}
	}	
}