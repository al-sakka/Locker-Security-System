/******************************************************************************
*
* Module: HMI ECU
*
* File Name: hmi.c
*
* Description: Source file for the HMI ECU
*
* Author: Abdallah El-Sakka
*
*******************************************************************************/

#include "../Utils/common_macros.h"
#include "../MCAL/gpio/gpio.h"
#include "../MCAL/UART/uart.h"
#include "../HAL/LCD/lcd.h"
#include "../HAL/Keypad/keypad.h"
#include "../MCAL/Timer/timer.h"
#include "hmi.h"
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

uint8 g_compareMatchCount = (INITIAL_VALUE_ZERO);
bool systemLocked = (FALSE);

/*******************************************************************************
*                       Functions Definitions                                  *
*******************************************************************************/

/*
 * Callback function of Timer1
 */
static void Timer1_CompareMatch_Callback(void)
{
	++(g_compareMatchCount);

	if(g_compareMatchCount >= ONE_MINUTE_TICKS)
	{
		/* After 15 seconds */
		Timer_deInit(TIMER1);
		g_compareMatchCount = (ZERO);
		systemLocked = (FALSE);

	}
}

/*
 * Initialize HMI ECU
 */
void HMI_init(void)
{
    LCD_init();
	sei();
}

/*
 * Configrations for HMI ECU
 */
static uint8 HMI_config(uint8 a_pressedKey, uint8 a_mode)
{
	Timer_ConfigType Timer_conf = {TIMER1, INITIAL_VALUE_ZERO, F_CPU_CLK_1024, COMPARE_MODE, COMPARE_VALUE};

	static uint8 password_str[PASSWORD_SIZE + ONE] = "";		/* 0 : 6 */
	static uint8 pass_chars = INITIAL_VALUE_ZERO;
	static uint8 passAttempts = INITIAL_VALUE_ZERO;
	static uint8 first_pass[PASSWORD_SIZE + ONE] = "";
	static uint8 second_pass[PASSWORD_SIZE + ONE] = "";
	static bool enterPressed = FALSE;

	if ((a_pressedKey >= '0') && (a_pressedKey <= '9') && (pass_chars < PASSWORD_SIZE))
	{
		LCD_displayCharacter('*');
		password_str[pass_chars] = (uint8)a_pressedKey;
		++pass_chars;
		_delay_ms(250);
	}
	else if (a_pressedKey == '=' && !enterPressed)
	{
		if (a_mode == 1)
		{
			password_str[PASSWORD_SIZE] = (STR_COMMON_CHAR);
			UART_sendByte(OPEN_DOOR_MODE);

			while(UART_receiveByte() != MCU_READY);

			UART_sendString(password_str);
			pass_chars = INITIAL_VALUE_ZERO;
			memset(password_str, '\0', PASSWORD_SIZE + 2);

			uint8 receivedByte;

			do
			{
			    receivedByte = UART_receiveByte();
			}
			while ((receivedByte != PASSWORD_CORRECT) && (receivedByte != PASSWORD_WRONG));

			if(receivedByte == PASSWORD_CORRECT)
			{
				LCD_doorUnlock();
				passAttempts = (ZERO);

				do
				{
					receivedByte = UART_receiveByte();
				}
				while (receivedByte != POLLING_WAITING_PEOPLE && receivedByte != POLLING_LOCKING_DOOR);

				if(receivedByte == POLLING_WAITING_PEOPLE)
				{
					LCD_waitPeople();

					while(UART_receiveByte() != POLLING_LOCKING_DOOR);

					LCD_lockDoor();

					while(UART_receiveByte() != (0xAA));

					LCD_menuDisplay();

					return (0xAA);

				}
				else if(receivedByte == POLLING_LOCKING_DOOR)
				{
					LCD_lockDoor();

					while(UART_receiveByte() != (0xAA));

					LCD_menuDisplay();

					return (0xAA);
				}
			}
			else if(receivedByte == PASSWORD_WRONG)
			{
				LCD_clearScreen();
				LCD_displayString("Wrong Password");
				_delay_ms(250);

				++passAttempts;

				if(passAttempts == 3)
				{
					UART_sendByte(ALARM_SIGNAL);
					LCD_alarm();
					passAttempts = ZERO;

					systemLocked = TRUE;

					Timer_init(&Timer_conf);
					Timer_setCallBack(	Timer1_CompareMatch_Callback, Timer_conf.timer_ID	);

					while(systemLocked == TRUE);
					UART_sendByte(ALARM_SIGNAL_OFF);
					LCD_menuDisplay();
					return (0xAA);

				}
				else
				{
					LCD_enterOldPass();
				}

				return (a_mode);
			}

			_delay_ms(250);

		}
		else if (a_mode == 2)
		{
			strncpy(first_pass, password_str, PASSWORD_SIZE);
			pass_chars = INITIAL_VALUE_ZERO;
			memset(password_str, '\0', PASSWORD_SIZE);
			enterPressed = TRUE;
			LCD_confirmPass();
		}
		else if (a_mode == 3)
		{
			password_str[PASSWORD_SIZE] = STR_COMMON_CHAR;
			UART_sendByte(CHANGE_PASS_MODE);

			while(UART_receiveByte() != MCU_READY);

			UART_sendString(password_str);
			pass_chars = INITIAL_VALUE_ZERO;
			memset(password_str, '\0', PASSWORD_SIZE + 2);
			_delay_ms(250);

			uint8 receivedByte;

			do
			{
			    receivedByte = UART_receiveByte();
			}
			while (receivedByte != PASSWORD_CORRECT && receivedByte != PASSWORD_WRONG);

			if(receivedByte == PASSWORD_CORRECT)
			{
				LCD_clearScreen();
				LCD_displayString("Right Password");
				_delay_ms(250);

				LCD_enterPass();

				passAttempts = 0;
				enterPressed = FALSE;
				return 2;
			}
			else if(receivedByte == PASSWORD_WRONG)
			{
				LCD_clearScreen();
				LCD_displayString("Wrong Password");
				_delay_ms(250);

				++passAttempts;

				if(passAttempts == 3)
				{
					UART_sendByte(ALARM_SIGNAL);
					LCD_alarm();
					passAttempts = ZERO;
					systemLocked = TRUE;

					Timer_init(&Timer_conf);
					Timer_setCallBack(	Timer1_CompareMatch_Callback, Timer_conf.timer_ID	);

					while(systemLocked == TRUE);

					UART_sendByte(ALARM_SIGNAL_OFF);
					LCD_menuDisplay();
					return (0xAA);

				}
				else
				{
					LCD_enterOldPass();
				}
			}

			pass_chars = INITIAL_VALUE_ZERO;
			memset(password_str, '\0', PASSWORD_SIZE + 2);
			_delay_ms(250);
		}

	}
	else if(a_pressedKey == '=' && enterPressed && a_mode == 2)
	{
		strncpy(second_pass, password_str, PASSWORD_SIZE);
		pass_chars = INITIAL_VALUE_ZERO;
		enterPressed = FALSE;

		bool passMatched = !(strcmp(first_pass, second_pass));

		memset(first_pass, '\0', PASSWORD_SIZE);
		memset(second_pass, '\0', PASSWORD_SIZE);

		LCD_clearScreen();

		if(passMatched)
		{
			password_str[PASSWORD_SIZE] 	= (uint8)STR_COMMON_CHAR;
			password_str[PASSWORD_SIZE + 1] = '\0';
			UART_sendByte(NEW_PASS_MODE);

			while(UART_receiveByte() != (MCU_READY));

			UART_sendString(password_str);

			memset(password_str, '\0', PASSWORD_SIZE + 2);
			_delay_ms(250);

			passAttempts = 0;

			return 1;
		}
		else
		{
			++passAttempts;

			if(passAttempts == 3)
			{
				UART_sendByte(ALARM_SIGNAL);
				passAttempts = 0;
			}

			memset(password_str, '\0', PASSWORD_SIZE + 2);
			LCD_enterPass();
		}

	}

	return (a_mode);	/* Return same mode */
}

/*
 * Main code logic in HMI ECU
 */
uint8 HMI_main(void)
{
	UART_ConfigType UART_conf = {_8_bit, Disabled, _1_bit, 9600};
    uint8 pressedKey = (ZERO);
    uint8 mode = 2;
    bool menuDisplayed = FALSE;  /* Flag to ensure LCD_menuDisplay is called only once */

    UART_init(&UART_conf);
    LCD_enterPass();

    for(;;)
    {
        if	(mode == 1 || mode == 0xAA)
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

				mode = 1;

        		for(;;)
        		{
        			pressedKey = KEYPAD_getPressedKey();
					mode = HMI_config(pressedKey, mode);

					if(mode == 0xAA)
					{
						mode = 1;
						break;
					}
        		}
        	}
        	else if(pressedKey == '-')
        	{
				LCD_enterOldPass();

        		mode = 3;

        		while(mode == 3)
        		{
        			pressedKey = KEYPAD_getPressedKey();
					mode = HMI_config(pressedKey, mode);
        		}
        	}

        }
        else if (mode == 2)
        {
        	/* New Password */
            pressedKey = KEYPAD_getPressedKey();  // Wait for the first key press
            mode = HMI_config(pressedKey, mode);
            menuDisplayed = FALSE;  // Reset flag when leaving menu mode
        }
    }
}
