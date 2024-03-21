#include "bsp_uart.h"
#include "uart.h"
#include "bt.h"
#include "ddl.h"
#include "gpio.h"


// UART0 中断服务函数
void Uart1_IRQHandler(void)
{
    char u8RxData[2];

    if(TRUE == Uart_GetStatus(M0P_UART1, UartRC))
    {
        Uart_ClrStatus(M0P_UART1, UartRC);

        u8RxData[1] = Uart_ReceiveData(M0P_UART1);
    }

}

static void uart0_port_init(void)
{
    stc_gpio_cfg_t stcGpioCfg;

    DDL_ZERO_STRUCT(stcGpioCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); //使能GPIO模块时钟

    ///<TX
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPort3, GpioPin2, &stcGpioCfg);
    Gpio_SetAfMode(GpioPort3, GpioPin2, GpioAf5);        //配置P32 端口为URART1_TX

    ///<RX
    stcGpioCfg.enDir = GpioDirIn;				
    Gpio_Init(GpioPort3, GpioPin6, &stcGpioCfg);
    Gpio_SetAfMode(GpioPort3, GpioPin6, GpioAf1); 		//配置P36 端口为URART1_RX
}

static void uart0_baud_cfg(void)
{
    uint16_t timer=0;

    stc_uart_baud_cfg_t stcBaud;
    stc_bt_cfg_t stcBtCfg;

    DDL_ZERO_STRUCT(stcBaud);
    DDL_ZERO_STRUCT(stcBtCfg);

    //外设时钟使能
    Sysctrl_SetPeripheralGate(SysctrlPeripheralBt,TRUE);//模式0/2可以不使能
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE);

    stcBaud.bDbaud  = 0u;//双倍波特率功能
    stcBaud.u32Baud = 9600u;//更新波特率位置
    stcBaud.enMode  = UartMode1; //计算波特率需要模式参数
    stcBaud.u32Pclk = Sysctrl_GetPClkFreq(); //获取PCLK
    timer = Uart_SetBaudRate(M0P_UART1, &stcBaud);

    stcBtCfg.enMD = BtMode2;
    stcBtCfg.enCT = BtTimer;
    Bt_Init(TIM1, &stcBtCfg);//调用basetimer0设置函数产生波特率
    Bt_ARRSet(TIM1,timer);
    Bt_Cnt16Set(TIM1,timer);
    Bt_Run(TIM1);
}

void uart0_init(void)
{
    stc_uart_cfg_t  stcCfg;

    uart0_port_init();
    uart0_baud_cfg();

    stcCfg.enRunMode = UartMode1;//测试项，更改此处来转换4种模式测试
    Uart_Init(M0P_UART1, &stcCfg);

    ///< UART中断配置
    Uart_EnableIrq(M0P_UART1, UartRxIrq);
    Uart_ClrStatus(M0P_UART1, UartRC);
    EnableNvic(UART1_IRQn, IrqLevel3, TRUE);	
}

char uart0_send_data(unsigned char *buf, int len)
{
    int i;

    if(buf == NULL)
    {
        return 0;
    }

    for(i = 0;i<len;i++)
    {
       Uart_SendDataPoll(M0P_UART1, buf[i]);
		//Uart_SendDataIt(M0P_UART1, buf[i]);
    }

    return 0;
}

