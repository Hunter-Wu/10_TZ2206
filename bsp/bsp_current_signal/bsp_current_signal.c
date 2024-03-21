#include "bsp_current_signal.h"
#include "gpio.h"


#define CURRENT_SIGNAL_PORT		GpioPort1
#define CURRENT_SIGNAL_PIN		GpioPin5


void current_signal_init(void)
{
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

	stc_gpio_cfg_t cfg;
	cfg.enDir =  GpioDirIn;
	cfg.enDrv = GpioDrvH;
	cfg.enPu = GpioPuEnable;
	cfg.enPd = GpioPdDisable;
	cfg.enOD = GpioOdDisable;

	Gpio_Init(CURRENT_SIGNAL_PORT, CURRENT_SIGNAL_PIN, &cfg);

}


over_current_signal_t over_current_dec(void)
{
	return Gpio_GetInputIO(CURRENT_SIGNAL_PORT, CURRENT_SIGNAL_PIN) ? OVER_CURRENT_YES : OVER_CURRENT_NO;
}