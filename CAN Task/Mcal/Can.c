/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Can.c
 *        \brief  Controller Area Network Driver
 *                
 *      \details The CAN driver provides services for basic CAN module initialization, 
 *               CAN read and CAN wirte
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Inc/Can.h"
/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
static const Can_ConfigType *Config = NULL_PTR;
/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/******************************************************************************
* \Syntax          : void Can_Init(const Can_ConfigType *Config)        
* \Description     : This function initializes the CAN module.                                                                                                               
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : Config       Pointer to CAN driver configuration set.                     
* \Parameters (out): None                                                      
* \Return value:   : None
*******************************************************************************/
void Can_Init(const Can_ConfigType *Config)
{
	// Enable clock for Controller in RCGC0 Register
	switch (Config->Controller)
	{
	case CONTROLLER_0:
		// Set bit 24 in RCGC0
		SYSCTL_RCGC0_R |= SYSCTL_RCGC0_CAN0;
		switch (Config->Port)
		{
		case PORT_B:
			// Enable Port B clock in RCGC2
			SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
			// Set GPIOAFSEL
			SETBIT(GPIO_PORTB_AFSEL_R, 4);
			SETBIT(GPIO_PORTB_AFSEL_R, 5);
			// Set GPIOPCTl
			GPIO_PORTB_PCTL_R |= (GPIO_PCTL_PB5_CAN0TX + GPIO_PCTL_PB4_CAN0RX);
			break;
		case PORT_E:
			// Enable Port E clock in RCGC2
			SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
			// Set GPIOAFSEL
			SETBIT(GPIO_PORTE_AFSEL_R, 4);
			SETBIT(GPIO_PORTE_AFSEL_R, 5);
			// Set GPIOPCRL
			GPIO_PORTE_PCTL_R |= (GPIO_PCTL_PE5_CAN0TX + GPIO_PCTL_PE4_CAN0RX);
			break;
		case PORT_F:
			// Enable Port F clock in RCGC2
			SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
			// Set GPIOAFSEL
			SETBIT(GPIO_PORTF_AFSEL_R, 0);
			SETBIT(GPIO_PORTF_AFSEL_R, 3);
			// Set GPIOPCTL
			GPIO_PORTB_PCTL_R |= (GPIO_PCTL_PF3_CAN0TX + GPIO_PCTL_PF0_CAN0RX);
			break;
		default:
			return;
			break;
		}
		// set baud rate
		Can_SetBaudrate(Config->Controller, CAN_BAUDRATE);
		break;
	case CONTROLLER_1:
		// Set bit 25 in RCGC0
		SYSCTL_RCGC0_R |= SYSCTL_RCGC0_CAN1;
		switch (Config->Port)
		{
		case PORT_A:
			// Enable Port A clock in RCGC2
			SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
			// Set GPIOAFSEL
			SETBIT(GPIO_PORTA_AFSEL_R, 4);
			SETBIT(GPIO_PORTA_AFSEL_R, 5);
			// Set GPIOPCTL
			GPIO_PORTB_PCTL_R |= (GPIO_PCTL_PA1_CAN1TX + GPIO_PCTL_PA0_CAN1RX);
			break;
		default:
			return;
			break;
		}
		// set baud rate
		Can_SetBaudrate(Config->Controller, CAN_BAUDRATE);
		break;
	default:
		break;
	}
}

/******************************************************************************
* \Syntax          : Can_SetBaudrate(uint8 Controller, uint32 Baudrate)        
* \Description     : This service shall set the baud rate configuration of the CAN controller. Depending 
*				   on necessary baud rate modifications the controller might have to reset. 
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : Controller    
*					 Baudrate                           
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType
*                    E_OK: Command has been accepted
*                    E_NOT_OK: Command has not been accepted
*******************************************************************************/
Std_ReturnType Can_SetBaudrate(uint8 Controller, uint32 Baudrate)
{
	uint32 ratio = (SYSTEM_CLOCK * 10^6) / Baudrate; /* ratio = n * prescaler */
	// Set INIT bit in CANCTL
	SETBIT(CAN_CTL_R(Controller), CAN_CTL_INIT);
	// Write accesses to the CANBIT register are allowed
	SETBIT(CAN_CTL_R(Controller), CAN_CTL_CCE_OFFSET);
	if ((SYSTEM_CLOCK * 10^6) / ratio > Baudrate)
	{
		ratio += 1;
	}
	while (ratio <= (CAN_MAX_PRESCALER * CAN_MAX_BIT_DIVISOR))
	{
		for (uint32 CanBits = CAN_MAX_BIT_DIVISOR; CanBits >= CAN_MIN_BIT_DIVISOR; CanBits--)
		{
			uint32 prescaler = ratio / CanBits;
			if ((prescaler * CanBits) == ratio)
			{
				CAN_BIT_R(Controller) = CAN_BIT_VALUE(seg2(CanBits), seg1(CanBits), sjw);
				CAN_BRPE_R(Controller) = (prescaler & 0x3C0) >> 6;
				// Clear Init bit CANCTL
				CLRBIT(CAN_CTL_R(Controller), CAN_CTL_INIT);
				return E_OK;
			}
		}
		ratio += 1;
	}

	return E_NOT_OK;
}

/******************************************************************************
* \Syntax          : Std_ReturnType Can_Write(uint8 Controller, const Can_MessageType *Message)
* \Description     : This service writes into can module.                                                                                                               
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : Controller       
*                    Message                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType 
*                    E_OK: Command has been accepted
*                    E_NOT_OK: Command has not been accepted
*******************************************************************************/
Std_ReturnType Can_Write(uint8 Controller, const Can_MessageType *Message)
{
	// Set WRNRD bit in CANIFCMSK
	SETBIT(CAN_IF1CMSK_R(Controller), CAN_IFnCMSK_WRNRD);
	// Set ARB bit in CANIFCMSK
	SETBIT(CAN_IF1CMSK_R(Controller), CAN_IFnCMSK_ARB);
	// Set CONTROL bit CANIFCMSK
	SETBIT(CAN_IF1CMSK_R(Controller), CAN_IFnCMSK_CONTROL);
	// Access Data Byte 0 to 3
	SETBIT(CAN_IF1CMSK_R(Controller), CAN_IFnCMSK_DATAA);
	// Access Data Byte 4 to 7
	SETBIT(CAN_IF1CMSK_R(Controller), CAN_IFnCMSK_DATAB);
	// Set direction to transmit
	SETBIT(CAN_IF1ARB2_R(Controller), CAN_IFnARB2_DIR);
	// Set UMASK bit in CANIFMCTL
	SETBIT(CAN_IF1MCTL_R(Controller), CAN_IFnMCTL_UMASK);

	// Check MessageId length standard or extended
	if (Message->MsgId <= 2047)
	{
		// Standard
		CLRBIT(CAN_IF1ARB2_R(Controller), CAN_IFnARB2_XTD);

		// MSK [12:2] are used for bits [10:0] of the MASK
		CAN_IF1MSK2_R(Controller) |= Message->MsgMaskId << 2;

		// bits[12:2] are used for bits [10:0] of the ID
		CAN_IF1ARB2_R(Controller) |= Message->MsgId << 2;
	}
	else
	{
		// Extended
		SETBIT(CAN_IF1ARB2_R(Controller), CAN_IFnARB2_XTD);

		/* 
		MSK[15:0]of the CANIFnARB1 register
        are [15:0] of the MASK, while these bits, MSK[12:0], 
		are [28:16] of the MASK. 
		*/
		CAN_IF1MSK1_R(Controller) |= Message->MsgMaskId & 0xFFFF;
		CAN_IF1MSK2_R(Controller) |= Message->MsgMaskId >> 16;

		/* 
		ID[15:0]of the CANIFnARB1 register
        are [15:0] of the ID, while these bits, ID[12:0], 
		are [28:16] of the ID. 
		*/
		CAN_IF1ARB1_R(Controller) |= (Message->MsgId & 0x0000FFFF);
		CAN_IF1ARB2_R(Controller) |= (Message->MsgId & 0x1FFF0000) >> 16;
	}

	// set the MSGVALbit to indicate that the message object is valid
	SETBIT(CAN_IF1ARB2_R(Controller), CAN_IFnARB2_MSGVAL);

	// Set data length
	CAN_IF1MCTL_R(Controller) |= Message->DLC;

	// Load data
	CAN_IF1DA1_R(Controller) = (Message->Data & 0x000000000000FFFF);
	CAN_IF1DA2_R(Controller) = (Message->Data & 0x00000000FFFF0000) >> 16;
	CAN_IF1DB1_R(Controller) = (Message->Data & 0x0000FFFF00000000) >> 32;
	CAN_IF1DB2_R(Controller) = (Message->Data & 0xFFFF000000000000) >> 48;

	CAN_IF1CRQ_R(Controller) |= Message->MsgObjectNum;

	// Set bit TXRQST the message object is available to be transmitted
	SETBIT(CAN_IF1MCTL_R(Controller), CAN_IFnMCTL_TXRQST);

	// This bit is cleared when write action has finished.
	while (GETBIT(CAN_IF1CRQ_R(Controller), CAN_IFnCRQ_BUSY))
		;
	return E_OK;
}

/******************************************************************************
* \Syntax          : Std_ReturnType Can_Read(uint8 Controller, const Can_MessageType *Message)
* \Description     : This service reades from can module.                                                                                                               
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : Controller       
*                    Message                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType 
*                    E_OK: Command has been accepted
*                    E_NOT_OK: Command has not been accepted
*******************************************************************************/
Std_ReturnType Can_Read(uint8 Controller, const Can_MessageType *Message)
{
	// Set WRNRD bit in CANIFCMSK
	SETBIT(CAN_IF2CMSK_R(Controller), CAN_IFnCMSK_WRNRD);
	// Set ARB bit in CANIFCMSK
	SETBIT(CAN_IF2CMSK_R(Controller), CAN_IFnCMSK_ARB);
	// Set CONTROL bit CANIFCMSK
	SETBIT(CAN_IF2CMSK_R(Controller), CAN_IFnCMSK_CONTROL);
	// Access Data Byte 0 to 3
	SETBIT(CAN_IF2CMSK_R(Controller), CAN_IFnCMSK_DATAA);
	// Access Data Byte 4 to 7
	SETBIT(CAN_IF2CMSK_R(Controller), CAN_IFnCMSK_DATAB);
	// Set direction to recieve
	CLRBIT(CAN_IF2ARB2_R(Controller), CAN_IFnARB2_DIR);
	// Set UMASK bit in CANIFMCTL
	SETBIT(CAN_IF2MCTL_R(Controller), CAN_IFnMCTL_UMASK);

	// Check MessageId length standard or extended
	if (Message->MsgId <= 2047)
	{
		// Standard
		CLRBIT(CAN_IF2ARB2_R(Controller), CAN_IFnARB2_XTD);

		// MSK [12:2] are used for bits [10:0] of the MASK
		CAN_IF2MSK2_R(Controller) |= Message->MsgMaskId << 2;

		// ID [12:2] are used for bits [10:0] of the ID
		CAN_IF2ARB2_R(Controller) |= Message->MsgId << 2;
	}
	else
	{
		// Extended
		SETBIT(CAN_IF2ARB2_R(Controller), CAN_IFnARB2_XTD);

		/* 
		MSK[15:0]of the CANIFnARB1 register
        are [15:0] of the MASK, while these bits, MSK[12:0], 
		are [28:16] of the MASK. 
		*/
		CAN_IF2MSK1_R(Controller) |= Message->MsgMaskId & 0xFFFF;
		CAN_IF2MSK2_R(Controller) |= Message->MsgMaskId >> 16;

		/* 
		ID[15:0]of the CANIFnARB1 register
        are [15:0] of the ID, while these bits, ID[12:0], 
		are [28:16] of the ID. 
		*/
		CAN_IF1ARB1_R(Controller) |= (Message->MsgId & 0x0000FFFF);
		CAN_IF1ARB2_R(Controller) |= (Message->MsgId & 0x1FFF0000) >> 16;
	}

	// set the MSGVALbit to indicate that the message object is valid
	SETBIT(CAN_IF2ARB2_R(Controller), CAN_IFnARB2_MSGVAL);

	// Set data length
	CAN_IF2MCTL_R(Controller) |= Message->DLC;

	CAN_IF2CRQ_R(Controller) |= Message->MsgObjectNum;

	// This bit is cleared when write action has finished.
	while (GETBIT(CAN_IF2CRQ_R(Controller), CAN_IFnCRQ_BUSY));

	static uint8 data[8] = {0};
	data[0] = (CAN_IF2DA1_R(Controller) & 0x00FF);
	data[1] = (CAN_IF2DA1_R(Controller) & 0xFF00) >> 8;

	data[2] = (CAN_IF2DA2_R(Controller) & 0x00FF);
	data[3] = (CAN_IF2DA2_R(Controller) & 0xFF00) >> 8;

	data[4] = (CAN_IF2DB1_R(Controller) & 0x00FF);
	data[5] = (CAN_IF2DB1_R(Controller) & 0xFF00) >> 8;

	data[6] = (CAN_IF2DB2_R(Controller) & 0x00FF);
	data[7] = (CAN_IF2DB2_R(Controller) & 0xFF00) >> 8;

	// Set bit TXRQST the message object is recieved
	SETBIT(CAN_IF2MCTL_R(Controller), CAN_IFnMCTL_TXRQST);

	return E_OK;
}

/**********************************************************************************************************************
 *  END OF FILE: Can.c
 *********************************************************************************************************************/
