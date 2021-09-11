/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Mcu_Hw.h
 *       Module:  -
 *
 *  Description:  Contains all MCU Registers Definitions     
 *  
 *********************************************************************************************************************/
#ifndef MCU_HW_H
#define MCU_HW_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
typedef enum {
	PORT_A,
	PORT_B,
	PORT_C,
	PORT_D,
	PORT_E,
	PORT_F,
} Mcu_Port;

#define CAN_IFnCMSK_WRNRD         7  // Write, Not Read
#define CAN_IFnCMSK_MASK          6  // Access Mask Bits
#define CAN_IFnCMSK_ARB           5  // Access Arbitration Bits
#define CAN_IFnCMSK_CONTROL       4  // Access Control Bits
#define CAN_IFnCMSK_CLRINTPND     3  // Clear Interrupt Pending Bit
#define CAN_IFnCMSK_NEWDAT        2  // Access New Data
#define CAN_IFnCMSK_TXRQST        2  // Access Transmission Request
#define CAN_IFnCMSK_DATAA         1  // Access Data Byte 0 to 3
#define CAN_IFnCMSK_DATAB         0  // Access Data Byte 4 to 7

#define CAN_IFnARB2_MSGVAL        15  // Message Valid
#define CAN_IFnARB2_XTD           14  // Extended Identifier
#define CAN_IFnARB2_DIR           13  // Message Direction
#define CAN_IFnARB2_ID            0   // Message Identifier

#define CAN_IFnMCTL_NEWDAT        15  // New Data
#define CAN_IFnMCTL_MSGLST        14  // Message Lost
#define CAN_IFnMCTL_INTPND        13  // Interrupt Pending
#define CAN_IFnMCTL_UMASK         12  // Use Acceptance Mask
#define CAN_IFnMCTL_TXIE          11  // Transmit Interrupt Enable
#define CAN_IFnMCTL_RXIE          10  // Receive Interrupt Enable
#define CAN_IFnMCTL_RMTEN         9   // Remote Enable
#define CAN_IFnMCTL_TXRQST        8   // Transmit Request
#define CAN_IFnMCTL_EOB           7   // End of Buffer
#define CAN_IFnMCTL_DLC           0   // Data Length Code

#define CAN_IFnCRQ_BUSY           15  // Busy Flag
#define CAN_IFnCRQ_MNUM           0   // Message Number

#define CAN_CTL_CCE_OFFSET        6  // Configuration Change Enable
/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/*************************************************** MCU Registers ***************************************************/
#define RCGC_PERPH_BASE_ADDRESS								0x400FE600
#define RCGC_PRIPH(n)										*((volatile uint32*)(RCGC_PERPH_BASE_ADDRESS+0x4*(n)))

/*************************************************** CAN Registers ***************************************************/
#define CAN_CTL_R(n)              (*((volatile unsigned long *)0x40040000+(n*0x1000)))
#define CAN_STS_R(n)              (*((volatile unsigned long *)0x40040004+(n*0x1000)))
#define CAN_ERR_R(n)              (*((volatile unsigned long *)0x40040008+(n*0x1000)))
#define CAN_BIT_R(n)              (*((volatile unsigned long *)0x4004000C+(n*0x1000)))
#define CAN_INT_R(n)              (*((volatile unsigned long *)0x40040010+(n*0x1000)))
#define CAN_TST_R(n)              (*((volatile unsigned long *)0x40040014+(n*0x1000)))
#define CAN_BRPE_R(n)             (*((volatile unsigned long *)0x40040018+(n*0x1000)))
#define CAN_IF1CRQ_R(n)           (*((volatile unsigned long *)0x40040020+(n*0x1000)))
#define CAN_IF1CMSK_R(n)          (*((volatile unsigned long *)0x40040024+(n*0x1000)))
#define CAN_IF1MSK1_R(n)          (*((volatile unsigned long *)0x40040028+(n*0x1000)))
#define CAN_IF1MSK2_R(n)          (*((volatile unsigned long *)0x4004002C+(n*0x1000)))
#define CAN_IF1ARB1_R(n)          (*((volatile unsigned long *)0x40040030+(n*0x1000)))
#define CAN_IF1ARB2_R(n)          (*((volatile unsigned long *)0x40040034+(n*0x1000)))
#define CAN_IF1MCTL_R(n)          (*((volatile unsigned long *)0x40040038+(n*0x1000)))
#define CAN_IF1DA1_R(n)           (*((volatile unsigned long *)0x4004003C+(n*0x1000)))
#define CAN_IF1DA2_R(n)           (*((volatile unsigned long *)0x40040040+(n*0x1000)))
#define CAN_IF1DB1_R(n)           (*((volatile unsigned long *)0x40040044+(n*0x1000)))
#define CAN_IF1DB2_R(n)           (*((volatile unsigned long *)0x40040048+(n*0x1000)))
#define CAN_IF2CRQ_R(n)           (*((volatile unsigned long *)0x40040080+(n*0x1000)))
#define CAN_IF2CMSK_R(n)          (*((volatile unsigned long *)0x40040084+(n*0x1000)))
#define CAN_IF2MSK1_R(n)          (*((volatile unsigned long *)0x40040088+(n*0x1000)))
#define CAN_IF2MSK2_R(n)          (*((volatile unsigned long *)0x4004008C+(n*0x1000)))
#define CAN_IF2ARB1_R(n)          (*((volatile unsigned long *)0x40040090+(n*0x1000)))
#define CAN_IF2ARB2_R(n)          (*((volatile unsigned long *)0x40040094+(n*0x1000)))
#define CAN_IF2MCTL_R(n)          (*((volatile unsigned long *)0x40040098+(n*0x1000)))
#define CAN_IF2DA1_R(n)           (*((volatile unsigned long *)0x4004009C+(n*0x1000)))
#define CAN_IF2DA2_R(n)           (*((volatile unsigned long *)0x400400A0+(n*0x1000)))
#define CAN_IF2DB1_R(n)           (*((volatile unsigned long *)0x400400A4+(n*0x1000)))
#define CAN_IF2DB2_R(n)           (*((volatile unsigned long *)0x400400A8+(n*0x1000)))
#define CAN_TXRQ1_R(n)            (*((volatile unsigned long *)0x40040100+(n*0x1000)))
#define CAN_TXRQ2_R(n)            (*((volatile unsigned long *)0x40040104+(n*0x1000)))
#define CAN_NWDA1_R(n)            (*((volatile unsigned long *)0x40040120+(n*0x1000)))
#define CAN_NWDA2_R(n)            (*((volatile unsigned long *)0x40040124+(n*0x1000)))
#define CAN_MSG1INT_R(n)          (*((volatile unsigned long *)0x40040140+(n*0x1000)))
#define CAN_MSG2INT_R(n)          (*((volatile unsigned long *)0x40040144+(n*0x1000)))
#define CAN_MSG1VAL_R(n)          (*((volatile unsigned long *)0x40040160+(n*0x1000)))
#define CAN_MSG2VAL_R(n)          (*((volatile unsigned long *)0x40040164+(n*0x1000)))

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

 
#endif  /* MCU_HW_H */

/**********************************************************************************************************************
 *  END OF FILE: Std_Types.h
 *********************************************************************************************************************/

