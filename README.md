# rg_toggle.c

The system has two input switches (SW1 and SW2) and two output LEDs (red and
green LEDs). 

- If SW1 is pressed, the green LED will be toggled every half second (the red LED will be off).

- If SW1 is not pressed, the red LED will be toggled every half second (the green LED will be off).

- Whenever SW2 is pressed (no matter SW1 is pressed or not), all LEDs will be off.

~~~~~~~~~~~~~

SW1 = PF4 = 0x10 
SW2 = PF0 = 0x01

red = PF1 = 0x02 
blue = PF2 = 0x04 
green = PF3 = 0x08

- toggle using bitwise xor because LED_data ^ LED_data = 0 and LED_data ^ 0 = LED_data

- Considering the Tiva’s clock frequency is 16 MHz +/- 1%, one cycle duration is 1 / (16 * 10^6) = 6.25 x 10^(-8) seconds. Using SysCtlDelay, each iteration takes 3 CPU cycles, therefore the total delay per toggle would be 3 * (number of loops) * (1 / clock freq.).

- To get 0.5 second delays, we would need 0.5 / [3 * 6.25 x 10^(-8)] = 2,666,667 loops.

# rg_toggle2.c

- defines a new function called DelayInSec which SysCtlDelay(delay_in_s * (SysCtlClockGet()  / 3));
SysCtlClockGet() returns the processor clock frequency of the Tiva (16 MHz +/- 1%)
1 clock cycle (in seconds) = 1 / SysCtlClockGet() second
1 SysCtlDelay = 3 clock cycles = 3 / SysCtlClockGet() 
1 second = SysCtlClockGet()  / 3

Check TivaWare Peripheral Driver Library for more info:
SysCtlClockGet (p. 488) 
SysCtlDelay (p. 493)
