/*
 * main.c
 *
 */

#include "../HAL/LCD/lcd.h"
#include "../HAL/Keypad/keypad.h"
#include "../MCAL/UART/uart.h"
#include <util/delay.h>

int main(void)
{
	UART_ConfigType UART_conf = {_8_bit, Disabled, _1_bit, 9600};
    uint8 pressedKey = 0;
    uint8 mode = 2;
    bool menuDisplayed = FALSE;  // Flag to ensure LCD_menuDisplay is called only once

    LCD_init();
	UART_init(&UART_conf);

    LCD_enterPass();

    while(TRUE)
    {
        if	(mode == 1)
        {
        	/* Main Menu */
            if(!menuDisplayed)
            {
                LCD_menuDisplay();
                menuDisplayed = TRUE;  // Set flag to avoid calling LCD_menuDisplay again
            }

            pressedKey = KEYPAD_getPressedKey();

        	if(pressedKey == '+')
        	{
				LCD_enterOldPass();
        		while(1)
        		{
        			pressedKey = KEYPAD_getPressedKey();
					mode = LCD_config(pressedKey, 1);
        		}
        	}
        	else if(pressedKey == '-')
        	{
				LCD_enterOldPass();
        		mode = 3;
        		while(mode == 3)
        		{
        			pressedKey = KEYPAD_getPressedKey();
					mode = LCD_config(pressedKey, mode);
        		}
        	}

        }
        else if (mode == 2)
        {
        	/* New Password */
            pressedKey = KEYPAD_getPressedKey();  // Wait for the first key press
            mode = LCD_config(pressedKey, mode);
            menuDisplayed = FALSE;  // Reset flag when leaving menu mode
        }
    }
}

