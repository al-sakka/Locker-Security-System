/******************************************************************************
*
* Module: CONTROL ECU
*
* File Name: control.c
*
* Description: Source file for the CONTROL ECU
*
* Author: Abdallah El-Sakka
*
*******************************************************************************/

#include "control.h"
#include "../MCAL/Timer/timer.h"
#include "../HAL/Motor/motor.h"
#include "../HAL/PIR/pir.h"
#include "../HAL/EEPROM/eeprom.h"
#include "../MCAL/I2C/i2c.h"
#include "../Utils/std_types.h"
#include "../Utils/common_macros.h"
#include "../MCAL/UART/uart.h"
#include "../HAL/Buzzer/buzzer.h"
#include "../HAL/PIR/pir.h"
#include "control.h"
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8 g_compareMatchCount = INITIAL_VALUE_ZERO;
Door_state g_door_state = OFF;
bool timerRunning = FALSE;

/*******************************************************************************
*                       Functions Definitions                                  *
*******************************************************************************/

/*
 * Callback function of Timer1
 */
static void Timer1_CompareMatch_Callback(void)
{
	++(g_compareMatchCount);

	if(g_compareMatchCount >= _15_MINUTES_TICKS)
	{
		/* After 15 seconds */
		Timer_deInit(TIMER1);
		g_compareMatchCount = (ZERO);
		DCMotor_Rotate(Stop, MIN_SPEED);
		g_door_state = !(g_door_state);
		timerRunning = (FALSE);

	}
}

/*
 * Initialize CONTROL ECU
 */
void CONTROL_init(void)
{
	Buzzer_init();
	DCMotor_Init();
	PIR_init();
}

/*
 * Main code logic in CONTROL ECU
 */
uint8 CONTROL_main(void)
{

	I2C_ConfigType I2C_conf = {BIT_RATE, MASTER_ADDRESS};
	UART_ConfigType UART_conf = {_8_bit, Disabled, _1_bit, 9600};
	Timer_ConfigType Timer_conf = {TIMER1, INITIAL_VALUE_ZERO, F_CPU_CLK_1024, COMPARE_MODE, 58594};

	UART_init(&UART_conf);
	I2C_init(&I2C_conf);
	sei();

	uint8 EEPROM_password[PASSWORD_SIZE + ONE] = "";
	uint8 UART_password[PASSWORD_SIZE + ONE] = "";
	uint8 receivedByte = INITIAL_VALUE_ZERO;

	for(;;)
	{
		receivedByte = UART_receiveByte();
		UART_sendByte(MCU_READY);

		if(receivedByte == NEW_PASS_MODE)
		{
			UART_receiveString(UART_password);
			EEPROM_writeString(PASSWORD_ADDRESS, UART_password);
			_delay_ms(10); /* Allow EEPROM write to complete */
		}
		else if(receivedByte == OPEN_DOOR_MODE)
		{
			UART_receiveString(UART_password);
			EEPROM_readString(PASSWORD_ADDRESS, EEPROM_password, PASSWORD_SIZE + ONE);
			_delay_ms(10);

			bool passMatched = !(strcmp(EEPROM_password, UART_password));

			if(passMatched)
			{
				DCMotor_Rotate(CW, MAX_SPEED);
				Buzzer_off();
				UART_sendByte(PASSWORD_CORRECT);

				Timer_init(&Timer_conf);
				Timer_setCallBack(	Timer1_CompareMatch_Callback, Timer_conf.timer_ID	);
				timerRunning = TRUE;

				for(;;)
				{
					if(PIR_getState() == LOGIC_HIGH && g_door_state == ON && timerRunning == FALSE)
					{
						/* Waiting People */
						UART_sendByte(POLLING_WAITING_PEOPLE);
					}
					else if(PIR_getState() == LOGIC_LOW && g_door_state == ON && timerRunning == FALSE)
					{
						/* Locking Door */
						DCMotor_Rotate(A_CW, MAX_SPEED);
						UART_sendByte(POLLING_LOCKING_DOOR);

						timerRunning = TRUE;

						Timer_init(&Timer_conf);
						Timer_setCallBack(	Timer1_CompareMatch_Callback, Timer_conf.timer_ID	);
					}
					else if(timerRunning == FALSE && g_door_state == OFF)
					{
						UART_sendByte(0xAA);
						break;
					}
				}
			}
			else
			{
				DCMotor_Rotate(Stop, MIN_SPEED);
				UART_sendByte(PASSWORD_WRONG);
			}
		}
		else if(receivedByte == CHANGE_PASS_MODE)
		{
			UART_receiveString(UART_password);
			EEPROM_readString(PASSWORD_ADDRESS, EEPROM_password, PASSWORD_SIZE + ONE);
			_delay_ms(10);

			bool passMatched = !(strcmp(EEPROM_password, UART_password));

			if(passMatched)
			{
				UART_sendByte(PASSWORD_CORRECT);
			}
			else
			{
				UART_sendByte(PASSWORD_WRONG);
			}
		}
		else if(receivedByte == ALARM_SIGNAL)
		{
			Buzzer_on();
		}
		else if(receivedByte == ALARM_SIGNAL_OFF)
		{
			Buzzer_off();
		}
	}
}
