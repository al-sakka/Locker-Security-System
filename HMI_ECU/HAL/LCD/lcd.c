/******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: Source file for the LCD driver
 *
 * Author: Abdallah El-Sakka
 *
 *******************************************************************************/

#include "../../Utils/common_macros.h"
#include "../../MCAL/gpio/gpio.h"
#include "../../MCAL/UART/uart.h"
#include "../Keypad/keypad.h"
#include "lcd.h"
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the LCD
 */
void LCD_init(void)
{
	GPIO_setupPinDirection(RS_PORT_ID, RS_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(EN_PORT_ID, EN_PIN_ID, PIN_OUTPUT);
	_delay_ms(20); /* Power On Delay */

#if (LCD_DATA_BITS_MODE == (4))

    GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, PIN_OUTPUT);
    GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, PIN_OUTPUT);
    GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, PIN_OUTPUT);
    GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, PIN_OUTPUT);

    /* 4-bit initialization of LCD */
    LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
    LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);

    /* use 2-lines LCD + 4-bits data mode + 5*7 dot display mode */
    LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE);

#elif (LCD_DATA_BITS_MODE == (8))

	GPIO_setupPortDirection(LCD_DATA_PORT_ID, PORT_OUTPUT);

	/* ues 2-lines LCD + 8-bits data mode + 5*7 display mode */
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE);

#endif

	LCD_sendCommand(LCD_CURSOR_OFF);
	LCD_sendCommand(LCD_CLEAR_SCREEN);
}

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(uint8 command)
{
	GPIO_writePin(RS_PORT_ID, RS_PIN_ID, LOGIC_LOW);
	_delay_ms(1); /*    delay for processing Tas     */
	GPIO_writePin(EN_PORT_ID, EN_PIN_ID, LOGIC_HIGH);
	_delay_ms(1); /* delay for processing Tpw - Tdws */

#if (LCD_DATA_BITS_MODE == (4))

    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(command, PIN4_ID));
    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(command, PIN5_ID));
    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(command, PIN6_ID));
    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(command, PIN7_ID));

    _delay_ms(1);        /* delay for processing Tdsw */
    GPIO_writePin(EN_PORT_ID, EN_PIN_ID, LOGIC_LOW);
    _delay_ms(1);        /*  delay for processing Th  */
    GPIO_writePin(EN_PORT_ID, EN_PIN_ID, LOGIC_HIGH);
    _delay_ms(1);       /*  delay for processing Tpw  */

    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(command, PIN0_ID));
    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(command, PIN1_ID));
    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(command, PIN2_ID));
    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(command, PIN3_ID));

#elif (LCD_DATA_BITS_MODE == (8))
	GPIO_writePort(LCD_DATA_PORT_ID, command);
#endif

	_delay_ms(1); /* delay for processing Tdsw */
	GPIO_writePin(EN_PORT_ID, EN_PIN_ID, LOGIC_LOW);
	_delay_ms(1); /*  delay for processing Th  */
}

/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(uint8 data)
{
	GPIO_writePin(RS_PORT_ID, RS_PIN_ID, LOGIC_HIGH);
	_delay_ms(1); /*    delay for processing Tas     */
	GPIO_writePin(EN_PORT_ID, EN_PIN_ID, LOGIC_HIGH);
	_delay_ms(1); /* delay for processing Tpw - Tdws */

#if (LCD_DATA_BITS_MODE == (4))

    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(data, PIN4_ID));
    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(data, PIN5_ID));
    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(data, PIN6_ID));
    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(data, PIN7_ID));

    _delay_ms(1);        /* delay for processing Tdsw */
    GPIO_writePin(EN_PORT_ID, EN_PIN_ID, LOGIC_LOW);
    _delay_ms(1);        /*  delay for processing Th  */
    GPIO_writePin(EN_PORT_ID, EN_PIN_ID, LOGIC_HIGH);
    _delay_ms(1);       /*  delay for processing Tpw  */

    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, GET_BIT(data, PIN0_ID));
    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, GET_BIT(data, PIN1_ID));
    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, GET_BIT(data, PIN2_ID));
    GPIO_writePin(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, GET_BIT(data, PIN3_ID));

#elif (LCD_DATA_BITS_MODE == (8))
	GPIO_writePort(LCD_DATA_PORT_ID, data);
#endif

	_delay_ms(1); /* delay for processing Tdsw */
	GPIO_writePin(EN_PORT_ID, EN_PIN_ID, LOGIC_LOW);
	_delay_ms(1); /*  delay for processing Th  */
}

/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(const char *const str)
{
	uint8 char_index;

	for (char_index = 0; str[char_index] != '\0'; ++char_index)
	{
		LCD_displayCharacter(str[char_index]);
	}
}

/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 * rows: 0 : 3
 * cols: 0 : 15
 */
void LCD_moveCursor(uint8 row, uint8 col)
{
	uint8 rows_mem_mapping[4] = { 0x00, 0x40, 0x10, 0x50 };
	uint8 memory_address = col + rows_mem_mapping[row];

	LCD_sendCommand(memory_address | LCD_SET_CURSOR_LOCATION);
}

/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowCol(uint8 row, uint8 col, const char *const str)
{
	LCD_moveCursor(row, col);
	LCD_displayString(str);
}

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_displayInt(uint32 data)
{
	uint8 buff[16];
	sprintf(buff, "%lu", data);
	LCD_displayString(buff);
}

/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void)
{
	LCD_sendCommand(LCD_CLEAR_SCREEN);
	_delay_ms(2);
	LCD_moveCursor(0, 0);
}

/*
 * Description :
 * Show first password attempt
 */
void LCD_enterPass(void)
{
	LCD_clearScreen();
	LCD_displayString("Plz Enter Pass :");
	LCD_moveCursor(1, 0);
}

void LCD_alarm(void)
{
	LCD_clearScreen();
	LCD_displayString("Error!");
}

void LCD_enterOldPass(void)
{
	LCD_clearScreen();
	LCD_displayString("Plz Enter Old");
	LCD_moveCursor(1, 0);
	LCD_displayString("Pass :");
}

/*
 * Description :
 * Show second password attempt
 */
void LCD_confirmPass(void)
{
	LCD_clearScreen();
	LCD_displayString("Plz re-enter the");
	LCD_moveCursor(1, 0);
	LCD_displayString("same pass:");
}

void LCD_menuDisplay()
{
	LCD_clearScreen();
	LCD_displayString("+ : Open Door");
	LCD_moveCursor(1, 0);
	LCD_displayString("- : Change Pass");
}

void LCD_doorUnlock()
{
	LCD_clearScreen();
	LCD_displayString("Door Unlocking");
	LCD_moveCursor(1, 0);
	LCD_displayString("Please Wait");
}


/*
 * Description :
 * Set-up LCD configrations
 */
uint8 LCD_config(uint8 a_pressedKey, uint8 a_mode)
{
	static uint8 password_str[PASSWORD_SIZE + 1] = "";		/* 0 : 6 */
	static uint8 pass_chars = INITIAL_VALUE_ZERO;
	static uint8 passAttempts = 0;
	static uint8 first_pass[PASSWORD_SIZE + 1] = "";
	static uint8 second_pass[PASSWORD_SIZE + 1] = "";
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
			password_str[PASSWORD_SIZE] = STR_COMMON_CHAR;
			UART_sendByte(OPEN_DOOR_MODE);

			while(UART_receiveByte() != 0x10);

			UART_sendString(password_str);
			pass_chars = INITIAL_VALUE_ZERO;
			memset(password_str, '\0', PASSWORD_SIZE + 2);

			uint8 receivedByte;

			do
			{
			    receivedByte = UART_receiveByte();
			}
			while (receivedByte != 0x11 && receivedByte != 0x01);

			if(receivedByte == 0x11)
			{
				LCD_doorUnlock();
				passAttempts = 0;
				_delay_ms(10000);
			}
			else if(receivedByte == 0x01)
			{
				LCD_clearScreen();
				LCD_displayString("Wrong Password");
				_delay_ms(250);

				LCD_enterOldPass();

				++passAttempts;

				if(passAttempts == 3)
				{
					UART_sendByte(ALARM_SIGNAL);
					LCD_alarm();
					passAttempts = 0;
				}

				return a_mode;
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

			while(UART_receiveByte() != 0x10);

			UART_sendString(password_str);
			pass_chars = INITIAL_VALUE_ZERO;
			memset(password_str, '\0', PASSWORD_SIZE + 2);
			_delay_ms(250);

			uint8 receivedByte;

			do
			{
			    receivedByte = UART_receiveByte();
			}
			while (receivedByte != 0x11 && receivedByte != 0x01);

			if(receivedByte == 0x11)
			{
				LCD_clearScreen();
				LCD_displayString("Right Password");
				_delay_ms(250);

				LCD_enterPass();

				passAttempts = 0;

				return 2;
			}
			else if(receivedByte == 0x01)
			{
				LCD_clearScreen();
				LCD_displayString("Wrong Password");
				_delay_ms(250);

				LCD_enterOldPass();

				++passAttempts;

				if(passAttempts == 3)
				{
					UART_sendByte(ALARM_SIGNAL);
					LCD_alarm();
					passAttempts = 0;
				}

				return 3;
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

			while(UART_receiveByte() != 0x10);

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

	return a_mode;
}
