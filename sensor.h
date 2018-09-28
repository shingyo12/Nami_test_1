#ifndef __SENSOR_HEADER__
#define __SENSOR_HEADER__

void set_sen_value(char position, char led_state, int ad_value);
void calc_sen_value(char position);
float get_sen_value(char position);

enum SEN_POSITION{
	LF_SEN,
	LS_SEN,
	RS_SEN,
	RF_SEN
};

enum LED_STATUS{
	LED_ON,
	LED_OFF
};


#endif

