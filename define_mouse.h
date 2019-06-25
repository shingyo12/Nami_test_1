#ifndef __DEFINEHEADER_HEADER__
#define __DEFINEHEADER_HEADER__

#include "iodefine.h"

#define LED_1		PORT5.PODR.BIT.B4
#define LED_2		PORT5.PODR.BIT.B5

#define LED_7SEG_1	PORT3.PODR.BIT.B2
#define LED_7SEG_2	PORT3.PODR.BIT.B1
#define LED_7SEG_3	PORT1.PODR.BIT.B4
#define LED_7SEG_4	PORT1.PODR.BIT.B5
#define LED_7SEG_5	PORT1.PODR.BIT.B6
#define LED_7SEG_6	PORT1.PODR.BIT.B7
#define LED_7SEG_7	PORT2.PODR.BIT.B7

#define MOT_R_IN1	PORTB.PODR.BIT.B0
#define MOT_R_IN2	PORTB.PODR.BIT.B1
#define MOT_L_IN1	PORTB.PODR.BIT.B5
#define MOT_L_IN2	PORTB.PODR.BIT.B6
#define MOT_STBY	PORTB.PODR.BIT.B7
#define BUZZ_OUT	PORTB.PODR.BIT.B3

#define CHIP_LED_ON 0
#define CHIP_LED_OFF 1

#define SW_MODE		PORT4.PIDR.BIT.B6
#define SW_START	PORT4.PIDR.BIT.B4

#define CAM_CLK		PORTH.PODR.BIT.B0
#define CAM_SI		PORTH.PODR.BIT.B1
#define CAM_AOO		S12AD.ADDR0

#define SW_ON	0
#define SW_OFF	1

#define RL 24		//L wheel
#define RR 24		//R wheel
#define WB 98		//wheel base
#define GEAR 18/42;
#define KP 1
#define KD 1

#define R_ENC 		MTU1.TCNT
#define L_ENC 		MTU2.TCNT

#define PI 3.1415926535
#define DEG2RAD PI/180

//extern short int move=0;

#endif