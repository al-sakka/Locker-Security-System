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

#define COMPARE_VALUE		(58594UL)
#define ONE_MINUTE_TICKS	(8u)

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
