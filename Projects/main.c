/**
  ******************************************************************************
  * @file    Uart/Printf/main.c 
  * @author  IOP Team
  * @version V1.0.0
  * @date    01-May-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WIZnet SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 WIZnet Co.,Ltd.</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "W7500x_uart.h"
#include "W7500x_gpio.h"
#include "W7500x_miim.h"
#include "W7500x_wztoe.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#define mainUART_COMMAND_CONSOLE_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define mainUART_COMMAND_CONSOLE_TASK_PRIORITY	( 4U )


//extern void main_blinky( void );
/*
 * Initialise the LED ports, and create a timer that periodically toggles an LED
 * just to provide a visual indication that the program is running.
 */
extern void vLEDsInitialise( void );
extern void vRegisterCLICommands(void);

void InitNetwork(void);
void vUARTCommandConsoleStart( uint16_t usStackSize, UBaseType_t uxPriority );

void delay(__IO uint32_t nCount);


int main()
{

    UART_InitTypeDef UART_InitStructure;

    SystemInit();
    SystemCoreClockUpdate();
    
    UART_StructInit(&UART_InitStructure);
    UART_Init(UART1,&UART_InitStructure);
    S_UART_Init(115200);
    
    InitNetwork();
    
    /* Start the timer that just toggles an LED to show the demo is running. */
	vLEDsInitialise();
    
	/* Start the tasks that implements the command console on the UART, as
	described above. */
	vUARTCommandConsoleStart(mainUART_COMMAND_CONSOLE_STACK_SIZE, mainUART_COMMAND_CONSOLE_TASK_PRIORITY);

    /* Register CLI Command */
    vRegisterCLICommands();


	/* Start the RTOS scheduler. */
	vTaskStartScheduler();

    for( ;; );
}


#define __DEF_USED_MDIO__ 
#define __DEF_USED_IC101AG__ //for W7500 Test main Board V001

void InitNetwork(void)
{
    uint8_t mac_addr[6] = {0x00, 0x08, 0xDC, 0x01, 0x02, 0x03};     
    uint8_t src_addr[4] = {192, 168,  77,  9};
    uint8_t gw_addr[4]  = {192, 168,  77,  1};
    uint8_t sub_addr[4] = {255, 255, 255,  0};		

    
#ifdef __DEF_USED_IC101AG__ //For using IC+101AG
    *(volatile uint32_t *)(0x41003068) = 0x64; //TXD0 - set PAD strengh and pull-up
    *(volatile uint32_t *)(0x4100306C) = 0x64; //TXD1 - set PAD strengh and pull-up
    *(volatile uint32_t *)(0x41003070) = 0x64; //TXD2 - set PAD strengh and pull-up
    *(volatile uint32_t *)(0x41003074) = 0x64; //TXD3 - set PAD strengh and pull-up
    *(volatile uint32_t *)(0x41003050) = 0x64; //TXE  - set PAD strengh and pull-up
#endif	
#ifdef __DEF_USED_MDIO__ 
    /* mdio Init */
    mdio_init(GPIOB, MDC, MDIO );
    /* PHY Link Check via gpio mdio */
    while( link() == 0x0 )
    {
        S_UartPutc('.');
        delay(500);
    }
    S_UartPuts("PHY is linked. \r\n");  
#else
    delay(1000);
    delay(1000);
#endif


    /* Network Configuration */
    setSHAR(mac_addr);
    setSIPR(src_addr);
    setGAR(gw_addr);
    setSUBR(sub_addr);

    
}

void delay(__IO uint32_t nCount)
{
    int i;
    for(i=0;i<nCount*1000;i++);
}


void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()).  The code in this
	tick hook implementation is for demonstration only - it has no real
	purpose.  It just gives a semaphore every 50ms.  The semaphore unblocks a
	task that then toggles an LED.  Additionally, the call to
	vQueueSetAccessQueueSetFromISR() is part of the "standard demo tasks"
	functionality. */

	/* The semaphore and associated task are not created when the simple blinky
	demo is used. */

}
/*-----------------------------------------------------------*/




