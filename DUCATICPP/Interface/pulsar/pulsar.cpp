/*
 * pulsar.cpp
 *
 *  Created on: Jan 26, 2024
 *      Author: Nguyen Tuan Phong
 * 		gmail: ntphong011102@gmail.com
 */

#include "pulsar.h"

pulsar *pulsar::instancePtr = nullptr;

pulsar::pulsar(/* args */)
{
}

pulsar::~pulsar()
{
}

void pulsar::modbusOnPower()
{
	HAL_GPIO_WritePin(config.powerPin.Port, config.powerPin.pin, GPIO_PIN_SET);
	HAL_Delay(3000);
}

/**
 * The function modbusOffPower turns off the power to the modbus device.
 */
/**
 * The function modbusOffPower turns off the power to the modbus device.
 */
void pulsar::modbusOffPower()
{
	HAL_GPIO_WritePin(config.powerPin.Port, config.powerPin.pin, GPIO_PIN_RESET);
}

// + 5V VCC(MAX485)
// │                    │
// ├────────────────────┤
// │                    │
// ├───┐                └── RO(Receiver Output)
// │   │
// │   │
// │   │
// │   └── DI(Driver Input)
// │
// ├────── DE(Driver Enable)
// │
// ├────── RE(Receiver Enable)
// │
// ├── A(A)A(RS485 twisted pair)
// │
// └── B(B)B(RS485 twisted pair)

/**
 * The function modbusTransmit sets the DE and RE pins to high.
 */
void pulsar::modbusTransmit()
{
	// modbusOnPower();
	HAL_GPIO_WritePin(config.DEPin.Port, config.DEPin.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(config.REPin.Port, config.REPin.pin, GPIO_PIN_SET);
}

/**
 * The function modbusReceive() sets the DE and RE pins to low.
 */
void pulsar::modbusReceive()
{
	// modbusOnPower();
	HAL_GPIO_WritePin(config.DEPin.Port, config.DEPin.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(config.REPin.Port, config.REPin.pin, GPIO_PIN_RESET);
}

/**
 * The function turns off the Modbus communication by resetting the DE pin and setting the RE pin.
 */
void pulsar::modbusOff()
{
	HAL_GPIO_WritePin(config.DEPin.Port, config.DEPin.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(config.REPin.Port, config.REPin.pin, GPIO_PIN_SET);
	// modbusOffPower();
}

/**
 * The function calculates a CRC (Cyclic Redundancy Check) value for a given data array using the
 * CRC-16 algorithm.
 *
 * @param data A pointer to an array of uint8_t (unsigned 8-bit integer) values, representing the data
 * for which the CRC (Cyclic Redundancy Check) needs to be calculated.
 * @param length The "length" parameter represents the number of bytes in the "data" array that you
 * want to calculate the CRC for.
 *
 * @return a uint16_t value, which is an unsigned 16-bit integer.
 */
uint16_t pulsar::calculateCRC(const uint8_t *data, uint8_t length)
{
	uint16_t crc = 0xFFFF;
	for (int i = 0; i < length; i++)
	{
		crc ^= data[i];
		for (int j = 0; j < 8; j++)
		{
			if (crc & 0x0001)
			{
				crc = (crc >> 1) ^ 0xA001;
			}
			else
			{
				crc = crc >> 1;
			}
		}
	}
	return crc;
}

/**
 * The SET_FLAG function sets the pulsarFlag variable to true.
 */
void pulsar::SET_FLAG()
{
	pulsarFlag = true;
}

/**
 * The function "RESET_FLAG" sets the value of the variable "pulsarFlag" to false.
 */
void pulsar::RESET_FLAG()
{
	pulsarFlag = false;
}

/**
 * The function GET_FLAG returns the value of the pulsarFlag variable.
 *
 * @return the value of the variable "pulsarFlag".
 */
bool pulsar::GET_FLAG()
{
	return pulsarFlag;
}

/**
 * The function sets the size of a pulsar object and returns true.
 *
 * @param size The size parameter is of type uint16_t, which is an unsigned integer that can hold
 * values from 0 to 65535.
 *
 * @return a boolean value, specifically `true`.
 */
bool pulsar::Get_size(uint16_t size)
{
	parent.size = size;
	HAL_UARTEx_ReceiveToIdle_IT(config.uart, (uint8_t *)parent.buffer, LEVEL_BUFFER_SIZE);
	return true;
}

/**
 * The function `pulsarSendCommand` sends a command over UART, receives a response, and checks if the
 * response is valid.
 *
 * @param command The `command` parameter is a pointer to an array of `uint8_t` (unsigned 8-bit
 * integer) values. It represents the command that needs to be sent to the Pulsar device.
 * @param size The parameter "size" represents the size of the command array, which is the number of
 * elements in the command array.
 * @param timeout The timeout parameter is the maximum time in milliseconds that the function will wait
 * for a response from the Pulsar device before considering it a failure.
 *
 * @return a boolean value.
 */
bool pulsar::pulsarSendCommand(uint8_t *command, uint8_t size, uint32_t timeout)
{
	HAL_UARTEx_ReceiveToIdle_IT(config.uart, (uint8_t *)parent.buffer, LEVEL_BUFFER_SIZE);
	modbusTransmit();
	bool answer = false;
	memset(parent.buffer, '\0', LEVEL_BUFFER_SIZE);
	HAL_UART_Transmit(config.uart, command, size, timeout);
	modbusReceive();
	uint32_t tickStart = HAL_GetTick();

	while (answer == false)
	{
		if (HAL_GetTick() - tickStart >= timeout)
		{
			LOGE("FALSE GET DATA FROM PULSAR", "");
			return false;
		}
		else
		{
			if (GET_FLAG())
			{
				uint16_t packetCRC = ((uint16_t)parent.buffer[parent.size - 1] << 8) |
									 (parent.buffer[parent.size - 2]);

				if (calculateCRC((uint8_t *)parent.buffer, parent.size - 2) == packetCRC)
				{
					LOGI("GET DATA FROM PULSAR", "");
					return true;
				}
			}
		}
	}

	modbusOff();
	RESET_FLAG();
	LOGW("ERROR GET DATA FROM PULSAR", "");
	return false;
}

bool pulsar::getFirmwareID()
{
	LOGI("GET FIRMWARE ID FROM PULSAR", "");
	uint8_t queryFrame[8] = {0x01, 0x03, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00};
	modbusReceive();

	uint16_t reponseCRC = calculateCRC(queryFrame, sizeof(queryFrame) - 2u);
	queryFrame[6] = (uint8_t)(reponseCRC & 0xFF);
	queryFrame[7] = (uint8_t)((reponseCRC >> 8) & 0xFF);

	pulsarSendCommand(queryFrame, sizeof(queryFrame), 1000);
	modbusOff();

	return true;
}

uint16_t pulsar::hardwareID()
{
	uint8_t queryFrame[8] = {0x01, 0x03, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00};
	modbusReceive();

	uint16_t reponseCRC = calculateCRC(queryFrame, sizeof(queryFrame) - 2u);
	queryFrame[6] = (uint8_t)(reponseCRC & 0xFF);
	queryFrame[7] = (uint8_t)((reponseCRC >> 8) & 0xFF);

	pulsarSendCommand(queryFrame, sizeof(queryFrame), 1000);
	modbusOff();

	LOG("Hard ware ID = %u", ((uint8_t)parent.buffer[3] << 8) | (uint8_t)parent.buffer[4]);

	return ((uint8_t)parent.buffer[3] << 8) | (uint8_t)parent.buffer[4];
}

uint16_t pulsar::gain()
{
	uint8_t queryFrame[8] = {0x7E, 0x03, 0x00, 0x66, 0x00, 0x01, 0x00, 0x00};
	modbusReceive();

	uint16_t reponseCRC = calculateCRC(queryFrame, sizeof(queryFrame) - 2u);
	queryFrame[6] = (uint8_t)(reponseCRC & 0xFF);
	queryFrame[7] = (uint8_t)((reponseCRC >> 8) & 0xFF);

	pulsarSendCommand(queryFrame, sizeof(queryFrame), 1000);
	modbusOff();

	return ((uint8_t)parent.buffer[3] << 8) | (uint8_t)parent.buffer[4];
}

uint16_t pulsar::serialNumber()
{
	LOGI("GET FLOW VELOCITY FROM PULSAR", "");
	uint8_t queryFrame[8] = {0x7E, 0x03, 0x00, 0xF0, 0x00, 0x02, 0x00, 0x00};
	modbusReceive();

	uint16_t reponseCRC = calculateCRC(queryFrame, sizeof(queryFrame) - 2u);
	queryFrame[6] = (uint8_t)(reponseCRC & 0xFF);
	queryFrame[7] = (uint8_t)((reponseCRC >> 8) & 0xFF);

	pulsarSendCommand(queryFrame, sizeof(queryFrame), 1000);
	modbusOff();

	return true;
}

float pulsar::getFlowVelocity()
{
	float flow = 0.0f;
	uint8_t queryFrame[8] = {0x7E, 0x03, 0x00, 0x20, 0x00, 0x02, 0x00, 0x00};
	modbusReceive();

	uint16_t reponseCRC = calculateCRC(queryFrame, sizeof(queryFrame) - 2u);
	queryFrame[6] = (uint8_t)(reponseCRC & 0xFF);
	queryFrame[7] = (uint8_t)((reponseCRC >> 8) & 0xFF);

	pulsarSendCommand(queryFrame, sizeof(queryFrame), 1000);
	modbusOff();

	flow = (((uint8_t)parent.buffer[3] << 8) | (uint8_t)parent.buffer[4]) + ((uint8_t)parent.buffer[4], ((uint8_t)parent.buffer[5] << 8) | (uint8_t)parent.buffer[6])/1000;
	LOG("Flow velocity = %f", flow);
	LOG("Data flow velocity = %02X %02X %02X %02X %02X %02X %02X", parent.buffer[0], parent.buffer[1], parent.buffer[2], parent.buffer[3], parent.buffer[4], parent.buffer[5], parent.buffer[6]);
	return flow;
}
