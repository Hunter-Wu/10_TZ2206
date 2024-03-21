#include "bsp_timer.h"
#include "bt.h"
#include "bsp_uart.h"

extern char buf[50];	
char cnt_2s = 0;
char timer_cnt = 0;//timer irq cnt 
char is_timer_flag = 0;
//led闪烁时使用Timer0 定时 
void timer0_timer_init(char hz)
{
	stc_bt_cfg_t btime_cfg;
	uint16_t clkDiv = 0;
	uint16_t u16ArrData = 0;
	uint16_t u16InitCntData = 0;
	DDL_ZERO_STRUCT(btime_cfg);
	switch (hz)
	{
		case TIME_8_HZ://8hz:62.5 ms
			clkDiv = BtPCLKDiv32;
			//  65535 - 24000/32*62.5 =  65535-46875 = 18660
			u16ArrData = 18660;
			u16InitCntData =18660;
			cnt_2s = 32;
			break;
		case TIME_4_HZ://4Hz:125ms;
			//  65535 - 24000/64*125 =  65535-46875 = 18660
			clkDiv = BtPCLKDiv64;
			u16ArrData = 18660;
			u16InitCntData =18660;
			cnt_2s = 16;
			break;
		case TIME_2_HZ://2Hz:250ms
			//  65535 - 24000/256*250 =  42098
			clkDiv = BtPCLKDiv256;
			u16ArrData = 42098;
			u16InitCntData = 42098;
			cnt_2s = 8;
			break;
		default:
			break;
	}
	btime_cfg.enPRS = clkDiv; 
	btime_cfg.enCT  = BtTimer;//定时器
	btime_cfg.enMD  = BtMode2;//16bit 	
	Bt_Init(TIM0,&btime_cfg); 
	//TIM1中断使能
    Bt_ClearIntFlag(TIM0);
    Bt_EnableIrq(TIM0);
    EnableNvic(TIM0_IRQn, IrqLevel3, TRUE); 
	Bt_ARRSet(TIM0, u16ArrData);
    Bt_Cnt16Set(TIM0, u16InitCntData);
}

void timer_start(char hz)
{
	memset(buf,0,sizeof(buf));
	sprintf(buf,"time_start  hz= %d \n\r",hz);
	uart0_send_data((unsigned char *)buf, strlen(buf));
	timer0_timer_init(hz);
    Bt_Run(TIM0);
    timer_cnt = 0;
    is_timer_flag = 1;
}

void timer_stop(void)
{
    Bt_Stop(TIM0);
    timer_cnt= 0;
	Bt_DisableIrq(TIM0);
    is_timer_flag =0;
	memset(buf,0,sizeof(buf));
	sprintf(buf,"time_stop is_timer_flag %d\n\r",is_timer_flag);
	uart0_send_data((unsigned char *)buf, strlen(buf));
}

#include "bsp_uart.h"
void Tim0_IRQHandler(void)
{	
	Bt_ClearIntFlag(TIM0);
	timer_cnt++;
//	sprintf(buf,"timer_cnt %d  \n\r",timer_cnt);
//	uart0_send_data((unsigned char *)buf, strlen(buf));
}
