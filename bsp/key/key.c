#include "key.h"
#include "bsp_uart.h"
#include "bsp_timer.h"
char 		last_key_func;
void key_init(void)
{
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
	stc_gpio_cfg_t cfg;
	cfg.enDir =  GpioDirIn;
	cfg.enDrv =  GpioDrvH;
	cfg.enPu  = GpioPuEnable;
	cfg.enPd  = GpioPdDisable;
	cfg.enOD  = GpioOdDisable;

	Gpio_Init(SET_KEY_PORT, SET_KEY_PIN, &cfg);	
}


/*
 * get_key_state 
 * 
*/
char get_key_state()
{	char buf[50]={0};
	static uint8_t cycle = 0;
	char key_func = KEY_FUNC_NULL;
	if(FALSE == Gpio_GetInputIO(SET_KEY_PORT, SET_KEY_PIN))//key press
	{	
		cycle++;
		if(cycle < 240u) //10ms
		{		
			return key_func;
		}else
		{
			if(!is_timer_flag)
			{
				timer_start(2);
			}		
		}
	}
	else //key null
	{  
		cycle = 0;
		if(is_timer_flag && last_key_func==KEY_FUNC_NULL)
		{				
			if( timer_cnt <  KEY_PRESS_2S || timer_cnt > KEY_PRESS_8S  )
			{
				key_func = KEY_FUNC_NULL;
			}
			else if( timer_cnt >=  KEY_PRESS_5S && timer_cnt < KEY_PRESS_8S  )
			{
				key_func = KEY_SWITCH_OC;
			}
			else if( timer_cnt >=  KEY_PRESS_2S && timer_cnt < KEY_PRESS_5S  )
			{
				key_func = KEY_CALIB_DIS;
			}
			last_key_func = key_func;
			timer_stop();				
		}
	}
//	memset(buf,0,sizeof(buf));
//	sprintf(buf,"key.c key_press_cnt:%d \n\r",key_func);
//	uart0_send_data((unsigned char *)buf, strlen(buf));			
	return key_func;	
}


char key_scan(void)
{
	char ret = KEY_PRESS_NONE;
	char trig=0, read_data=0;
	static char cnt=0, press_flag = 0;
	static unsigned int cycle = 0,long_press_time = 0,press_cnt = 0,press_release_time = 0;


	cycle++;
	if(cycle%2400u != 0) //10ms
	{
		return ret;
	}
	else
	{
		cycle = 0;
	}

	if(FALSE == Gpio_GetInputIO(SET_KEY_PORT, SET_KEY_PIN))
	{
		read_data = 1;
	}
	else
	{
		read_data = 0;
	}

	trig = read_data & (read_data ^ cnt);
	cnt = read_data;

	if(1 == trig)
	{
		press_flag = 1;
		long_press_time = 0;
	}

	if(1 == cnt)
	{
		long_press_time++;
	}

	if(0 == trig && 0 == cnt)
	{
		if(1 == press_flag)
		{
			press_cnt++;					
	
		
			if(long_press_time > 200)
			{
				ret = long_press_time / 100;
				if(ret > KEY_LONG_PRESS_10S)
				{
						ret = KEY_LONG_PRESS_10S;
				}
				
				press_cnt = 0;
				press_release_time = 0;
			}
			else
			{
				if(2 == press_cnt)
				{
					press_cnt = 0;
					press_release_time = 0;
					ret = KEY_DOUBLE_CLICK;
				}
			}
			
			press_flag = 0;
		}
	}

	if(press_cnt == 1)
	{
		press_release_time++;

		if(press_release_time > 30)
		{
			press_cnt = 0;
			press_release_time = 0;
			ret = KEY_CLICK;
		}
	}

	return ret;
}

