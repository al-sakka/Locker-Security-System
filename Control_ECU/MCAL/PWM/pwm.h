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
 *  Initializes Timer0 in PWM mode
 * Prescaler: F_CPU/1024
 * Non-inverting mode
 * The function configures OC0 as the output pin
 */
void PWM_Timer0_init(void);

/*
 *  Sets the required duty cycle for OCR0
 */
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_PWM_H_ */
