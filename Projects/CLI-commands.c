/**
 *
 * \file
 *
 * \brief FreeRTOS+CLI command examples
 *
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* FreeRTOS+CLI includes. */
#include "FreeRTOS_CLI.h"
#include "W7500x_wztoe.h"

#include "util.h"
#include "serial.h"

#define SUCCESS_MESSAGE "[OK]\r\n"
#define FAIL_MESSAGE           "[FAIL]\r\n"

static BaseType_t prvSetNetworkConfigCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t prvGetNetworkConfigCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );


static const CLI_Command_Definition_t xSetNetConfig =
{
	"set-net",
	"set-network <ip-addr> <subnet> <gateway>:\r\n",
	prvSetNetworkConfigCommand, /* The function to run. */
	3 /* Three parameters are expected, which can take any value. */
};

static const CLI_Command_Definition_t xGetNetConfig =
{
	"get-net",
	"get-network\r\n",
	prvGetNetworkConfigCommand, /* The function to run. */
	0
};



/*
 * Holds the handle of the task created by the create-task command.
 */


/*-----------------------------------------------------------*/

void vRegisterCLICommands(void)
{
	/* Register all the command line commands defined immediately above. */
    FreeRTOS_CLIRegisterCommand(&xSetNetConfig);
    FreeRTOS_CLIRegisterCommand(&xGetNetConfig);
    
}


static BaseType_t prvSetNetworkConfigCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
const char *pcParameter;
char ip[16]={0};
char subnet[16]={0};
char gateway[16]={0};

BaseType_t xParameterStringLength, xReturn;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

    pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameterStringLength);
    strncat(ip, pcParameter, xParameterStringLength);
    setSIPR( str_to_ip_array(ip) );

    pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParameterStringLength);
    strncat(subnet, pcParameter, xParameterStringLength);
    setSUBR( str_to_ip_array(subnet) ) ;

    pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, 3, &xParameterStringLength);
    strncat(gateway, pcParameter, xParameterStringLength);
    setGAR( str_to_ip_array(gateway) );
    
    sprintf(pcWriteBuffer,SUCCESS_MESSAGE); 
    
    xReturn = pdFALSE;
    return xReturn;
}

static BaseType_t prvGetNetworkConfigCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
BaseType_t xReturn;
uint8_t tmp[8];
static int paramCount = 0;

    /* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );


    switch(paramCount)
    {
        case 0:
            getSHAR(tmp);
            sprintf(pcWriteBuffer,"MAC ADDRESS : %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\r\n",tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5]); 
            xReturn = pdPASS;
            paramCount++;
            break;
        case 1:
            getSIPR(tmp);
            sprintf(pcWriteBuffer,"IP ADDRESS  : %d.%d.%d.%d\r\n",tmp[0],tmp[1],tmp[2],tmp[3]); 
            xReturn = pdPASS;
            paramCount++;
            break;
        case 2:
            getGAR(tmp);
            sprintf(pcWriteBuffer,"GW ADDRESS  : %d.%d.%d.%d\r\n",tmp[0],tmp[1],tmp[2],tmp[3]); 
            xReturn = pdPASS;
            paramCount++;
            break;
        case 3:
            getSUBR(tmp);
            sprintf(pcWriteBuffer,"SN MASK    : %d.%d.%d.%d\r\n",tmp[0],tmp[1],tmp[2],tmp[3]); 
            xReturn = pdPASS;
            paramCount++;
            break;
        default:
            paramCount=0;
            xReturn = pdFALSE;
            break;
    }

    
    return xReturn;
}

