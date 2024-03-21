#ifndef __KEY_H
#define __KEY_H
#include "gpio.h"


#define SET_KEY_PORT		GpioPort2
#define SET_KEY_PIN			GpioPin5

enum
{
	//KEY_PRESS_NONE = 0,
	KEY_LONG_PRESS_1S = 1,
	KEY_LONG_PRESS_2S = 2, //����2�� ��ͬ,����n��...
	KEY_LONG_PRESS_3S = 3,
	KEY_LONG_PRESS_4S = 4,
	KEY_LONG_PRESS_5S = 5,
	KEY_LONG_PRESS_6S = 6,
	KEY_LONG_PRESS_7S = 7,
	KEY_LONG_PRESS_8S = 8,
	KEY_LONG_PRESS_9S = 9,
	KEY_LONG_PRESS_10S = 10,
	KEY_DOUBLE_CLICK = 11, //˫��
	KEY_CLICK = 12, //����
};

typedef enum
{
	KEY_PRESS_NONE = 0,
	KEY_PRESS = 1,
	KEY_WAIT = 2,
}KEY_PRESS_STATE;


// 	case TIME_2_HZ://2Hz:250ms
// 		cnt_2s = 8;

typedef enum
{
	KEY_PRESS_2S = 8, 
	KEY_PRESS_5S = 20,
	KEY_PRESS_8S = 32,	
}KEY_PRESS_TIME;

typedef enum
{
	KEY_FUNC_NULL 	= 0,
	KEY_CALIB_DIS 	= 1, 
	KEY_SWITCH_OC 	= 2,
}KEY_FUNC;

extern  char last_key_func;
void key_init(void);
char get_key_state();
char key_scan(void);
#endif //__KEY_H

