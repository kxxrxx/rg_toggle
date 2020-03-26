//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "rg_toggle.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"

#define SW1	     						GPIO_PIN_4		// PF4
#define SW2      						GPIO_PIN_0		// PF0
#define red_LED           	GPIO_PIN_1		// 0x02
#define green_LED					GPIO_PIN_3		// 0x08

//*****************************************************************************
void
PortFunctionInit(void)
{
    //
    // Enable Peripheral Clocks 
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    //First open the lock and select the bits we want to modify in the GPIO commit register.
    //
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0x1;

    //
    //Now modify the configuration of the pins that we unlocked.
    //
	
	  //
    // Enable pin PF0 for GPIOInput
    //

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);

    //
    // Enable pin PF4 for GPIOInput
    //
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);

    //
    // Enable pin PF1 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
		
    //
    // Enable pin PF3 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

		// ****** Initialized by PinMux Utility *************** 
	
    // enable pullup resistors using bitwise OR of PF4(0x10), PF0(0x01)
    GPIO_PORTF_PUR_R |= 0x11;        

}

int main(void)
{
    uint8_t LED_data;
	
		//initialize the GPIO ports	
		PortFunctionInit();
	
		while(1)
		{
					if(GPIOPinRead(GPIO_PORTF_BASE, SW2)==0x00) //SW2 is pressed, all off
										{
												GPIOPinWrite(GPIO_PORTF_BASE, red_LED, 0x00);
												GPIOPinWrite(GPIO_PORTF_BASE, green_LED, 0x00);
										}
				 else
										{
												if(GPIOPinRead(GPIO_PORTF_BASE, SW1)==0x00) //SW1 is pressed, green LED is on
														{
																GPIOPinWrite(GPIO_PORTF_BASE, red_LED, 0x00);
																GPIOPinWrite(GPIO_PORTF_BASE, green_LED, 0x08);
														}
												else //SW1 is not pressed, red LED is on
														{
																GPIOPinWrite(GPIO_PORTF_BASE, red_LED, 0x02);
																GPIOPinWrite(GPIO_PORTF_BASE, green_LED, 0x00);
														}
										}
	 }
}
















