// If SW1 is pressed, the green LED will be toggled every half second (the red LED will be off).
// If SW1 is not pressed, the red LED will be toggled every half second (the green LED will be off).
// Whenever SW2 is pressed (no matter SW1 is pressed or not), all LEDs will be off.

// Considering the Tiva’s clock frequency is 16 MHz +/- 1%, one cycle duration is 1 / (16 * 10^6) = 62.5 ns. 
// Using SysCtlDelay, each iteration takes 3 CPU cycles, therefore the 
// total delay (in seconds) per toggle would be (3 cycle/loop) * (number of loops) * (1 / clock freq.).
// To get 0.5 second delays, we would need 0.5s / [3 * 62.5 ns] = 2,666,667 loops.

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

#define SW1	     		GPIO_PIN_4		// PF4
#define SW2      		GPIO_PIN_0		// PF0
#define red_LED           	GPIO_PIN_1		// 0x02
#define green_LED		GPIO_PIN_3		// 0x08

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

// ************** Initialized by PinMux Utility *************** 
	
    // enable pullup resistors using bitwise OR of PF4(0x10), PF0(0x01)
    GPIO_PORTF_PUR_R |= 0x11;        

}

void DelayInSec(double delay_in_s)
{
    // SysCtlClockGet() returns the processor clock frequency of the Tiva (16 MHz +/- 1%)
    // 1 clock cycle (in seconds) = 1 / SysCtlClockGet() second
    // 1 loop of SysCtlDelay = 3 clock cycles = 3 / SysCtlClockGet() 
    // 1 second = SysCtlClockGet()  / 3
    SysCtlDelay(delay_in_s * (SysCtlClockGet()  / 3));
}
	

int main(void)
{
    uint8_t LED_data;
	
    // initializes the GPIO ports	
    PortFunctionInit();
	
    while(1)
   {
	if(GPIOPinRead(GPIO_PORTF_BASE, SW2)==0x00) //SW2 is pressed, all off
	{
		GPIOPinWrite(GPIO_PORTF_BASE, red_LED, 0x00);	// red LED off
		GPIOPinWrite(GPIO_PORTF_BASE, green_LED, 0x00); // green LED off

	}
	else
	{
		if(GPIOPinRead(GPIO_PORTF_BASE, SW1)==0x00) //SW1 is pressed 
		{
			GPIOPinWrite(GPIO_PORTF_BASE, red_LED, 0x00);	// red LED off
			SysCtlDelay(2666667);	// about 0.5 second delay
			LED_data^=green_LED;	// toggles green LED (PF3)
			GPIOPinWrite(GPIO_PORTF_BASE, green_LED, LED_data); // green LED on
		}
		else //SW1 is not pressed, toggle red LED (PF3)
		{
			GPIOPinWrite(GPIO_PORTF_BASE, green_LED, 0x00);	// green LED off
			DelayInSec(0.5);   // about 0.5 second delay
			LED_data^=red_LED; // bitwise XOR red LED (PF1)
			GPIOPinWrite(GPIO_PORTF_BASE, red_LED, LED_data); // red LED on
		}
	}
    }
}
