#include "uart.h"
/****************** UART0 *************/
void UART0_Init(void){
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0; 
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
  UART0_CTL_R &= ~UART_CTL_UARTEN;      
  UART0_IBRD_R = 104;  
  UART0_FBRD_R = 11;  
                                        
  UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART0_CTL_R |= UART_CTL_UARTEN;       
  GPIO_PORTA_AFSEL_R |= 0x03;           
  GPIO_PORTA_DEN_R |= 0x03;                                            
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
  GPIO_PORTA_AMSEL_R &= ~0x03;          
}

unsigned char UART0_InChar(void){
  while((UART0_FR_R&UART_FR_RXFE) != 0);
  return((unsigned char)(UART0_DR_R&0xFF));
}

void UART0_OutChar(unsigned char data){
  while((UART0_FR_R&UART_FR_TXFF) != 0);
  UART0_DR_R = data;
}
