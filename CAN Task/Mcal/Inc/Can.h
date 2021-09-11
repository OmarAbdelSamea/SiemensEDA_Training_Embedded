/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Can.h
 *       Module:  Can
 *
 *  Description:  Header file for Can Module     
 *  
 *********************************************************************************************************************/
#ifndef CAN_H
#define CAN_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "../../Common/Std_Types.h"
#include "../../Common/Mcu_Hw.h"
#include "../../Common/tm4c123gh6pm.h"
#include "../../Config/Can_Cfg.h"
#include "../../Config/Can_Lcfg.h"
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define CONTROLLER_0 0
#define CONTROLLER_1 1

#define Standard 0x00FF
#define Extended 0xFFFF

#define Standard32Bit 0x400007FF
#define Extended32Bit 0xDFFFFFFF
/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
#define seg1(n) ((2 + ((n - 3) / 2)) - 1)
#define seg2(n) (((n - 3) / 2) - 1)
#define sjw (0)

#define CAN_BIT_VALUE(seg1, seg2, sjw)  \
	((((seg1 - 1) << CAN_BIT_TSEG1_S) & \
	  CAN_BIT_TSEG1_M) |                \
	 (((seg2 - 1) << CAN_BIT_TSEG2_S) & \
	  CAN_BIT_TSEG2_M) |                \
	 (((sjw - 1) << CAN_BIT_SJW_S) &    \
	  CAN_BIT_SJW_M))
	  
/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef uint32 Can_IdType;
typedef uint16 Can_HwHandleType;

typedef struct
{
	Can_IdType CanId;
	Can_HwHandleType Hoh;
	uint8 ControllerId;
} Can_HwType;

typedef struct
{
	boolean Controller;
	Mcu_Port Port;
} Can_ConfigType;

typedef struct
{
	uint32 MsgId;
	uint32 MsgMaskId;
	uint8 DLC;
	uint32 Data;
	uint8 MsgObjectNum;
} Can_MessageType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
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
void Can_Init(const Can_ConfigType *Config);
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
Std_ReturnType Can_SetBaudrate(uint8 Controller, uint32 Baudrate);
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
Std_ReturnType Can_Write(uint8 Controller, const Can_MessageType *Message);

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
Std_ReturnType Can_Read(uint8 Controller, const Can_MessageType *Message);

#endif /* CAN_H */

/**********************************************************************************************************************
 *  END OF FILE: Std_Types.h
 *********************************************************************************************************************/
