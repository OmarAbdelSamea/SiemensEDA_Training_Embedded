#ifndef _UART_H_
#define _UART_H_                   // Device header
#include "stdint.h"               // Device header
#include <tm4c123gh6pm.h>
/******** UART0 **********/
void UART0_Init(void);

unsigned char UART0_InChar(void);

void UART0_OutChar(unsigned char data);
#endif
