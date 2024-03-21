#ifndef __BSP_UART_H
#define __BSP_UART_H


void uart0_init(void);
char uart0_send_data(unsigned char *buf, int len);

#endif //__BSP_UART_H

