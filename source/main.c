
#include "ddl.h"
#include "led.h"
#include "bsp_uart.h"
#include "key.h"
#include "ld_pwm.h"
#include "bsp_adc.h"
#include "bsp_current_signal.h"
#include "bsp_output_signal.h"
#include "bsp_timer.h"
#include "data_save.h"
#define MAX_DISTANCE_DETECT		2000
#define MIN_DETECT_VOL     1070 // 5CM 对应865mv(透明体-烟盒塑料)
//#define MIN_DETECT_VOL     1427 // 50CM 对应1150mv(无遮挡)
//#define MIN_DETECT_VOL     1961 // 50CM 对应1580mv(透明体-烟盒塑料)

//#define MAX_DETECT_VOL 	   2500 // 2M 对应2500mv(透明体-烟盒塑料)(待测)
#define MAX_DETECT_VOL     1961 // 50CM 对应1580mv(透明体-烟盒塑料)

#define NC_OUT	 	1
#define NO_OUT 		0

#define DIS_SET_WAIT 0
#define DIS_SET_OK 	 1
#define DIS_SET_FAIL 2

char 		dis_set_state = 0;
char 		led_sync = 1; //led 闪烁同步1，异步0
char 		timer_hz = 0; //led 闪烁频率
char 		object_detected = 0;


char 		buf[256]={0}; //buf to uart 

char		no_nc_state = NC_OUT;//
uint16_t	object_detect_vol = 0;
uint16_t 	u16Adc_ch8_Result = 0;		
uint16_t	adc_ch3_result = 0;
uint16_t	adc_ch5_result = 0;
uint16_t	detect_distance	= MAX_DISTANCE_DETECT;
uint16_t	object_vol_min  = MAX_DETECT_VOL;
uint16_t	object_vol_max  = MAX_DETECT_VOL;

uint32_t 	add_detect_vol_min =  0x3f00;
uint32_t	add_no_nc_state =     0x3f04;



//将时钟从RCH4MHz切换至RCH24MHz,
void App_Rch4MHzTo24MHz(void)
{
	stc_sysctrl_clk_cfg_t pstcCfg;

	///<============== 将时钟从RCH4MHz切换至RCH24MHz ==============================
	///< RCH时钟不同频率的切换，需要先将时钟切换到RCL，设置好频率后再切回RCH
	Sysctrl_SetRCLTrim(SysctrlRclFreq32768);
	Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE);
	Sysctrl_SysClkSwitch(SysctrlClkRCL);

	///< 加载目标频率的RCH的TRIM值
	Sysctrl_SetRCHTrim(SysctrlRchFreq24MHz);
	///< 使能RCH（默认打开，此处可不需要再次打开）
	//Sysctrl_ClkSourceEnable(SysctrlClkRCH, TRUE);
	///< 时钟切换到RCH
	Sysctrl_SysClkSwitch(SysctrlClkRCH);
	///< 关闭RCL时钟
	Sysctrl_ClkSourceEnable(SysctrlClkRCL, FALSE);

	delay1ms(3);

	pstcCfg.enClkSrc = SysctrlClkRCH;
	pstcCfg.enHClkDiv = SysctrlHclkDiv1;
	pstcCfg.enPClkDiv = SysctrlPclkDiv1;

	Sysctrl_ClkInit(&pstcCfg);
}

void pnp_output(void)
{
	if( object_detected )
	{
		signal_output_low_level();
	}else
	{
		signal_output_high_level();
	}
}


//检测物体的LED输出
void act_led_output(void)
{
	if(object_detected)
	{
		if (no_nc_state==NO_OUT)
		{
			act_led_on();
		}else if(no_nc_state == NC_OUT)
		{
			act_led_off();
		}	
	}else
	{
		if (no_nc_state==NO_OUT)
		{
			act_led_off();
		}else if(no_nc_state == NC_OUT)
		{
			act_led_on();
		}	
	}
}


void func_led_output(void)
{	
	if(led_sync)
	{
		if (timer_cnt%2)
		{
			act_led_on();
			power_led_on();
		}else 
		{
			act_led_off();
			power_led_off();
		}	
	}else
	{
		if (timer_cnt%2)
		{
			act_led_on();
			power_led_off();
		}else 
		{
			act_led_off();
			power_led_on();			
		}	
	}
}

//物体检测函数
void object_detect(void)
{
	static char cnt = 0;
	if( u16Adc_ch8_Result >object_vol_min )
	{
		cnt ++;
		if(cnt>3)
		{		
//			sprintf(buf,"detected cnt:%d \n\r",  cnt);
//			uart0_send_data((unsigned char *)buf, strlen(buf));
			object_detected = 1;
			return ;
		}
	}
	else
	{
		cnt = 0;
		object_detected = 0;
	}	
	return;
}


//设置检测距离 
char detect_dis_set()
{
	static char cnt = 0;
	if( u16Adc_ch8_Result < object_vol_max )
	{
		cnt ++;
		if(cnt>3)
		{
			
			object_vol_min = u16Adc_ch8_Result+50;
//			sprintf(buf,"set cnt:%d \n\r",  cnt);
//			uart0_send_data((unsigned char *)buf, strlen(buf));
			return 1;
		}
	}
	else
	{
		cnt = 0;
//		sprintf(buf,"set cnt:%d \n\r",  cnt);
//		uart0_send_data((unsigned char *)buf, strlen(buf));
		object_vol_min = MAX_DETECT_VOL;
		return 2;
	}	
	return 0;
}


void func_led_display(void)
{	
	if(is_timer_flag)
	{
		if (timer_cnt > cnt_2s)//2s后
		{
			timer_stop();
			last_key_func = KEY_FUNC_NULL;
			dis_set_state = DIS_SET_WAIT;
			act_led_off();
			power_led_off();
		}
		else
		{
			func_led_output();
		}
	}		
}


void current_protect(void)
{
	if( OVER_CURRENT_NO == over_current_dec())
	{
		signal_output_low_level();
	}else
	{
		signal_output_low_level();
	}
}

int main(void)
{	
	char key_func;
	App_Rch4MHzTo24MHz();
	all_led_init();
	uart0_init();
	key_init();
	adc_init();
	ld_pwm_init();
	output_signal_init();
	current_signal_init();
	while(Ok != Flash_Init(1))
    {
        check_update_data();
    }
	//1 flash存储按键设置的数据:产品检测距离、NO/NC状态
	//2 读取adc数据 对数据分析处理 返回处理结果
	//3 根据处理结果做出状态指示和信号输出空载

	while (1)
	{	
		current_protect();	//过流保护	
		key_func = get_key_state();
		//保持蓝光信号持续稳定的输出
//		if(last_key_func)
//		{
//			sprintf(buf,"main key_funct:%d \n\r",  last_key_func);
//			uart0_send_data((unsigned char *)buf, strlen(buf));
//		}

		switch (last_key_func)
		{
			case	KEY_FUNC_NULL://实现正常采集输出功能
				object_detect();	//ad通道采集
				power_led_on();
				//act_led_on();
				act_led_output();
				pnp_output();
				//PNP输出控制				
				break;
			case	KEY_CALIB_DIS:// 距离信号标定
							
				if(dis_set_state != DIS_SET_WAIT)
				{
					if(!is_timer_flag)
					{
						if(dis_set_state == DIS_SET_FAIL)
						{
							timer_start(8);
							led_sync = 0;
						}
						else if(dis_set_state == DIS_SET_OK)
						{
							timer_start(4);
							led_sync = 1;
						}
						write_data_to_flash(add_detect_vol_min,object_vol_min);	
					}
					else
					{
						func_led_display();
					}
				}else
				{
					dis_set_state = detect_dis_set();	
				}				
				
				break;
			case 	KEY_SWITCH_OC:// NO/NC状态切换
			   	if(!is_timer_flag)
				{
					timer_start(2);
					led_sync = 1;
					no_nc_state = !no_nc_state;
					write_data_to_flash(add_no_nc_state,no_nc_state);
				}
				else
				{
					func_led_display();
				}
				break;
			default:
				break;
		}
	}
}


