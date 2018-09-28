#include "define_mouse.h"
#include "interrupt.h"
#include "sensor.h"
#include "switch.h"
#include "sci.h"
#include "mode.h"
#include "output.h"
#include "camera.h"
#include "run.h"

int cam[128];
float ref_cam[128];
short int bin_cam[128];
//short int w_mass[128][2];

struct camera cam_data;

void init_cam(){
	short int i;
	for(i=0;i<128;++i){
		ref_cam[i]=STD_CAM;
	}
}

void line_scan(){
	static float w_line = 64;
	short int i,m=0,w_flg=0,e_l,e_r;
	short int w_centor[3];
	short int h,k=0,dis,tmp_dis,w_cnt=0,d_sg,sg_0,sg_1;
	static short int n=0;
	LED_1 = CHIP_LED_OFF;
	
	if(n>10){
		//sci_printf("n ");
		h=0;
		sci_printf("\r\n");
		for(i=0;i<128;++i){
			CAM_CLK=1;
			S12AD.ADANSA.WORD = 0x01;			//AN000を選択
			while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
			S12AD.ADCSR.BIT.ADST = 1;			//AD変換開始
			cam[i]=CAM_AOO;
			if(cam[i]>1000 && w_flg==0){
				bin_cam[i]=1;
				//w_cnt++;
				//w_mass[h][0]=i;
				e_l=i;
				w_flg=1;
				//sci_printf(" %d ", w_mass[h][0]);
			}
			if(cam[i]<1000 && w_flg==1){
				//bin_cam[i]=0;
				//w_mass[h][1]=i;
				e_r=i;
				w_flg=0;
				//sci_printf(" %d ", w_mass[h][1]);
				h++;
				if((e_r-e_l)>10){
					w_centor[k] = (e_r+e_l)/2;
					//sci_printf("\r\n |%d| \r\n", w_centor[k]);
					k++;
				}
			}
			//sci_printf("%d", bin_cam[i]);
			while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
			sci_printf(" %d", cam[i]);
			CAM_CLK=0;
		}
		sci_printf("\r\n");
		CAM_SI=1;
		CAM_CLK=1;
		CAM_SI=0;
		CAM_CLK=0; 
		n=0;
		
		sci_printf("h:%d ", h);
		for(i=0;i<h;++i){
			sci_printf("%d ", w_centor[i]);
		}
		sci_printf("\r\n");
		
		if(h==1){
			sg_0=abs(w_centor[0] - d_sg);
			sg_1=abs(w_centor[1] - d_sg);
			if(sg_0 > sg_1){
				cam_data.sg=w_centor[1];
				cam_data.mk_l = 1;
				cam_data.mk_r = 0;
				cam_data.stop = 0;
			}else{
				cam_data.sg=w_centor[0];
				cam_data.mk_l = 0;
				cam_data.mk_r = 1;
				cam_data.stop = 0;
			}
		}else if(h==2){
			cam_data.sg = w_centor[1];
			cam_data.mk_l = 0;
			cam_data.mk_r = 0;
			cam_data.stop = 0;
		}else if(h==0){
			cam_data.stop = 1;
		}else{
			cam_data.sg = w_centor[0];
			cam_data.mk_l = 0;
			cam_data.mk_r = 0;
			cam_data.stop = 0;
		}
		d_sg = cam_data.sg;
	}
	n++;

	//dis = abs((w_mass[0][0]+w_mass[0][1])*0.5 - w_line);
	//w_line = (w_mass[0][0]+w_mass[0][1])*0.5;

	//cam_data.sg = w_line;
	//sci_printf("camera move:%d sg:%d mk_l:%d mk_r:%d \r\n",move,cam_data.sg,cam_data.mk_l,cam_data.mk_r);
}

void scan(){
	int clk=0,i=0,rokou=0;
	int cam_arr[128];
	for(clk=0;clk<128;++clk){
		CAM_CLK=1;
		S12AD.ADANSA.WORD = 0x01;			//AN000を選択
		while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
		S12AD.ADCSR.BIT.ADST = 1;			//AD変換開始
		cam_arr[clk]=CAM_AOO;
		while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
		//sci_printf("%d ", S12AD.ADDR0);
		CAM_CLK=0;
	}
	for(rokou=0;rokou<10000;rokou++);
	CAM_SI=1;
	CAM_CLK=1;
	CAM_SI=0;
	CAM_CLK=0;
	for(i=0;i<128;++i){
		sci_printf("%d ",cam_arr[i]);
		/*
		if(cam_arr[i]>ref_cam[i]){
			sci_printf("|");
		}else{
			sci_printf("_");
		}*/
	}
	sci_printf("\r\n");
}