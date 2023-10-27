/*
 * sim7600.h
 *
 *  Created on: Oct 25, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#ifndef SIM7600_SIM7600_H_
#define SIM7600_SIM7600_H_

#include "sim.h"

#define SIM7600_POWER_ON(__ME__) HAL_GPIO_WritePin((__ME__)->powerEnPort, (__ME__)->powerEnPin, GPIO_PIN_SET)
#define SIM7600_POWER_OFF(__ME__) HAL_GPIO_WritePin((__ME__)->powerEnPort, (__ME__)->powerEnPin, GPIO_PIN_RESET)
#define SIM7600_PWRKEY_ON(__ME__)                                                     \
	do                                                                                \
	{                                                                                 \
		HAL_GPIO_WritePin((__ME__)->pwrkeyPort, (__ME__)->pwrkeyPin, GPIO_PIN_SET);   \
		HAL_Delay(1000UL);                                                            \
		HAL_GPIO_WritePin((__ME__)->pwrkeyPort, (__ME__)->pwrkeyPin, GPIO_PIN_RESET); \
		HAL_Delay(10000UL);                                                           \
	} while (0)
#define SIM7600_RESET_ON(__ME__)                                                    \
	do                                                                              \
	{                                                                               \
		HAL_GPIO_WritePin((__ME__)->resetPort, (__ME__)->resetPin, GPIO_PIN_SET);   \
		HAL_Delay(500UL);                                                           \
		HAL_GPIO_WritePin((__ME__)->resetPort, (__ME__)->resetPin, GPIO_PIN_RESET); \
		HAL_Delay(1000UL);                                                          \
	} while (0)

class SIM7600 : public SIM
{
private:
	/* data */
public:
	SIM7600(/* args */);
	~SIM7600();
	APP_StatusTypeDef SIM_sendATCommand(const char *command, const char *expect, const char *unexpect, uint32_t timeout);

	int8_t SIM_getRSSI();
	uint8_t SIM_getNwtype(char pNwtype[], uint8_t size);
	APP_StatusTypeDef SIM_getSimPhone(char pPhone[], uint8_t size);
	APP_StatusTypeDef SIM_getIMEI(char pIMEI[], uint8_t size);
	
	APP_StatusTypeDef SIM_callUSSD(const char *ussd, char response[], uint16_t size);
};

#endif /* SIM7600_SIM7600_H_ */
