/******************************************************************************
*
* Module: DC Motor
*
* File Name: motor.c
*
* Description: Source file for the DC Motor driver
*
* Author: Abdallah El-Sakka
*
*******************************************************************************/

#include "../../Utils/std_types.h"
#include "../../MCAL/GPIO/gpio.h"
#include "../../MCAL/PWM/pwm.h"
#include "motor.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
*******************************************************************************/

/*
 * Initializes the DC motor by setting the direction for the motor pins
 * and stopping the motor at the beginning
 */
void DCMotor_Init(void)
{
	GPIO_setupPinDirection(IN1_PORT, IN1_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(IN2_PORT, IN2_PIN, PIN_OUTPUT);

	GPIO_setupPinDirection(EN1_PORT, EN1_PIN, PIN_OUTPUT);

	GPIO_writePin(IN1_PORT, IN1_PIN, LOGIC_LOW);
	GPIO_writePin(IN2_PORT, IN2_PIN, LOGIC_LOW);

	PWM_Timer0_Start(0);
}

/*
 *  Controls the motor's state (Clockwise/Anti-Clockwise/Stop)
 *  and adjusts the speed based on the input duty cycle.
 */
void DCMotor_Rotate(DCMotor_State state, uint8 speed)
{
	switch(state)
	{
		case Stop:
			GPIO_writePin(IN1_PORT, IN1_PIN, LOGIC_LOW);
			GPIO_writePin(IN2_PORT, IN2_PIN, LOGIC_LOW);
			break;
		case CW:
			GPIO_writePin(IN1_PORT, IN1_PIN, LOGIC_HIGH);
			GPIO_writePin(IN2_PORT, IN2_PIN, LOGIC_LOW);
			break;
		case A_CW:
			GPIO_writePin(IN1_PORT, IN1_PIN, LOGIC_LOW);
			GPIO_writePin(IN2_PORT, IN2_PIN, LOGIC_HIGH);
			break;
	}

	uint8 _duty = (uint8)(PWM_MAX_VALUE * speed / PWM_OFFSET);

	PWM_Timer0_Start(_duty);
}
