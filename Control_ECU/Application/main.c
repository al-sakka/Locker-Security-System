#include "../HAL/EEPROM/eeprom.h"
#include "../MCAL/I2C/i2c.h"
#include "../Utils/std_types.h"
#include "../Utils/common_macros.h"
#include "../MCAL/UART/uart.h"
#include "../HAL/Buzzer/buzzer.h"
#include "../HAL/Motor/motor.h"
#include <string.h>
#include <util/delay.h>

int main(void)
{

	I2C_ConfigType I2C_conf = {BIT_RATE, MASTER_ADDRESS};
	UART_ConfigType UART_conf = {_8_bit, Disabled, _1_bit, 9600};

	Buzzer_init();
	DCMotor_Init();

	uint8 EEPROM_password[PASSWORD_SIZE + 1] = "";
	uint8 UART_password[PASSWORD_SIZE + 1] = "";
	uint8 receivedByte = 0;

	UART_init(&UART_conf);
	I2C_init(&I2C_conf);

	while(TRUE)
	{
		receivedByte = UART_receiveByte();
		UART_sendByte(0x10);

		if(receivedByte == NEW_PASS_MODE)
		{
			UART_receiveString(UART_password);
			EEPROM_writeString(PASSWORD_ADDRESS, UART_password);
			_delay_ms(10); // Allow EEPROM write to complete
		}
		else if(receivedByte == OPEN_DOOR_MODE)
		{
			UART_receiveString(UART_password);
			EEPROM_readString(PASSWORD_ADDRESS, EEPROM_password, PASSWORD_SIZE + 1);
			_delay_ms(10);

			bool passMatched = !(strcmp(EEPROM_password, UART_password));

			if(passMatched)
			{
				DCMotor_Rotate(CW, 100);
				Buzzer_off();
				UART_sendByte(0x11);
			}
			else
			{
				DCMotor_Rotate(Stop, 0);
				UART_sendByte(0x01);
			}
		}
		else if(receivedByte == CHANGE_PASS_MODE)
		{
			UART_receiveString(UART_password);
			EEPROM_readString(PASSWORD_ADDRESS, EEPROM_password, PASSWORD_SIZE + 1);
			_delay_ms(10);

			bool passMatched = !(strcmp(EEPROM_password, UART_password));

			if(passMatched)
			{
				UART_sendByte(0x11);
			}
			else
			{
				UART_sendByte(0x01);
			}
		}
		else if(receivedByte == ALARM_SIGNAL)
		{
			Buzzer_on();
		}

	}
}
