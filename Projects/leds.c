/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "timers.h"

/* Library includes. */
#include "W7500x_gpio.h"

#define ledTOGGLE_RATE	( 500 / portTICK_RATE_MS )


/*
 * Toggles an LED just to show the application is running.
 */
static void prvLEDToggleTimerCallback( xTimerHandle xTimer );

/*-----------------------------------------------------------*/

void vLEDsInitialise( void )
{
static xTimerHandle xLEDToggleTimer = NULL;

	/* Set the LED pin-muxing and configure as output. */
    GPIO_InitTypeDef GPIO_InitDef;

    /* GPIO LED1 & LED2 Set */
    GPIO_InitDef.GPIO_Pin = ( GPIO_Pin_1 | GPIO_Pin_2) ; // Set to Pin_1 (LED1) & Pin_2 (LED2)
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT; // Set to Mode Output
    GPIO_Init(GPIOA, &GPIO_InitDef);
    PAD_AFConfig(PAD_PA,(GPIO_Pin_1|GPIO_Pin_2), PAD_AF1); // PAD Config - LED used 2nd Function
    

    /* Create the timer used to toggle LED0. */
	xLEDToggleTimer = xTimerCreate(	"LEDTmr", 		/* Just a text name to associate with the timer, useful for debugging, but not used by the kernel. */
									ledTOGGLE_RATE,	/* The period of the timer. */
									pdTRUE,			/* This timer will autoreload, so uxAutoReload is set to pdTRUE. */
									NULL,			/* The timer ID is not used, so can be set to NULL. */
									prvLEDToggleTimerCallback );		/* The callback function executed each time the timer expires. */

    /* Sanity check that the timer was actually created. */
    configASSERT( xLEDToggleTimer );

    /* Start the timer.  If this is called before the scheduler is started then
    the block time will automatically get changed to 0 (from portMAX_DELAY). */
    xTimerStart( xLEDToggleTimer, portMAX_DELAY );
}
/*-----------------------------------------------------------*/

static void prvLEDToggleTimerCallback( xTimerHandle xTimer )
{
static uint8_t ucState = 0;

	/* Remove compiler warnings. */
	( void ) xTimer;

	/* Just toggle an LED to show the program is running. */
	if( ucState == 0 )
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	}

	ucState = !ucState;
}

