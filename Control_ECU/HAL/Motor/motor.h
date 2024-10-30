/******************************************************************************
*
* Module: DC Motor
*
* File Name: motor.h
*
* Description: Header file for the DC Motor driver
*
* Author: Abdallah El-Sakka
*
*******************************************************************************/

#ifndef MOTOR_MOTOR_H_
#define MOTOR_MOTOR_H_

#include "../../Utils/std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
*******************************************************************************/

#define IN1_PORT PORTD_ID
#define IN1_PIN  PIN6_ID

#define IN2_PORT PORTD_ID
#define IN2_PIN  PIN7_ID

#define EN1_PORT PORTB_ID
#define EN1_PIN  PIN3_ID

#define MAX_SPEED	(100)
#define MIN_SPEED	(0)

typedef enum
{
	Stop,	/* 0 */
	CW,		/* 1 */
	A_CW	/* 2 */
} DCMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
*******************************************************************************/

/*
 * Initializes the DC motor by setting the direction for the motor pins
 * and stopping the motor at the beginning
 */
void DCMotor_Init(void);

/*
 *  Controls the motor's state (Clockwise/Anti-Clockwise/Stop)
 *  and adjusts the speed based on the input duty cycle.
 */
void DCMotor_Rotate(DCMotor_State state, uint8 speed);

#endif /* MOTOR_MOTOR_H_ */
