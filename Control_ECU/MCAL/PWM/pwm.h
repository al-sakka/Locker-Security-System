/******************************************************************************
*
* Module: PWM
*
* File Name: pwm.h
*
* Description: Header file for the PWM driver
*
* Author: Abdallah El-Sakka
*
*******************************************************************************/

#ifndef PWM_PWM_H_
#define PWM_PWM_H_

/*******************************************************************************
 *                                Definitions                                  *
*******************************************************************************/

#define PWM_MAX_VALUE (255u)
#define PWM_OFFSET	  (100u)

/*******************************************************************************
 *                      Functions Prototypes                                   *
*******************************************************************************/

/*
 *  Sets the required duty cycle for OCR0
 */
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_PWM_H_ */
