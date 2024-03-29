#include "bsp_adc.h"
#include "gpio.h"
#include "bgr.h"
#include "adc.h"
#include "bsp_uart.h"
stc_adc_cfg_t      stcAdcCfg;
stc_adc_norm_cfg_t stcAdcNormCfg; 


static void adc_pin_init(void)
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);    //GPIO ����ʱ��ʹ��

    Gpio_SetAnalogMode(GpioPort0, GpioPin2);//config P02 AIN8 
    Gpio_SetAnalogMode(GpioPort3, GpioPin3);//config P33 AIN3
	Gpio_SetAnalogMode(GpioPort3, GpioPin5);//config P35 AIN5
}

static void adc_cfg(void)
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE);  //ADCBGR ����ʱ��ʹ��
    
    //ADC����
    Adc_Enable();
    Bgr_BgrEnable();
   
    stcAdcCfg.enAdcOpMode = AdcNormalMode;          //���β���ģʽ
    stcAdcCfg.enAdcClkSel = AdcClkSysTDiv2;         //PCLK
    stcAdcCfg.enAdcSampTimeSel = AdcSampTime4Clk; //AdcSampTime8Clk;   //4������ʱ��
    stcAdcCfg.enAdcRefVolSel = RefVolSelAVDD;   	//�ο���ѹ:�ڲ�2.5V(avdd>3V,SPS<=200kHz)  SPS���� = ADCʱ�� / (����ʱ�� + 16CLK) 
    stcAdcCfg.bAdcInBufEn = FALSE;                  //��ѹ���������ʹ�ܣ�SPS�������� <=200K
    stcAdcCfg.u32AdcRegHighThd = 0u;                //�Ƚ���ֵ������
    stcAdcCfg.u32AdcRegLowThd  = 0u;                //�Ƚ���ֵ������
    stcAdcCfg.enAdcTrig0Sel    = AdcTrigDisable;    //ADCת���Զ���������
    stcAdcCfg.enAdcTrig1Sel    = AdcTrigDisable;
    Adc_Init(&stcAdcCfg);
}

//	
static void App_AdcInit(void)
{
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);    

    Gpio_SetAnalogMode(GpioPort0, GpioPin2);
    stc_adc_cfg_t             stcAdcCfg;
    stc_adc_cont_cfg_t        stcAdcContCfg;
    stc_adc_irq_t             stcAdcIrq;    
    
    DDL_ZERO_STRUCT(stcAdcCfg);
    DDL_ZERO_STRUCT(stcAdcContCfg);
    DDL_ZERO_STRUCT(stcAdcIrq);
     
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE);  
    
    Adc_Enable();
    Bgr_BgrEnable();
    
    stcAdcCfg.enAdcOpMode = AdcNormalMode;               
    stcAdcCfg.enAdcClkSel = AdcClkSysTDiv8;         
    stcAdcCfg.enAdcSampTimeSel = AdcSampTime8Clk;      
    stcAdcCfg.enAdcRefVolSel = RefVolSelAVDD;           
    stcAdcCfg.bAdcInBufEn = FALSE;                      
    stcAdcCfg.enAdcTrig0Sel = AdcTrigDisable;//AdcTrigPCA;          
    stcAdcCfg.enAdcTrig1Sel = AdcTrigPCA;
    Adc_Init(&stcAdcCfg);    
    
    stcAdcIrq.bAdcIrq    = FALSE;                            
    stcAdcIrq.bAdcRegCmp = FALSE;
    stcAdcIrq.bAdcHhtCmp = TRUE;
    stcAdcIrq.bAdcLltCmp = FALSE;
    Adc_CmpCfg(&stcAdcIrq);                              
	Adc_ClrContIrqState();
	Adc_ClrRegIrqState();
	Adc_ClrHhtIrqState();
	Adc_ClrIrqLltState();
    Adc_EnableIrq();                                     
    EnableNvic(ADC_IRQn, IrqLevel3, TRUE);
	
	stcAdcNormCfg.enAdcNormModeCh = AdcExInputCH8;
	stcAdcNormCfg.bAdcResultAccEn = FALSE;
	Adc_ConfigNormMode(&stcAdcCfg, &stcAdcNormCfg);
	
}

void adc_init(void)
{	
	App_AdcInit();
}

uint16_t get_adc_value(uint8_t channel)
{
	uint16_t u16AdcResult = 0;	
	if(channel >= AdcExInputCH0 && channel <= AdcVref1P2Input)
	{
		stcAdcNormCfg.enAdcNormModeCh = channel;
		stcAdcNormCfg.bAdcResultAccEn = FALSE;
		Adc_ConfigNormMode(&stcAdcCfg, &stcAdcNormCfg);

		Adc_Start();
		while(FALSE != Adc_PollBusyState());
		Adc_GetResult(&u16AdcResult);
		Adc_Stop();
	}
	return u16AdcResult;
}

///< ADC �жϷ������
extern char buf[10];
extern uint16_t u16Adc_ch8_Result;
void Adc_IRQHandler(void)
{	
    static char cnt = 0;	
    if (TRUE == M0P_ADC->IFR_f.REG_INTF)
    {
        Adc_ClrRegIrqState();
    }

    if (TRUE == M0P_ADC->IFR_f.HHT_INTF)
    {
        Adc_ClrHhtIrqState();
    }

    if (TRUE == M0P_ADC->IFR_f.LLT_INTF)
    {
        Adc_ClrIrqLltState();
    }

    if (TRUE == M0P_ADC->IFR_f.CONT_INTF)
    {
        Adc_ClrContIrqState();         
    }
	
	cnt++;
	if(cnt%2)
	{
		Adc_GetCH8Result(&u16Adc_ch8_Result);	
	}		
}
