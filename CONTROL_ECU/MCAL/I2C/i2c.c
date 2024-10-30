/******************************************************************************
*
* Module: TWI(I2C)
*
* File Name: i2c.c
*
* Description: Source file for the TWI(I2C) driver
*
* Author: Abdallah El-Sakka
*
*******************************************************************************/

#include "i2c.h"
#include <avr/io.h>
#include "../../Utils/common_macros.h"

/*******************************************************************************
*                      Functions Definitions                                  *
*******************************************************************************/

/*
 * Initialize TWI(I2C) Protocol
 */
void I2C_init(const I2C_ConfigType * Config_Ptr)
{
	uint8 twbr_value = (uint8)((CPU_FREQUENCY / Config_Ptr->bit_rate) - 16) / 2;

	TWBR = (twbr_value);
	TWSR = (INITIAL_VALUE_ZERO);

    /* Two Wire Bus address my address if any master device want to call me: 0x1 (used in case this MC is a slave device)
       General Call Recognition: Off */
    TWAR = Config_Ptr->address; // my address = 0x01 :)

    /* Enable TWI */
    SET_BIT(TWCR, TWEN);
}

/*
 * Start Sending
 */
void I2C_start(void)
{
    /*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

/*
 * Stop Sending
 */
void I2C_stop(void)
{
    /*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

/*
 * Write Data to be sent
 */
void I2C_writeByte(uint8 data)
{
    /* Put data On TWI data Register */
    TWDR = data;
    /*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

/*
 * Read received fata from slave using ACK technique
 */
uint8 I2C_readByteWithACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

/*
 * Read received fata from slave using NACK technique
 */
uint8 I2C_readByteWithNACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

/*
 *  Read status register
 */
uint8 I2C_getStatus(void)
{
    uint8 status;
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWSR & 0xF8;
    return status;
}
