/*
********************************************************************************
* TERN, Inc.
* (c) Copyright 2005, http://www.tern.com
*
* MODIFIED BY RICHARD BARRY TO ADD SEMAPHORE FOR COMMUNICATION BETWEEN THE 
* WIZnet ISR AND THE HTTP TASK.
*
* - Derived based on development version provided by Wiznet.
*
* Filename : socket.h
* Programmer(s):
* Created : 2002/06/20
* Modified :
*  2002/09/27 : - Renaming
*			       INT_STATUS --> INT_REG
*			       STATUS(i) --> INT_STATUS(i)
*			       C_STATUS(i) --> SOCK_STATUS(i)
*  2003/11/06 : Ported for use with TERN controller.  Note all byte access is at even addresses
*  2005/10/8  : Modified constants for easier initialization.
*
* Description : Header file of W3100A for TERN embedded controller
********************************************************************************
*/
/*
###############################################################################
File Include Section
###############################################################################
*/
#include "socket_freertos.h"
#include <string.h>
#include <stdio.h>

#include <FreeRTOS.h>
#include <semphr.h>
#include <portasm.h>


/*
###############################################################################
Local Variable Declaration Section
###############################################################################
*/
u_char I_STATUS[4];				// Store Interrupt Status according to channels
u_int Local_Port;				   // Designate Local Port
union un_l2cval	SEQ_NUM;		// Set initial sequence number

u_long SMASK[MAX_SOCK_NUM];   // Variable to store MASK of Tx in each channel,
                              // on setting dynamic memory size.
u_long RMASK[MAX_SOCK_NUM];   // Variable to store MASK of Rx in each channel,
                              // on setting dynamic memory size.
int SSIZE[MAX_SOCK_NUM];      // Maximun Tx memory size by each channel
int RSIZE[MAX_SOCK_NUM];      // Maximun Rx memory size by each channel

u_int SBUFBASEADDRESS[MAX_SOCK_NUM];   // Maximun Tx memory base address by each channel
u_int RBUFBASEADDRESS[MAX_SOCK_NUM];   // Maximun Rx memory base address by each channel

/*
###############################################################################
Function Implementation Section
###############################################################################
*/

void WZTOE_Handler(void)
{
    
}
