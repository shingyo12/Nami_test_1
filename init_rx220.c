#include "init_rx220.h"
#include "iodefine.h"
#include "define_mouse.h"

void init_rx220(void){
	SYSTEM.PRCR.WORD 	= 0xA50B;		//Write protect ALL OFF
	SYSTEM.SCKCR.BIT.ICK 	= 0x0;			//ICLK divided by 1
	SYSTEM.SCKCR.BIT.PCKB 	= 0x0;			//PCLKB divided by 1
	SYSTEM.SCKCR.BIT.PCKD 	= 0x0;			//PCLKD divided by 1
	SYSTEM.SCKCR3.BIT.CKSEL = 0x1; 			//HOCO selected
	SYSTEM.HOCOCR.BIT.HCSTP	= 0x0;			//HOCO ON
	SYSTEM.HOCOCR2.BIT.HCFRQ	= 0x0;		//HOCO 32MHz
	SYSTEM.HOCOPCR.BIT.HOCOPCNT	= 0x0;		//HOCO power ON
	 
	//Standby OFF
	MSTP(MTU0) = 0;
	MSTP(SCI1) = 0;
	MSTP(S12AD) = 0;
	 
	//I/O Setting
	PORTE.PDR.BYTE = 0x3f;		//PORTE bit1,2,4,5: Output
	PORT5.PDR.BYTE = 0x30;		//PORT5 bit4,5: Output
	PORT1.PDR.BYTE = 0xf0;		//PORT1 bit4,5,6,7: Output
	PORT2.PDR.BYTE = 0x80;		//PORT2 bit7: Output
	PORT3.PDR.BYTE = 0x06;		//PORT3 bit1,2: Output
	PORTH.PDR.BYTE = 0x0f;		//PORTH bit1,2,3,4: Output
	PORTB.PDR.BYTE = 0xff;		//PORTB bit1-7: Output
	PORTA.PDR.BYTE = 0x00;		//PORTA bit1-7: Input
	
	PORT4.PDR.BIT.B4 = 0;		//PORT4 bit4: Input switch
	PORT4.PDR.BIT.B6 = 0;		//PORT4 bit6: Input switch
	//PORTH.PDR.BIT.B0 = 0;		//PORTH bit0: Output cammera
	//PORTH.PDR.BIT.B1 = 0;		//PORTH bit1: Output cammera
	
	PORT4.PDR.BIT.B0 = 0;		//PORTB bit0: Input encoder
	PORT4.PDR.BIT.B1 = 0;		//PORTB bit1: Input encoder
	PORT4.PDR.BIT.B5 = 0;		//PORTB bit5: Input encoder
	PORT4.PDR.BIT.B6 = 0;		//PORTB bit6: Input encoder
	PORT4.PDR.BIT.B3 = 1;		//PORTB bit3: Output speaker
	
	PORTE.PODR.BYTE = 0x00;		//PORTE bit1,4,5: Low
	PORT5.PODR.BYTE = 0x00;		//PORT5 bit4,5: Low
	PORT1.PODR.BYTE = 0x00;		//PORT1 bit4,5,6,7: Low
	PORT2.PODR.BYTE = 0x00;		//PORT2 bit7: Low
	PORT3.PODR.BYTE = 0x00;		//PORT3 bit1,2: Low
	PORTH.PODR.BYTE = 0x00;		//PORTH bit1,2,3,4: Low
	PORTB.PODR.BYTE = 0x00;		//PORTB bit1-7: Low
	
	PORTE.PMR.BYTE = 0x00;		//PORTE bit1,4,5: I/O
	PORT5.PMR.BYTE = 0x00;		//PORT5 bit4,5: I/O
	PORT1.PMR.BYTE = 0x00;		//PORT1 bit4,5,6,7: I/O
	PORT2.PMR.BYTE = 0x00;		//PORT2 bit7: I/O
	PORT3.PMR.BYTE = 0x00;		//PORT3 bit1,2: I/O
	PORT4.PMR.BYTE = 0x00;		//PORT4 bit1,2: I/O
	PORTH.PMR.BYTE = 0x00;		//PORTH bit1,2,3,4: I/O
	PORTB.PMR.BYTE = 0x00;		//PORTB bit1-7: I/O
	
	PORT2.PMR.BIT.B6 = 1; 		//PORT2 bit6: SCI TXD1
	PORTE.PMR.BIT.B1 = 1; 		//PORTE bit1: MTIOC4C
	PORTE.PMR.BIT.B2 = 1; 		//PORTE bit2: MTIOC4A
	
	PORTB.PMR.BIT.B3 = 0;		//spk
	
	//MPC setting
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.P26PFS.BYTE = 0xa; 		//P26 TXD1
	MPC.PE1PFS.BYTE = 0x1; 		//PE1 MTIOC4C
	MPC.PE2PFS.BYTE = 0x1; 		//PE2 MTIOC4A
	MPC.PA1PFS.BYTE = 0x2;		//MTCLK
	MPC.PA3PFS.BYTE = 0x2;		//MTCLK
	MPC.PA4PFS.BYTE = 0x2;		//MTCLK
	MPC.PA6PFS.BYTE = 0x2;		//MTCLK
	//MPC.PE1PFS.BIT.PSEL = 1;		//MTIOC0Bとして使用する
	//MPC.PE3PFS.BIT.PSEL = 1;		//MTIOC0Bとして使用する
	
	//MTU2a Setting
	MTU1.TMDR.BIT.MD = 0x4;	//MTCLK mode1
	MTU2.TMDR.BIT.MD = 0x4;	//MTCLK mode1
	
	MTU3.TCR.BIT.TPSC = 0x2;	//PCLK/16
	//MTU3.TCR.BIT.TPSC = 2;
	MTU3.TCR.BIT.CCLR = 0x2;	//Clear TGRB compare match
	//MTU3.TCR.BIT.CCLR = 0x1;	//Clear TGRA compare match
	MTU3.TGRA = (204-1);		//204 102us
	MTU3.TGRB = (250-1);		//250 125us interrupt period
	//MTU3.TGRB = 32*1000/16-1;
	//MTU3.TGRB = 50000-1;
	MTU3.TIER.BIT.TGIEA = 1;	//Enable interrupt from TGIA
	MTU3.TIER.BIT.TGIEB = 1;	//Enable interrupt from TGIB
	
	MTU.TSTR.BIT.CST4 = 0;		//MTU4タイマー停止
	MTU.TSYR.BIT.SYNC4 = 0; 
	MTU4.TCR.BIT.TPSC = 0x0;	//PCLK/1
	MTU4.TCR.BIT.CKEG = 0; //立ち上がりエッジでカウントする
	MTU4.TCR.BIT.CCLR = 0x02;	//Clear TGRB compare match
	MTU4.TMDR.BIT.MD = 0x02;	//PWM mode1
	MTU4.TMDR.BIT.BFA = 0;                  //TGRAとTGRCを通常動作させる
	MTU4.TMDR.BIT.BFB = 0;                  //TGRBとTGRDを通常動作させる
	MTU.TOER.BIT.OE4A = 1;		//MTIOC4A Output
	MTU.TOER.BIT.OE4C = 1;		//MTIOC4C Output
	//MTU4.TIORH.BIT.IOA = 2;		//初期出力L,コンペアマッチでH
	//MTU4.TIORH.BIT.IOB = 1;		//初期出力L,コンペアマッチでL
	MTU4.TIORH.BYTE = 0x12;		//initial Low, compare match High
	MTU4.TIORL.BYTE = 0x12;		//initial Low, compare match High 
	MTU4.TGRA = (320-1);		//Duty MTIOC4A
	MTU4.TGRB = (320-1);		//PWM period
	MTU4.TGRC = (320-1);		//Duty MTIOC4C
	MTU4.TGRD = (320-1);		//PWM period
	MTU4.TIER.BIT.TGIEB = 1;	//Enable interrupt from TGIB
	MTU4.TCNT = 0x0000;                     //カウント値の初期化
	MTU.TSTR.BIT.CST4 = 1;                  //MTU0のカウントを開始する
	
	/*MSTP_MTU0 = 0;				//MTU0スタンバイ解除
	MTU.TSTR.BIT.CST0 = 0;			//タイマー停止
	MTU0.TCR.BIT.TPSC = 3;			//動作周波数設定(CLOCK/64)
	MTU0.TCR.BIT.CCLR = 2;			//TGRBのコンペアマッチでTCNTクリア
	MTU0.TMDR.BIT.MD = 0;			//通常動作に設定
	MTU0.TIORH.BIT.IOB = 3;			//初期出力Lコンペアマッチでトグル
	MTU0.TGRB = 0;*/
		
	//SCI
	SCI1.SMR.BYTE = 0x00;		//PCLK/1, stopbit=1, no parity bit 
	SCI1.SEMR.BIT.ABCS = 1;		//1bit = 8clock cicle
	SCI1.BRR = 51;			//baudrate 38462bps
	SCI1.SCR.BYTE = 0x20;		//TE=1
	
	//ADC
	S12AD.ADCER.BIT.ADRFMT = 0;		//ADC data right justified
	S12AD.ADCSR.BIT.EXTRG = 1;		//Asynchronous
	S12AD.ADCSR.BIT.TRGE = 1;		//Enable ADC start by Trigger
	
	//Interrupt Setting
	IEN(MTU3,TGIA3) = 1;		//Enable MTU3 TGIB interrupt
	IPR(MTU3,TGIA3) = 10;		//MTU3 Priority is 10
	IEN(MTU3,TGIB3) = 1;		//Enable MTU3 TGIB interrupt
	IPR(MTU3,TGIB3) = 10;		//MTU3 Priority is 10
	IEN(MTU4,TGIB4) = 1;		//Enable MTU3 TGIB interrupt
	IPR(MTU4,TGIB4) = 9;		//MTU3 Priority is 10
	
	MTU.TSTR.BIT.CST1 = 1;		//MTU1 Timer start
	MTU.TSTR.BIT.CST2 = 1;		//MTU2 Timer start
	MTU.TSTR.BIT.CST3 = 1;		//MTU3 Timer start
	MTU.TSTR.BIT.CST4 = 1;		//MTU4 Timer start
	
	LED_7SEG_1 = CHIP_LED_OFF;
	LED_7SEG_2 = CHIP_LED_OFF;
	LED_7SEG_3 = CHIP_LED_OFF;
	LED_7SEG_4 = CHIP_LED_OFF;
	LED_7SEG_5 = CHIP_LED_OFF;
	LED_7SEG_6 = CHIP_LED_OFF;
	LED_7SEG_7 = CHIP_LED_OFF;
	
	LED_1 = CHIP_LED_OFF;
	LED_2 = CHIP_LED_OFF;
	
	MOT_R_IN1 = 1;
	MOT_R_IN2 = 0;
	MOT_L_IN1 = 1;
	MOT_L_IN2 = 0;
	
	PORT4.PCR.BIT.B4=1;
	PORT4.PCR.BIT.B6=1;

}

