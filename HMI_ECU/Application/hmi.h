/******************************************************************************
*
* Module: HMI ECU
*
* File Name: ecu.h
*
* Description: Header file for the HMI ECU
*
* Author: Abdallah El-Sakka
*
*******************************************************************************/

#ifndef HMI_H_
#define HMI_H_

#include "../Utils/std_types.h"

/*******************************************************************************
*                                Definitions                                  *
*******************************************************************************/

#define ONE_MINUTE_TICKS	(1)

/*******************************************************************************
*                      Functions Prototypes                                   *
*******************************************************************************/

/*
 * Initialize HMI ECU
 */
void HMI_init(void);

/*
 * Main code logic in HMI ECU
 */
uint8 HMI_main(void);

#endif	/* HMI_H_ */
