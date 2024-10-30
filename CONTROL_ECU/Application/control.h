/******************************************************************************
*
* Module: CONTROL ECU
*
* File Name: control.h
*
* Description: Header file for the CONTROL ECU
*
* Author: Abdallah El-Sakka
*
*******************************************************************************/

#ifndef APPLICATION_CONTROL_H_
#define APPLICATION_CONTROL_H_

#include "../Utils/std_types.h"

/*******************************************************************************
*                                Definitions                                  *
*******************************************************************************/

#define _15_MINUTES_TICKS	(2u)

typedef enum
{
	OFF,
	ON
} Door_state;

extern Door_state g_door_state;
extern bool timerRunning;

/*******************************************************************************
*                      Functions Prototypes                                   *
*******************************************************************************/

/*
 * Initialize CONTROL ECU
 */
void CONTROL_init(void);

/*
 * Main code logic in CONTROL ECU
 */
uint8 CONTROL_main(void);

#endif /* APPLICATION_CONTROL_H_ */
