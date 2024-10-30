/******************************************************************************
*
* Module: PIR
*
* File Name: pir.h
*
* Description: Header file for the PIR driver
*
* Author: Abdallah El-Sakka
*
*******************************************************************************/

#ifndef PIR_H_
#define PIR_H_

/*******************************************************************************
*                                Definitions                                  *
*******************************************************************************/

#define PIR_OUT_PORT_ID		(PORTC_ID)
#define PIR_OUT_PIN_ID		(PIN2_ID)

/*******************************************************************************
*                      Functions Prototypes                                   *
*******************************************************************************/

/*
 * Description: Function to initialize the PIR driver
 */
void PIR_init(void);

/*
 * Description: Function to return PIR State
 */
uint8 PIR_getState(void);

#endif /* PIR_H_ */
