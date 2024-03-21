#include "bsp_output_signal.h"
#include "gpio.h"


#define CONTROL_SIGNAL_PORT		GpioPort2
#define CONTROL_SIGNAL_PIN		GpioPin3


void output_signal_init(void)
{
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

	stc_gpio_cfg_t cfg;
	cfg.bOutputVal = FALSE;
	cfg.enDir =  GpioDirOut;
	cfg.enDrv = GpioDrvH;
	cfg.enPu = GpioPuDisable;
	cfg.enPd = GpioPdDisable;
	cfg.enOD = GpioOdDisable;

	Gpio_Init(CONTROL_SIGNAL_PORT, CONTROL_SIGNAL_PIN, &cfg);	
	Gpio_WriteOutputIO(CONTROL_SIGNAL_PORT, CONTROL_SIGNAL_PIN, TRUE);

}

void signal_output_low_level(void)
{
	Gpio_WriteOutputIO(CONTROL_SIGNAL_PORT, CONTROL_SIGNAL_PIN, TRUE);
}

void signal_output_high_level(void)
{
	Gpio_WriteOutputIO(CONTROL_SIGNAL_PORT, CONTROL_SIGNAL_PIN, FALSE);
}
