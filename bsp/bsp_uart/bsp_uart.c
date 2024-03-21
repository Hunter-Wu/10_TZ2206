#include "bsp_uart.h"
#include "uart.h"
#include "bt.h"
#include "ddl.h"
#include "gpio.h"


// UART0 �жϷ�����
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

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); //ʹ��GPIOģ��ʱ��

    ///<TX
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPort3, GpioPin2, &stcGpioCfg);
    Gpio_SetAfMode(GpioPort3, GpioPin2, GpioAf5);        //����P32 �˿�ΪURART1_TX

    ///<RX
    stcGpioCfg.enDir = GpioDirIn;				
    Gpio_Init(GpioPort3, GpioPin6, &stcGpioCfg);
    Gpio_SetAfMode(GpioPort3, GpioPin6, GpioAf1); 		//����P36 �˿�ΪURART1_RX
}

static void uart0_baud_cfg(void)
{
    uint16_t timer=0;

    stc_uart_baud_cfg_t stcBaud;
    stc_bt_cfg_t stcBtCfg;

    DDL_ZERO_STRUCT(stcBaud);
    DDL_ZERO_STRUCT(stcBtCfg);

    //����ʱ��ʹ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralBt,TRUE);//ģʽ0/2���Բ�ʹ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE);

    stcBaud.bDbaud  = 0u;//˫�������ʹ���
    stcBaud.u32Baud = 9600u;//���²�����λ��
    stcBaud.enMode  = UartMode1; //���㲨������Ҫģʽ����
    stcBaud.u32Pclk = Sysctrl_GetPClkFreq(); //��ȡPCLK
    timer = Uart_SetBaudRate(M0P_UART1, &stcBaud);

    stcBtCfg.enMD = BtMode2;
    stcBtCfg.enCT = BtTimer;
    Bt_Init(TIM1, &stcBtCfg);//����basetimer0���ú�������������
    Bt_ARRSet(TIM1,timer);
    Bt_Cnt16Set(TIM1,timer);
    Bt_Run(TIM1);
}

void uart0_init(void)
{
    stc_uart_cfg_t  stcCfg;

    uart0_port_init();
    uart0_baud_cfg();

    stcCfg.enRunMode = UartMode1;//��������Ĵ˴���ת��4��ģʽ����
    Uart_Init(M0P_UART1, &stcCfg);

    ///< UART�ж�����
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

