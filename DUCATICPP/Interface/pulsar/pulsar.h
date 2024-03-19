/*
 * pulsar.h
 *
 *  Created on: Jan 26, 2024
 *      Author: Nguyen Tuan Phong
 * 		gmail: ntphong011102@gmail.com
 */

#ifndef PULSAR_PULSAR_H_
#define PULSAR_PULSAR_H_

#include "main.h"

#include <string.h>
#include <stdio.h>

#include <cstring> // Include the necessary header for strlen
#include "../logData/logData.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;

#define LEVEL_BUFFER_SIZE 99U

class pulsar
{
private:
	/* data */
	pulsar(/* args */);
	~pulsar();

	struct pulsarPin
	{
		GPIO_TypeDef *Port;
		uint16_t pin;
	};

	struct pulsarConfig
	{
		/* data */
		UART_HandleTypeDef *uart{&huart1};
		DMA_HandleTypeDef *dma{&hdma_usart1_rx};
		pulsarPin powerPin = {
			.Port = RS485_PWON_GPIO_Port,
			.pin = RS485_PWON_Pin};

		pulsarPin DEPin = {
			.Port = RS485_DE_GPIO_Port,
			.pin = RS485_DE_Pin};
		pulsarPin REPin = {
			.Port = RS485_RE_GPIO_Port,
			.pin = RS485_RE_Pin};
	};
	pulsarConfig config;

	struct parentData
	{
		/* data */
		char buffer[LEVEL_BUFFER_SIZE] = {0};
		uint8_t size = 0;
	};

	parentData parent;

	static pulsar *instancePtr;
	bool pulsarFlag = false;

	void modbusTransmit();
	void modbusReceive();
	void modbusOff();

	uint16_t calculateCRC(const uint8_t *data, uint8_t length);
	bool pulsarSendCommand(uint8_t *command, uint8_t size, uint32_t timeout);

public:
	static pulsar *getInstance()
	{
		if (instancePtr == nullptr)
		{
			instancePtr = new pulsar;
		}
		return instancePtr;
	}
	void modbusOnPower();
	void modbusOffPower();

	void SET_FLAG();
	void RESET_FLAG();
	bool GET_FLAG();
	bool Get_size(uint16_t size);

	bool getFirmwareID();
	uint16_t hardwareID();
	uint16_t gain();
	uint16_t serialNumber();
	float getFlowVelocity();
};

#endif /* PULSAR_PULSAR_H_ */
