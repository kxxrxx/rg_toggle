# rg_toggle

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
