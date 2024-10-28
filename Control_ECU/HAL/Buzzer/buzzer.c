/******************************************************************************
*
* Module: BUZZER
*
* File Name: buzzer.c
*
* Description: Source file for the buzzer driver
*
* Author: Abdallah El-Sakka
*
*******************************************************************************/

#include "../../MCAL/GPIO/gpio.h"
#include "buzzer.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
*******************************************************************************/

/*
 * Initializes the buzzer pin direction and turn off the buzzer.
 */
void Buzzer_init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, BUZZER_OFF);
}

/*
 * Activates the buzzer.
 */
void Buzzer_on(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, BUZZER_ON);
}

/*
 * Deactivates the buzzer
 */
void Buzzer_off(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, BUZZER_OFF);
}
