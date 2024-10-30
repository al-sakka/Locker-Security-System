/******************************************************************************
*
* Module: PWM
*
* File Name: pwm.c
*
* Description: Source file for the PWM driver
*
* Author: Abdallah El-Sakka
*
*******************************************************************************/

#include <avr/io.h>
#include "../../MCAL/GPIO/gpio.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
*******************************************************************************/

/*
 * Initializes Timer0 in PWM mode
 * Prescaler: F_CPU/64
 * Non-inverting mode
 * The function configures OC0 as the output pin
 */
void PWM_Timer0_Start(uint8 duty_cycle)
{
	TCNT0 = 0;
	GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);
	TCCR0 |= (1 << WGM01) | (1 << WGM00) | (1 << COM01) | (1 << CS00) | (1 << CS01);

	/* Sets the required duty cycle for OCR0 */
	OCR0 = duty_cycle;

}

