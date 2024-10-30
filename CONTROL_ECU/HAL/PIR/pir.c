/******************************************************************************
*
* Module: PIR
*
* File Name: pir.c
*
* Description: Source file for the PIR driver
*
* Author: Abdallah El-Sakka
*
*******************************************************************************/

#include "../../MCAL/GPIO/gpio.h"
#include "pir.h"

/*******************************************************************************
*                       Functions Definitions                                  *
*******************************************************************************/

/*
 * Description: Function to initialize the PIR driver
 */
void PIR_init(void)
{
	GPIO_setupPinDirection(PIR_OUT_PORT_ID, PIR_OUT_PIN_ID, PIN_INPUT);
}

/*
 * Description: Function to return PIR State
 */
uint8 PIR_getState(void)
{
	return	GPIO_readPin(PIR_OUT_PORT_ID, PIR_OUT_PIN_ID);
}


