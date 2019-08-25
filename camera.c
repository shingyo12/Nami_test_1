#include "define_mouse.h"
#include "interrupt.h"
#include "sensor.h"
#include "switch.h"
#include "sci.h"
#include "mode.h"
#include "output.h"
#include "camera.h"
#include "run.h"

#define CAM_CLB 5000
#define CAM_FRC 20
#define RK 0
#define GAP 6
#define NUM_DIF 10
#define UP 10
#define UND 0

int cam[128];
int ref_cam[128];
short int bin_cam[128];
short int pst_cam[128];
int dif_cam[128];
int up_cam[128],lw_cam[128];
int h=0,stp_cnt=0,g=0;
short int clk = 0, rokou=0;
int line_pos;
//int find_start = 64;
int pst_line_cam = 0;
int pst_line_pos = 64;

struct camera cam_data;

int rokou_cnt = 50;

void init_cam(){
	short int i;
	for(i=0;i<128;++i){
		ref_cam[i]=STD_CAM;
		pst_cam[i]=STD_CAM;
	}
}

void line_scan(){
	short int i;
	short int sg_0,sg_1;
	static int d_sg;
	int w_area[50][3];
	int w_num;
	
	//LED_1 = CHIP_LED_OFF;

	//sci_printf("\r\n");
	if(rokou > rokou_cnt){
		//CAM_CLK=0; 
		for(i=0;i<127;++i){
			CAM_CLK=1;
			S12AD.ADANSA.WORD = 0x01;			//AN000を選択
			while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
			S12AD.ADCSR.BIT.ADST = 1;			//AD変換開始
			cam[i]=CAM_AOO;
			while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
			//sci_printf("*");
			//sci_printf("%d ",cam[i]/100);
			CAM_CLK=0;
		}
		CAM_SI=1;
		CAM_CLK=1;
		CAM_SI=0;
		CAM_CLK=0;
		w_num=0;
		for(i=0;i<127-NUM_DIF;i++){
			dif_cam[i]=cam[i+NUM_DIF]-cam[i];
			//sci_printf("%d ",dif_cam[i]);
		}
		//find zero point
		if(pst_line_cam < dif_cam[pst_line_pos] ){
			for(i=pst_line_pos;i<127-NUM_DIF;i++){
				if(dif_cam[i-GAP]>UP && dif_cam[i]<UND){
					//sci_printf("| ");
					line_pos = i-3;
				}
			}
		}else{
			for(i=pst_line_pos;i>GAP;i--){
				if(dif_cam[i-GAP]>UP && dif_cam[i]<UND){
					//sci_printf("| ");
					line_pos = i-3;
				}
			}
		}
		pst_line_pos = line_pos;
		//sci_printf("cam64:%d cam_dif64:%d line %d \r\n",cam[64],dif_cam[64],line_pos);
		rokou=0;
		
		//marker
		for(i=line_pos-18; i>line_pos-40; i--){
			if(cam[i] > ref_cam[i]){
				LED_2 = CHIP_LED_ON;
				BUZZ_OUT = 1;
				break;
			}else{
				LED_2 = CHIP_LED_OFF;
				BUZZ_OUT = 0;
			}
		}
		for(i=line_pos+18; i<line_pos+40; i++){
			if(cam[i] > ref_cam[i]){
				LED_1 = CHIP_LED_ON;
				BUZZ_OUT = 1;
				break;
			}else{
				LED_1 = CHIP_LED_OFF;
				BUZZ_OUT = 0;
			}
		}
		if(g>CAM_CLB){
			for(i=0;i<127;i++){
				sci_printf("%d ",cam[i]);
			}
			sci_printf("\r\n");
			g=0;
		}
		g++;
	}
	
	//sci_printf("camera move:%d sg:%d str:%d spd:%d \r\n",move,cam_data.sg,str,spd);
	rokou++;
}
	
int str;	
void line_scan2(){
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
			for(i=0;i<127;++i){
				if(bin_cam[i+1]>bin_cam[i]){
					//sci_printf("%d[",bin_cam[i]);
					w_area[w_num][0]=i;
				}else if(bin_cam[i+1]<bin_cam[i]){
					//sci_printf("%d]",bin_cam[i]);
					w_area[w_num][1]=i;
					w_area[w_num][2]=(w_area[w_num][0]+w_area[w_num][1])/2;
					w_num++;
				}else{
					//sci_printf("%d",bin_cam[i]);
				}
				if(i==64){
					//sci_printf("#");
				}
			}
			
			///////////////////////////////////
			for(i=0;i<127-NUM_DIF;i++){	
				dif_cam[i]=cam[i+NUM_DIF]-cam[i];
			}
			//find zero point
			for(i=GAP;i<127-NUM_DIF;i++){
				//sci_printf("%d ",dif_cam[i]);
				if(dif_cam[i-GAP]>UP && dif_cam[i]<UND){
					//sci_printf("| ");
					sci_printf("%d ",i);
				}
			}
			sci_printf("\r\n");
			///////////////////////////////////
			
			//marker
			if(cam_data.line_pos>22 && cam_data.line_pos<103){
				if(bin_cam[cam_data.line_pos+22]){
					cam_data.mk_r=1;
					BUZZ_OUT = 1;
					LED_1 = CHIP_LED_ON;
				}
				else if(bin_cam[cam_data.line_pos-22]){
					cam_data.mk_l=1;
					BUZZ_OUT = 1;
					LED_1 = CHIP_LED_ON;
				}
				else{
					cam_data.mk_r=0;
					cam_data.mk_r=0;
					BUZZ_OUT = 0;
					LED_1 = CHIP_LED_OFF;
				}
			}else{
				cam_data.mk_r=0;
				cam_data.mk_r=0;
				BUZZ_OUT = 0;
				LED_1 = CHIP_LED_OFF;
			}
			
			stp_cnt++;
			if(stp_cnt>10010)stp_cnt=10010;
	
			str = 130*(cam_data.line_pos-64)/64;
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
}

void camera_calib(){
	short int i;
	
	if(move==1){
		h++;
		if(rokou > rokou_cnt){
			//CAM_CLK=0; 
			for(i=0;i<127;++i){
				CAM_CLK=1;
				S12AD.ADANSA.WORD = 0x01;			//AN000を選択
				while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
				S12AD.ADCSR.BIT.ADST = 1;			//AD変換開始
				cam[i]=CAM_AOO;
				while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
				//sci_printf("*");
				//sci_printf("%d ",cam[i]/100);
				CAM_CLK=0;
			}
			CAM_SI=1;
			CAM_CLK=1;
			CAM_SI=0;
			CAM_CLK=0;

			for(i=0;i<127;++i){
				if(h==1){
					//ref_cam[i]=cam[i];
					up_cam[i]=lw_cam[i]=cam[i];
				}else{
					//ref_cam[i]=ref_cam[i]+cam[i];
					if(up_cam[i]<cam[i]) up_cam[i]=cam[i];
					//if(lw_cam[i]>cam[i] && cam[i]>100)lw_cam[i]=cam[i];
				}
			}
			
			//sci_printf("cam64:%d cam_dif64:%d line %d \r\n",cam[64],dif_cam[64],line_pos);
			rokou=0;
			if(h>CAM_CLB){
				move=0;
				servo(0, 0);
				//sci_printf("ref");
				for(i=0;i<128;++i){
					ref_cam[i]=((lw_cam[i]+up_cam[i])*2)/3;
					sci_printf(" %d", cam[i]);
				}
				sci_printf(" \r\n");
			}
		}
		servo(200, 0);
		//sci_printf("camera move:%d sg:%d str:%d spd:%d \r\n",move,cam_data.sg,str,spd);
		rokou++;
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
					//sci_printf("ref");
					for(i=0;i<128;++i){
						ref_cam[i]=lw_cam[i]+(up_cam[i]-lw_cam[i])*0.8;
						//sci_printf(" %d", ref_cam[i]);
					}
					//sci_printf("\r\n");
			
					//sci_printf("max");
					for(i=0;i<128;++i){
						//sci_printf(" %d", up_cam[i]);
					}
					//sci_printf("\r\n");
			
					//sci_printf("min");
					for(i=0;i<128;++i){
						//sci_printf(" %d", lw_cam[i]);
					}
					//sci_printf("\r\n");
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