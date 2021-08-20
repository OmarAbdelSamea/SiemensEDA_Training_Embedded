#include "stdint.h"
#include "tm4c123gh6pm.h"
#include "uart.h" 

uint8_t data;

int main(void)
{
	UART0_Init();
	while(1)
	{
		data = UART0_InChar();
		if (data > 90) // small letter
			UART0_OutChar(data - 32); // change to capital
		else // capital letter
			UART0_OutChar(data + 32); // change to small
	}
}
