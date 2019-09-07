#include "define_mouse.h"
#include "interrupt.h"
#include "sensor.h"
#include "switch.h"
#include "sci.h"
#include "mode.h"
#include "output.h"
#include "camera.h"
#include "run.h"
#include "trace.h"

#define CAM_CLB 300
#define CAM_FRC 50
#define RK 0
#define GAP 8
#define NUM_DIF 10
#define UP 10
#define UND 0
#define CAM_AVE 10

int cam[128];
int ref_cam[128];
short int bin_cam[128];
short int pst_cam[128];
int dif_cam[128];
int up_cam[128],lw_cam[128];
int h=0,mkr_cnt=0;
short int clk = 0, rokou=0;
int line_pos;
int left_mkr;
int right_mkr;
//int find_start = 64;
int pst_line_cam;
int pst_line_pos;

struct camera cam_data;

int rokou_cnt = 10;

long int up_sum_cam,lw_sum_cam;
long int l_sum_cam,r_sum_cam;
long int lup_sum_cam,rup_sum_cam;
long int llw_sum_cam,rlw_sum_cam;
long int l_mkr_cam,r_mkr_cam;
long int cross_cam;
long int l_cam,r_cam;
long int l_max_cam,r_max_cam;
long int l_tmp_cam,r_tmp_cam;
long int l_ave_cam,r_ave_cam;
long int m_sum_cam;
short int r_cnt;
int ave_cam[128];
int max_cam[128];

void init_cam(){
	short int i;
	for(i=0;i<128;++i){
		ref_cam[i]=STD_CAM;
		pst_cam[i]=STD_CAM;
		ave_cam[i]=0;
		max_cam[i]=0;
	}
	line_pos = 68;
	
	//init camera data
	cam_data.line = 68;
	cam_data.mk_l = 0;
	cam_data.mk_r = 0;
	cam_data.stop = 0;
	cam_data.mk_l_cnt = 0;
	cam_data.mk_r_cnt = 0;
	
	pst_line_pos = 64;
	pst_line_cam = 0;
	
	mkr_cnt = 0;
	
	l_cam = r_cam = 0;
	l_max_cam = r_max_cam =0;
	m_sum_cam = 0;
	
	r_cnt = 0;
	
}

void line_scan(){
	short int i;
	static short int tmp_l=0;
	static short int tmp_r=0;
	static short int cnt_mkr_l=0;
	static short int cnt_mkr_r=0;
	static short int pst_cnt_mkr_l=0;
	static short int pst_cnt_mkr_r=0;
	static long int sum_cam = 0;
	int p_cam;
	int k;
	short int r_flg=0,l_flg=0;
	//int l_cam,r_cam;
	
	//LED_1 = CHIP_LED_OFF;

	//sci_printf("\r\n");
	if(rokou > rokou_cnt){
		//CAM_CLK=0; 
		sum_cam = 0;
		
		CAM_SI=1;
		CAM_CLK=1;
		CAM_SI=0;
		CAM_CLK=0;
		for(i=0;i<127;++i){
			CAM_CLK=1;
			S12AD.ADANSA.WORD = 0x01;			//AN000を選択
			while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
			S12AD.ADCSR.BIT.ADST = 1;			//AD変換開始
			cam[i]=CAM_AOO;
			while(S12AD.ADCSR.BIT.ADST == 1);		//ADSTが0になるまで待つ
			//sci_printf("*");
			//sci_printf("%d ",cam[i]/100);
			sum_cam += cam[i]/10;
			CAM_CLK=0;
		}
		//CAM_SI=1;
		//CAM_CLK=1;
		//CAM_SI=0;
		//CAM_CLK=0;
		
		if((sum_cam < cross_cam) && move == 1){	
			//BUZZ_OUT = 0;
			for(i=0;i<127-NUM_DIF;i++){
				dif_cam[i]=cam[i+NUM_DIF]-cam[i];
				//sci_printf("%d ",dif_cam[i]);
			}
			//find zero point
			//sci_printf("pst_line_cam %d dif_cam[pst_line_pos] %d ",pst_line_cam,dif_cam[pst_line_pos]);
			if(pst_line_cam <= dif_cam[pst_line_pos] ){
				for(i=pst_line_pos-GAP;i<127-NUM_DIF;i++){
					if(dif_cam[i-GAP]>UP && dif_cam[i]<UND){
						//sci_printf("-> ");
						line_pos = i - GAP/2;
						//sci_printf("pline1 %d ",pst_line_pos);
						break;
					}
				}
			}else{
				for(i=pst_line_pos+GAP;i>GAP;i--){
					if(dif_cam[i]>UP && dif_cam[i+GAP]<UND){
						//sci_printf("<- ");
						line_pos = i - GAP/2;
						//sci_printf("pine1 %d ",pst_line_pos);
						break;
					}
				}
			}
			//cam_data.line = line_pos;
			//pst_line_pos = line_pos;
			pst_line_cam = dif_cam[line_pos];
			rokou=0;
			
			if(mkr_cnt > CAM_AVE*2){
				if(mkr_cnt == CAM_AVE*2 +1){
					l_ave_cam = l_max_cam + ((l_max_cam - l_cam/(CAM_AVE ))*1)/3;
					r_ave_cam = r_max_cam + ((r_max_cam - r_cam/(CAM_AVE ))*1)/3;
					for(i=0;i<128;++i){
						ave_cam[i] = ave_cam[i]/(CAM_AVE);
						//sci_printf("ave i:%d max:%d ave_cam:%d cam:%d  \r\n",i, max_cam[i],ave_cam[i],cam[i] );
					}
					//sci_printf("\r\n");
					mkr_cnt++;
				}else{
					//maker
					l_tmp_cam = r_tmp_cam = 0;
					for(i=0;i<128;++i){
						p_cam = cam[i]/10;
						if(i<line_pos-35 ){
						//if(i<20){
							l_tmp_cam += p_cam;
						}
						if(i>line_pos+30 ){
						//if(i>90){
							r_tmp_cam += p_cam;
						}
						//l_tmp_cam = cam[line_pos-35];
						//l_ave_cam = max_cam[line_pos-35] + (max_cam[line_pos-35] - ave_cam[line_pos-35])/2;
						
						//r_tmp_cam = cam[line_pos+40];
						//r_ave_cam = max_cam[line_pos+40] + (max_cam[line_pos+40] - ave_cam[line_pos+40])/2;
					}
					if((r_tmp_cam > r_ave_cam) ){
						LED_2 = CHIP_LED_ON;
						for(k=0;k<6000;k++);
						if((l_tmp_cam < l_ave_cam) && (r_tmp_cam > r_ave_cam) ){
							//for(k=0;k<6000;k++);
							//LED_2 = CHIP_LED_ON;
							//BUZZ_OUT = 1;
							tmp_r = 1;
							r_cnt++;
							if(r_cnt > 2){
								//BUZZ_OUT = 1;
								//cam_data.mk_r_cnt++;
								//if((l_tmp_cam < l_ave_cam) && (r_tmp_cam > r_ave_cam) ){
									if(l_flg == 0){
										r_flg = 1;
									}else{
										r_flg = 0;
									}
								//}
								//for(k=0;k<6000;k++);
							}
							//for(k=0;k<6000;k++);
							//r_flg = 1;
							//BUZZ_OUT = 0;
						}else{
							r_cnt=0;
							r_flg = 0;
							//BUZZ_OUT = 0;
						}
						//BUZZ_OUT = 0;
						//tmp_r = 1;
						
					}else{
						tmp_r = 0;
						LED_2 = CHIP_LED_OFF;
						r_cnt = 0;
						r_flg = 0;
					}
					if((l_tmp_cam > l_ave_cam) ){
						LED_1 = CHIP_LED_ON;
						for(k=0;k<60000;k++);
						if((r_tmp_cam < r_ave_cam) ){
							//LED_1 = CHIP_LED_ON;
							tmp_l = 1;
						}
						//BUZZ_OUT = 1;
						//tmp_l = 1;
						r_cnt = 0;
						r_flg = 0;
						l_flg = 1;
					}else{
						tmp_l = 0;
						LED_1 = CHIP_LED_OFF;
						l_flg = 0;
					}
					
					if(tmp_l == 0 && tmp_r == 0){
						BUZZ_OUT = 0;
					}
					
					if(r_flg==1){
						BUZZ_OUT = 1;
						cam_data.mk_r_cnt++;
						if(cam_data.mk_r_cnt<2){
							for(k=0;k<30000;k++);
						}
						//BUZZ_OUT = 0;
						
					}else{
						BUZZ_OUT = 0;
					}
					//sci_printf("l:%d %d r:%d %d \r\n",l_tmp_cam, l_ave_cam ,r_tmp_cam ,r_ave_cam );
				}
			}else if(mkr_cnt > CAM_AVE){
				mkr_cnt++;
				lup_sum_cam = rup_sum_cam = 0;
				for(i=0;i<128;++i){
					p_cam = cam[i]/10;
					//if(i<20){
					if(i<line_pos-35 ){
						l_cam += p_cam;
						lup_sum_cam += p_cam;
					}
					//if(i>90){
					if(i>line_pos+30 ){
						r_cam += p_cam;
						rup_sum_cam += p_cam;
					}
					
					//ave
					ave_cam[i] += p_cam;
					if(max_cam[i]<p_cam){
						max_cam[i] = p_cam;
					}
				}
				if(l_max_cam < lup_sum_cam){
					l_max_cam = lup_sum_cam;
				}
				if(r_max_cam < rup_sum_cam){
					r_max_cam = rup_sum_cam;
				}
			}else{
				mkr_cnt++;
				sci_printf("%d \r\n",mkr_cnt);
			}
			
			//maker
			/*for(i=0;i<128;++i){
				p_cam = cam[i]/10;
				if(i<24){
					l_cam += p_cam;
				}
				if(i>100){
					r_cam += p_cam;
				}
			}
			if(l_cam > l_mkr_cam ){
				LED_2 = CHIP_LED_ON;
				//BUZZ_OUT = 1;
				tmp_l = 1;
			}else{
				tmp_l = 0;
				LED_2 = CHIP_LED_OFF;
				//BUZZ_OUT = 0;
			}
			if(cam[i] > ref_cam[i]){
				LED_2 = CHIP_LED_ON;
				//BUZZ_OUT = 1;
				tmp_l = 1;
			}else{
				tmp_l = 0;
				LED_2 = CHIP_LED_OFF;
				//BUZZ_OUT = 0;
			}
			if(tmp_l == 0 && tmp_r == 0){
				BUZZ_OUT = 0;
			}
			*/
			
			//marker
			/*for(i=line_pos-18; i>5 ; i--){
				if(cam[i] > ref_cam[i]){
					LED_2 = CHIP_LED_ON;
					BUZZ_OUT = 1;
					tmp_l = 1;
					break;
				}else{
					tmp_l = 0;
					LED_2 = CHIP_LED_OFF;
					//BUZZ_OUT = 0;
				}
			}
			for(i=line_pos+18; i<122 ; i++){
				if(cam[i] > ref_cam[i]){
					LED_1 = CHIP_LED_ON;
					BUZZ_OUT = 1;
					tmp_r = 1;
					break;
				}else{
					tmp_r = 0;
					LED_1 = CHIP_LED_OFF;
					//BUZZ_OUT = 0;
				}
			}
			if(tmp_l == 0 && tmp_r == 0){
				BUZZ_OUT = 0;
			}
		
			//maker setting
			if(tmp_l == 1 && tmp_r == 0){
				cam_data.mk_l = 1;
				cnt_mkr_l = 1; 
			}else if(tmp_l == 0 && tmp_r == 1){
				cam_data.mk_r = 1;
				cnt_mkr_r = 1;
			}else{
				cam_data.mk_l = 0;
				cam_data.mk_r = 0;
				cnt_mkr_l = 0;
				cnt_mkr_r = 0;
			}
			
			//count marker
			if(cnt_mkr_l == 0 && pst_cnt_mkr_l == 1){
				cam_data.mk_l_cnt++;
			}
			if(cnt_mkr_r == 0 && pst_cnt_mkr_r == 1){
				cam_data.mk_r_cnt++;
			}
			pst_cnt_mkr_l = cnt_mkr_l;
			pst_cnt_mkr_r = cnt_mkr_r;
			*/
		}else{
			//BUZZ_OUT = 1;
			line_pos = 64;
		}
		cam_data.line = line_pos;
		pst_line_pos = line_pos;
	}
	
	//sci_printf("camera move:%d sg:%d str:%d spd:%d \r\n",move,cam_data.sg,str,spd);
	rokou++;
}

void camera_calib(){
	short int i;
	
	if(move==1){
		//h++;
		if(rokou > rokou_cnt){
			h++;
			//CAM_CLK=0; 
			CAM_SI=1;
			CAM_CLK=1;
			CAM_SI=0;
			CAM_CLK=0;
			for(i=0;i<128;++i){
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
			//CAM_SI=1;
			//CAM_CLK=1;
			//CAM_SI=0;
			//CAM_CLK=0;

			for(i=0;i<128;++i){
				if(h==1){
					//ref_cam[i]=cam[i];
					up_cam[i]=lw_cam[i]=cam[i];
				}else{
					//ref_cam[i]=ref_cam[i]+cam[i];
					if(up_cam[i]<cam[i]) up_cam[i]=cam[i];
					if(lw_cam[i]>cam[i]) lw_cam[i]=cam[i];
				}
			}

			//sci_printf("sum : %d  l : %d  r : %d\r\n",sum_cam,l_sum_cam,r_sum_cam);
			//sci_printf("cam64:%d cam_dif64:%d line %d \r\n",cam[64],dif_cam[64],line_pos);
			rokou=0;
			if(h>CAM_CLB){
				move=0;
				servo(0, 0);
				//sci_printf("ref");
				for(i=0;i<128;++i){
					ref_cam[i]=((lw_cam[i]+up_cam[i])*2)/4;
					sci_printf("%d:%d ", i,cam[i]);
				}
				sci_printf(" \r\n");
				up_sum_cam = lw_sum_cam = 0;
				lup_sum_cam = rup_sum_cam = 0;
				llw_sum_cam = rlw_sum_cam = 0;
				for(i=0;i<128;++i){
					up_sum_cam += up_cam[i]/10;
					lw_sum_cam += lw_cam[i]/10;
					if(i<24){
						lup_sum_cam += up_cam[i]/10;
						llw_sum_cam += lw_cam[i]/10;
						//m_sum_cam += lw_cam[i]/10;
					}
					if(i>100){
						rup_sum_cam += up_cam[i]/10;
						rlw_sum_cam += lw_cam[i]/10;
						//m_sum_cam += lw_cam[i]/10;
					}
					else{
						//m_sum_cam += up_cam[i]/10;
					}
					//sci_printf(" %d", cam[i]);
				}
				
				//making slesh hold for maker
				l_mkr_cam = llw_sum_cam + ((lup_sum_cam - llw_sum_cam)*1)/3;
				r_mkr_cam = rlw_sum_cam + ((rup_sum_cam - rlw_sum_cam)*1)/3;
				cross_cam = up_sum_cam - ((up_sum_cam - lw_sum_cam)*1)/5;  
				
				sci_printf("sum : %d  up : %d %d  lw : %d %d\r\n",cross_cam,lup_sum_cam,rup_sum_cam,llw_sum_cam,rlw_sum_cam);
			}
		}
		servo(400, 0);
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