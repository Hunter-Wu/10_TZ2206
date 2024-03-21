#ifndef __BSP_CURRENT_SIGNAL_H
#define __BSP_CURRENT_SIGNAL_H


typedef enum
{
	OVER_CURRENT_NO = 0,
	OVER_CURRENT_YES = 1
}over_current_signal_t;


void current_signal_init(void);
over_current_signal_t over_current_dec(void);


#endif //__BSP_CURRENT_SIGNAL_H


