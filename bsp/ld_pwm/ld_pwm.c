#include "ld_pwm.h"
#include "lpm.h"
#include "pca.h"
#include "gpio.h"


#define PWM_PORT		GpioPort2
#define PWM_PIN			GpioPin4

uint16_t pulse = 9;

uint16_t pulse_cir = 300;//249;//100us;200;//80us;

static void pwm_pin_init(void)
{	
	stc_gpio_cfg_t    GpioInitStruct;
	DDL_ZERO_STRUCT(GpioInitStruct);

	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

	//P24
	GpioInitStruct.enDrv  = GpioDrvH;
	GpioInitStruct.enDir  = GpioDirOut;
	Gpio_Init(PWM_PORT, PWM_PIN, &GpioInitStruct);
	Gpio_SetAfMode(PWM_PORT, PWM_PIN, GpioAf4);
}

static void pca_init(void)
{
	stc_pcacfg_t  PcaInitStruct;
	uint16_t duty_val;

	Sysctrl_SetPeripheralGate(SysctrlPeripheralPca, TRUE);
	
	PcaInitStruct.pca_clksrc = PcaPclkdiv8;
	PcaInitStruct.pca_cidl   = FALSE;
	#if 0
	
	PcaInitStruct.pca_ecom   = PcaEcomEnable;       //允许比较器功能
	PcaInitStruct.pca_capp   = PcaCappDisable;      //禁止上升沿捕获
	PcaInitStruct.pca_capn   = PcaCapnDisable;      //禁止下降沿捕获
	PcaInitStruct.pca_mat    = PcaMatDisable;        //使能匹配功能
	PcaInitStruct.pca_tog    = PcaTogDisable;        //使能翻转控制功能
	PcaInitStruct.pca_pwm    = PcaPwm8bitEnable;    //使能PWM控制输出
	//设置占空比
	

	duty_val = 255 - pulse;
	PcaInitStruct.pca_ccaph = duty_val;
	PcaInitStruct.pca_ccapl = duty_val;
	#else
	
	PcaInitStruct.pca_ecom   = PcaEcomEnable;       //允许比较器功能
	PcaInitStruct.pca_capp   = PcaCappDisable;      //禁止上升沿捕获
	PcaInitStruct.pca_capn   = PcaCapnDisable;      //禁止下降沿捕获
	PcaInitStruct.pca_mat    = PcaMatEnable;        //使能匹配功能
	PcaInitStruct.pca_tog    = PcaTogEnable;        //使能翻转控制功能
	PcaInitStruct.pca_pwm    = PcaPwm8bitDisable;   //使能PWM控制输出
	
	pulse = pulse_cir - pulse;

	PcaInitStruct.pca_ccap = pulse;
	
	#endif
	

	Pca_M1Init(&PcaInitStruct);
	Pca_ClrItStatus(PcaCcf1);
	
    EnableNvic(PCA_IRQn, IrqLevel3, TRUE);
    Pca_ConfModulexIt(PcaModule1, TRUE);
    Pca_ConfPcaIt(TRUE); 
}


void ld_pwm_init(void)
{
  pwm_pin_init();
  pca_init(); 
  Pca_StartPca(TRUE);
}

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
 /**
******************************************************************************
    ** \brief  
    ** 
  ** @param  ?
    ** \retval ?
    **
******************************************************************************/ 

void Pca_IRQHandler(void)
{
	pulse = pulse_cir - pulse;
	Pca_StartPca(FALSE);
	Pca_SetCnt(0);
	Pca_SetCcap(PcaModule1,pulse);
    if(Pca_GetItStatus(PcaCcf1) != FALSE)
    {
        Pca_ClrItStatus(PcaCcf1);
    }
    else if(Pca_GetItStatus(PcaCf) != FALSE)
    {
        Pca_ClrItStatus(PcaCf);    
    }
	Pca_StartPca(TRUE);
}
