#include "led.h"
#include "gpio.h"

#define POWER_LED_PORT		GpioPort2
#define POWER_LED_PIN		GpioPin6

//#define POWER_LED_PORT	GpioPort0
//#define POWER_LED_PIN		GpioPin3

#define ACT_LED_PORT		GpioPort1
#define ACT_LED_PIN			GpioPin4

void power_led_init(void)
{			
	stc_gpio_cfg_t cfg;
	cfg.bOutputVal = FALSE;
	cfg.enDir =  GpioDirOut;
	cfg.enDrv = GpioDrvH;
	cfg.enPu = GpioPuDisable;
	cfg.enPd = GpioPdDisable;
	cfg.enOD = GpioOdDisable;

	Gpio_Init(POWER_LED_PORT, POWER_LED_PIN, &cfg);	
	Gpio_WriteOutputIO(POWER_LED_PORT, POWER_LED_PIN, TRUE);
}

void power_led_on(void)
{
	Gpio_WriteOutputIO(POWER_LED_PORT, POWER_LED_PIN, FALSE);
}

void power_led_off(void)
{
	Gpio_WriteOutputIO(POWER_LED_PORT, POWER_LED_PIN, TRUE);
}

void act_led_init(void)
{
	stc_gpio_cfg_t cfg;
	cfg.bOutputVal = FALSE;
	cfg.enDir =  GpioDirOut;
	cfg.enDrv = GpioDrvH;
	cfg.enPu = GpioPuDisable;
	cfg.enPd = GpioPdDisable;
	cfg.enOD = GpioOdDisable;

	Gpio_Init(ACT_LED_PORT, ACT_LED_PIN, &cfg);	
	Gpio_WriteOutputIO(ACT_LED_PORT, ACT_LED_PIN, TRUE);
}

void act_led_on(void)
{
	Gpio_WriteOutputIO(ACT_LED_PORT, ACT_LED_PIN, FALSE);
}

void act_led_off(void)
{
	Gpio_WriteOutputIO(ACT_LED_PORT, ACT_LED_PIN, TRUE);
}

void all_led_init(void)
{
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

	power_led_init();
	act_led_init();
}



