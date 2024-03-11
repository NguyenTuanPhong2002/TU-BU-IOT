/*
 * rfm95.c
 *
 *  Created on: Mar 24, 2023
 *      Author: Hoang Van Binh (gmail: binhhv.23.1.99@gmail.com)
 */

#include "rfm95.h"

#include <stdio.h>
#include <math.h>

/* Export functions */

/**
 * @brief Set NSS pin - this enable communication between RFM95 module and MCU
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 */
static inline void RFM95_setNSSPin(RFM95_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return;
	}
	HAL_GPIO_WritePin(me->nssPort, me->nssPin, GPIO_PIN_SET);
}

/**
 * @brief Reset NSS pin - this disable communication between RFM95 module and MCU
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 */
static inline void RFM95_clearNSSPin(RFM95_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return;
	}
	HAL_GPIO_WritePin(me->nssPort, me->nssPin, GPIO_PIN_RESET);
}

/**
 * @brief Write a single RFM95W register
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param address [in] Register address
 * @param data [in] The data of register
 * @return THT_StatusTypeDef
 */
static THT_StatusTypeDef RFM95_writeRegister(RFM95_HandleTypeDef *const me,
											 uint8_t address, const uint8_t data)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	/* The NSS pin goes low at the beginning of the frame */
	RFM95_clearNSSPin(me);
	/* an address byte followed by a data byte is sent for a write access */
	uint8_t txBuffer[2] = {address | 0x80, data};
	//	printf("Write %X to address %x\n", txBuffer[1], txBuffer[0]);
	HAL_StatusTypeDef res = HAL_SPI_Transmit(me->hspi, txBuffer, 2, 100);
	/* The NSS pin goes high after the data byte.*/
	RFM95_setNSSPin(me);

	if (res == HAL_OK)
	{
		return THT_OK;
	}
	else if (res == HAL_ERROR)
	{
		return THT_ERROR;
	}
	else
	{
		return THT_TIMEOUT;
	}
}

/**
 * @brief Write multiple register
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param address [in] Start register address
 * @param pData [out] A pointer to buffer to store the value of register
 * @param size [in] number register
 * @return THT_StatusTypeDef
 */
static THT_StatusTypeDef RFM95_writeBurstRegister(
	RFM95_HandleTypeDef *const me, uint8_t address, uint8_t *pData,
	uint8_t size)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	HAL_StatusTypeDef res = HAL_OK;

	if (size == 1)
	{
		res = RFM95_writeRegister(me, address, *pData);
	}
	else
	{
		RFM95_clearNSSPin(me);

		address |= 0x80;

		res = HAL_SPI_Transmit(me->hspi, &address, 1, 100);
		if (res == HAL_OK)
		{
			for (size_t i = 0; i < size; i++)
			{
				//				printf("Write %X to address %x\n", *pData, address);
				res = HAL_SPI_Transmit(me->hspi, pData++, 1, 100); // pdata++

				if (res != HAL_OK)
				{
					break;
				}
			}
		}

		RFM95_setNSSPin(me);
	}

	return res;
}

/**
 * @brief Read register
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param address [in] Register address
 * @param value [out] A pointer to a variable store the value of register
 * @return THT_StatusTypeDef
 */
static THT_StatusTypeDef RFM95_readRegister(RFM95_HandleTypeDef *const me, uint8_t address, uint8_t *value)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	/* The NSS pin goes low at the beginning of the frame */
	RFM95_clearNSSPin(me);

	HAL_StatusTypeDef res = HAL_OK;

	/* an address byte is sent */
	res = HAL_SPI_Transmit(me->hspi, &address, 1, 100);
	/* and a read byte is received for the read access. */
	res = HAL_SPI_Receive(me->hspi, value, 1, 100);

	/* The NSS pin goes high after the data byte. b*/
	RFM95_setNSSPin(me);
	//	printf("Read address %x: %x\n ", address, *value);
	if (res == HAL_OK)
	{
		return THT_OK;
	}
	else if (res == HAL_ERROR)
	{
		return THT_ERROR;
	}
	else
	{
		return THT_TIMEOUT;
	}
}
#if 0
static THT_StatusTypeDef RFM95_readBurstRegister(RFM95_HandleTypeDef *const me, uint8_t address, uint8_t *pBuffer, uint8_t size)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	HAL_StatusTypeDef res = HAL_OK;

	if (size == 1)
	{
		res = RFM95_readRegister(me, address, pBuffer);
	}
	else
	{
		RFM95_clearNSSPin(me);

		res = HAL_SPI_Transmit(me->hspi, &address, 1, 100);

		if (res == HAL_OK)
		{
			for (size_t i = 0; i < size; i++)
			{
				res = HAL_SPI_Receive(me->hspi, pBuffer++, 1, 100);

				if (res != HAL_OK)
				{
					break;
				}
			}
		}

		RFM95_setNSSPin(me);
	}
	return res;
}

#endif

/*
 * @arg RFLR_MODEMCONFIG1_IMPLICITHEADER_ON         0x01
 * @arg RFLR_MODEMCONFIG1_IMPLICITHEADER_OFF        0x00 // Default
 */

/**
 * @brief Set header LoRa mode
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param headerMode [in] Header mode
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_setHeaderMode(RFM95_HandleTypeDef *const me, const uint8_t headerMode)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	uint8_t rByte = 0;
	uint8_t wByte = 0;
	THT_StatusTypeDef res;

	/* Read current value of REG_LR_MODEMCONFIG1 register */
	res = RFM95_readRegister(me, REG_LR_MODEMCONFIG1, &rByte);

	if (res != THT_OK)
	{
		return res;
	}

	/* Set the new value of REG_LR_MODEMCONFIG1 register */
	wByte = (uint8_t)(headerMode | (rByte & RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK));

	/* Write new value to REG_LR_MODEMCONFIG2 register */
	res = RFM95_writeRegister(me, REG_LR_MODEMCONFIG1, wByte);

	return res;
}

/**
 * @brief Set spreading factor. Spreading Factor = Chips/Symbol = 2^SF (SF = 6-12)
 * @note SF must be set both side transmiter and receiver
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param SF [in] Spreading Factor
 *	@arg RFLR_MODEMCONFIG2_SF_6
	@arg RFLR_MODEMCONFIG2_SF_7                       // Default
	@arg RFLR_MODEMCONFIG2_SF_8
	@arg RFLR_MODEMCONFIG2_SF_9
	@arg RFLR_MODEMCONFIG2_SF_10
	@arg RFLR_MODEMCONFIG2_SF_11
	@arg RFLR_MODEMCONFIG2_SF_12                      // Phải cấu hình thêm LowDataRate
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_setSpreadingFactor(RFM95_HandleTypeDef *const me, const uint8_t SF)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	me->parent.SF = SF >> 4u;

	uint8_t rByte = 0;
	uint8_t wByte = 0;
	THT_StatusTypeDef res;

	/* Read current value of REG_LR_MODEMCONFIG2 register */
	res = RFM95_readRegister(me, REG_LR_MODEMCONFIG2, &rByte);
	if (res != THT_OK)
	{
		return THT_ERROR;
	}

	/* SF = 6 Is a special use case for the highest data rate transmission possible with the LoRa modem */
	if (SF == RFLR_MODEMCONFIG2_SF_6)
	{
		/* Set the new value of REG_LR_MODEMCONFIG2 register */
		wByte = (uint8_t)(RFLR_MODEMCONFIG2_SF_6 | (rByte & RFLR_MODEMCONFIG2_SF_MASK));

		/* 1. Write new value to REG_LR_MODEMCONFIG2 register */
		res = RFM95_writeRegister(me, REG_LR_MODEMCONFIG2, wByte);

		if (res != THT_OK)
		{
			return res;
		}

		/* 2. Set the header to implicit mode */
		RFM95_setHeaderMode(me, RFLR_MODEMCONFIG1_IMPLICITHEADER_ON);

		/* 3. Write bit 2-0 of register 0x31 to value "0b101" */
		res = RFM95_readRegister(me, REG_LR_DETECTOPTIMIZE, &rByte);
		if (res != THT_OK)
		{
			return THT_ERROR;
		}

		wByte = (uint8_t)(RFLR_DETECTIONOPTIMIZE_SF6 | (rByte & RFLR_DETECTIONOPTIMIZE_MASK));

		res = RFM95_writeRegister(me, REG_LR_DETECTOPTIMIZE, wByte);

		if (res != THT_OK)
		{
			return res;
		}

		/* 4. Write register address 0x37 to value 0x0C */
		return RFM95_writeRegister(me, REG_LR_DETECTIONTHRESHOLD, RFLR_DETECTIONTHRESH_SF6);
	}

	/* else if SF != 6 following below */
	/* 1. Write bit 2-0 of register 0x31 to value "0b101" */
	res = RFM95_readRegister(me, REG_LR_DETECTOPTIMIZE, &rByte);
	if (res != THT_OK)
	{
		return THT_ERROR;
	}

	wByte = (uint8_t)(RFLR_DETECTIONOPTIMIZE_SF7_TO_SF12 | (rByte & RFLR_DETECTIONOPTIMIZE_MASK));

	res = RFM95_writeRegister(me, REG_LR_DETECTOPTIMIZE, wByte);

	if (res != THT_OK)
	{
		return res;
	}

	/* 2. Write register address 0x37 to value 0x0A */
	res = RFM95_writeRegister(me, REG_LR_DETECTIONTHRESHOLD, RFLR_DETECTIONTHRESH_SF7_TO_SF12);

	if (res != THT_OK)
	{
		return res;
	}

	/* 3. Write register REG_LR_MODEMCONFIG2 to config SF */

	/* Set the new value of REG_LR_MODEMCONFIG2 register */
	wByte = (uint8_t)(SF | (rByte & RFLR_MODEMCONFIG2_SF_MASK));
	/* 1. Write new value to REG_LR_MODEMCONFIG2 register */
	//	return RFM95_writeRegister(me, REG_LR_MODEMCONFIG2, wByte);
	res = RFM95_writeRegister(me, REG_LR_MODEMCONFIG2, wByte);

	if (SF == RFLR_MODEMCONFIG2_SF_12 || SF == RFLR_MODEMCONFIG2_SF_11)
	{
		return RFM95_lowDataRateOptimize(me);
	}

	return res;
	//	return RFM95_readRegister(me, REG_LR_MODEMCONFIG2, &rByte);
}

/**
 * @brief Set coding rate
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param CR [in] Coding rate value
 * 	@arg RFLR_MODEMCONFIG1_CODINGRATE_4_5 0x02
	@arg RFLR_MODEMCONFIG1_CODINGRATE_4_6 0x04 // Default
	@arg RFLR_MODEMCONFIG1_CODINGRATE_4_7 0x06
	@arg RFLR_MODEMCONFIG1_CODINGRATE_4_8 0x08
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_setCodingRate(RFM95_HandleTypeDef *const me, uint8_t CR)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	me->parent.CR = CR >> 1u;

	uint8_t rByte = 0;
	uint8_t wByte = 0;
	THT_StatusTypeDef res;

	/* Read current value of REG_LR_MODEMCONFIG1 register */
	res = RFM95_readRegister(me, REG_LR_MODEMCONFIG1, &rByte);

	if (res != THT_OK)
	{
		return THT_ERROR;
	}

	wByte = (uint8_t)(CR | (rByte & RFLR_MODEMCONFIG1_CODINGRATE_MASK));

	return RFM95_writeRegister(me, REG_LR_MODEMCONFIG1, wByte);
}

/**
 * @brief Set bandwidth
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param BW [in] Bandwidth value
 *  @arg RFLR_MODEMCONFIG1_BW_125_KHZ 0x70 // Default
	@arg RFLR_MODEMCONFIG1_BW_250_KHZ 0x80
	@arg RFLR_MODEMCONFIG1_BW_500_KHZ 0x90
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_setBandwidth(RFM95_HandleTypeDef *const me, uint8_t BW)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	me->parent.BW = BW >> 4u;

	uint8_t rByte = 0;
	uint8_t wByte = 0;
	THT_StatusTypeDef res;

	/* Read current value of REG_LR_MODEMCONFIG1 register */
	res = RFM95_readRegister(me, REG_LR_MODEMCONFIG1, &rByte);

	if (res != THT_OK)
	{
		return THT_ERROR;
	}

	wByte = (uint8_t)(BW | (rByte & RFLR_MODEMCONFIG1_BW_MASK));

	return RFM95_writeRegister(me, REG_LR_MODEMCONFIG1, wByte);
	//	RFM95_writeRegister(me, REG_LR_MODEMCONFIG1, wByte);
	//	return RFM95_readRegister(me, REG_LR_MODEMCONFIG1, &rByte);
}

/**
 * @brief Set CRC mode
 * @note Only set tranmit side is enough
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param CRCMode [in] CRC mode
 *  @arg RFLR_MODEMCONFIG2_RXPAYLOADCRC_ON
 * 	@arg RFLR_MODEMCONFIG2_RXPAYLOADCRC_OFF
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_setCRCMode(RFM95_HandleTypeDef *const me, uint8_t CRCMode)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	uint8_t rByte = 0;
	uint8_t wByte = 0;
	THT_StatusTypeDef res;

	/* Read current value of REG_LR_MODEMCONFIG2 register */
	res = RFM95_readRegister(me, REG_LR_MODEMCONFIG2, &rByte);

	if (res != THT_OK)
	{
		return THT_ERROR;
	}

	wByte = (uint8_t)(CRCMode | (rByte & RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK));

	return RFM95_writeRegister(me, REG_LR_MODEMCONFIG2, wByte);
}

/**
 * @brief Set low data rate
 * @note When SF is 11 or 12, need to be set low data rate optimize to reduce consumption
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_lowDataRateOptimize(RFM95_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}
	uint32_t bandwidth = 0;
	switch (me->parent.BW)
	{
	case 7:
		bandwidth = 125000;
		break;
	case 8:
		bandwidth = 250000;
		break;
	case 9:
		bandwidth = 500000;
		break;
	default:
		break;
	}

	float Tsymbol = 1000.0 * pow(2, me->parent.SF) / bandwidth;
	uint8_t isEnableLDO = 0;

	if (Tsymbol > 16.0)
	{
		isEnableLDO = RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_ON;
	}
	else
	{
		isEnableLDO = RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_OFF;
	}

	uint8_t rByte = 0;
	uint8_t wByte = 0;
	THT_StatusTypeDef res;

	res = RFM95_readRegister(me, REG_LR_MODEMCONFIG3, &rByte);

	if (res != THT_OK)
	{
		return THT_ERROR;
	}

	wByte = (uint8_t)(isEnableLDO | (rByte & RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK));
	wByte = (uint8_t)(RFLR_MODEMCONFIG3_AGCAUTO_ON | wByte); // turn on AGC
	return RFM95_writeRegister(me, REG_LR_MODEMCONFIG3, wByte);
	//	RFM95_writeRegister(me, REG_LR_MODEMCONFIG3, wByte);
	//	return RFM95_readRegister(me, REG_LR_MODEMCONFIG3, &rByte);
}

/**
 * @brief Set LNA
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_setLNA(RFM95_HandleTypeDef *const me)
{
	uint8_t rByte = 0;
	uint8_t wByte = 0;
	THT_StatusTypeDef res;

	res = RFM95_readRegister(me, REG_LR_LNA, &rByte);

	if (res != THT_OK)
	{
		return THT_ERROR;
	}

	wByte = (uint8_t)(RFLR_LNA_BOOST_HF_ON | (rByte & RFLR_LNA_BOOST_HF_MASK));

	return RFM95_writeRegister(me, REG_LR_LNA, wByte);
	//	RFM95_writeRegister(me, REG_LR_LNA, wByte);
	//	return RFM95_readRegister(me, REG_LR_LNA, &rByte);
}

/**
 * @brief
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_FHSS(RFM95_HandleTypeDef *const me)
{
	return THT_ERROR;
}

/*
 * Tính toán time-on-air
 * Symbol rate: Rs = BW / 2^SF
 * Symbol Time: Ts = 1 / Rs = 2^SF / BW
 * PHY payload size    : 8 Bytes
 * Ex: SF = 12, BW = 500kHz, Rs = 122.070 symbol/s, Ts = 1 / Rs = 8.192 ms/symbol
MAC payload size    : 3 Bytes
Spreading Factor    : 12
Band width          : 500 kHz
Low data rate opt.  : disable
Explicit header     : disable
CR (coding rate)    : 1 (4/5)
Symbol Rate         : 122.070 symbol/s
Symbol Time         : 8.192 msec/symbol
Preamble size       : 6 symbols
Packet symbol size  : 13 symbols
Preamble ToA        : 83.968 msec
Payload ToA         : 106.496 msec
Time on Air         : 190.464 msec
RAW data rate       : 1171.875 bps
Duty Cycle          : 1 %
Min span of a cycle : 19.046 sec
Max Frames per day  : 4536 frames
 */

/**
 * @brief Calculate the time on air
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @return float
 */
float RFM95_CalculateTimeOnAir(RFM95_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return 0.0f;
	}

	float timeOnAir = 0.0f;
	float preambleToA = 0.0f;
	float payloadToA = 0.0f;

	float Tsymbol = pow(2, me->parent.SF) / me->parent.BW;

	uint16_t nPreamble = 1u;
	preambleToA = nPreamble * Tsymbol;

	uint16_t nPayload = 1u;
	uint16_t payloadBytes = 1u;

	nPayload = (8 * payloadBytes - 4 * me->parent.SF + 28 + 16);

	if (nPayload < 0)
	{
		nPayload = 0;
	}

	nPayload += 8;

	payloadToA = nPayload * Tsymbol;

	timeOnAir = preambleToA + payloadToA;

	return timeOnAir;
}

/**
 * @brief Set mode
 * @note In standby mode or sleep mode: can read and write registers. Other mode: only read registers
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param mode [in] Mode is combine of LORA, SLEEP, STANDBY, SYNTHESIZERTX, TRANSMITTER, RECEIVER, SYNTHESIZERRX
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_setMode(RFM95_HandleTypeDef *const me, const uint8_t mode)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	uint8_t rByte = 0;
	uint8_t wByte = 0;
	THT_StatusTypeDef res;

	/* Read current value of REG_LR_MODEMCONFIG1 register */
	res = RFM95_readRegister(me, REG_LR_OPMODE, &rByte);

	if (res != THT_OK)
	{
		return THT_ERROR;
	}

	wByte = (uint8_t)(mode | (rByte & 0x70));

	return RFM95_writeRegister(me, REG_LR_OPMODE, wByte);
}

/**
 * @brief Set frequency 815-923
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param frequency frequency value, ex: 923
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_setFrequency(RFM95_HandleTypeDef *const me, uint32_t frequency)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	me->parent.freq = frequency;
	uint64_t frf = (uint64_t)(frequency << 19u) / 32;

	uint8_t freq[3] = {(uint8_t)(frf >> 16), (uint8_t)(frf >> 8),
					   (uint8_t)(frf >> 0)};

	return RFM95_writeBurstRegister(me, REG_LR_FRFMSB, freq, 3);
}

/**
 * @brief set Preamble
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param preamble preamble value
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_setPreamble(RFM95_HandleTypeDef *const me, const uint16_t preamble)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	me->parent.preamble = preamble;

	uint8_t preambleArr[2] = {(uint8_t)((preamble & 0xFF00) >> 8), (uint8_t)((preamble && 0xFF) >> 0)};

	return RFM95_writeBurstRegister(me, REG_LR_PREAMBLEMSB, preambleArr, 2);
}

/**
 * @brief Set OCP
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_setOCP(RFM95_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	uint8_t wByte = 0;
	// 00110001

	wByte = (uint8_t)(RFLR_OCP_ON | RFLR_OCP_TRIM_140_MA);

	return RFM95_writeRegister(me, REG_LR_OCP, wByte);
}

/**
 * @brief Send packet
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param pData [in] A pointer to buffer stored data
 * @param size [in] The size of buffer
 * @param timeout [in] The timeout to terminate transmit
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_transmit(RFM95_HandleTypeDef *const me, uint8_t *pData, uint8_t size, uint32_t timeout)
{
	if (me == NULL || pData == NULL)
	{
		return THT_ERROR;
	}

	uint8_t rByte = 0;
	uint8_t wByte = 0;
	uint8_t currentMode = 0;

	/* 0. Tx init */
	/* Read current value of REG_LR_MODEMCONFIG1 register */
	RFM95_readRegister(me, REG_LR_OPMODE, &currentMode);

	/* 1. Static configuration registers can only be accessed in Sleep mode, Stand-by mode or FSTX mode. */
	wByte = (uint8_t)(RFLR_OPMODE_STANDBY | (currentMode & RFLR_OPMODE_MASK));

	RFM95_writeRegister(me, REG_LR_OPMODE, wByte);

	if (me->parent.SF == RFLR_MODEMCONFIG2_SF_6 >> 4)
	{
		RFM95_setHeaderMode(me, RFLR_MODEMCONFIG1_IMPLICITHEADER_ON);
	}

	/* 2. The LoRaTM FIFO can only be filled in Stand-by mode. */
	RFM95_readRegister(me, REG_LR_FIFOTXBASEADDR, &rByte);

	RFM95_writeRegister(me, REG_LR_FIFOADDRPTR, rByte);

	RFM95_writeRegister(me, REG_LR_PAYLOADLENGTH, size);
	uint8_t temp = 0;
	RFM95_readRegister(me, REG_LR_PAYLOADLENGTH, &temp);

	(void)temp;

	for (size_t i = 0; i < size; i++)
	{
		RFM95_writeRegister(me, REG_LR_FIFO, *(pData++));
	}

	/* 3. Data transmission is initiated by sending TX mode request. */
	wByte = (uint8_t)(RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_TRANSMITTER);

	RFM95_writeRegister(me, REG_LR_OPMODE, wByte);

	HAL_Delay(1000);
	/* 4. Upon completion the TxDone interrupt is issued and the radio returns to Stand-by mode. */
	uint32_t tickStart = HAL_GetTick();

	while (HAL_GetTick() - tickStart <= timeout)
	{
		RFM95_readRegister(me, REG_LR_IRQFLAGS, &rByte);

		if ((rByte & RFLR_IRQFLAGS_TXDONE) == RFLR_IRQFLAGS_TXDONE)
		{
			/* Clear TX DONE flag by write 1 */
			wByte = (uint8_t)(RFLR_IRQFLAGS_TXDONE | (rByte & RFLR_IRQFLAGS_TXDONE_MASK));
			RFM95_writeRegister(me, REG_LR_IRQFLAGS, wByte);
			RFM95_readRegister(me, REG_LR_IRQFLAGS, &rByte);

			wByte = (uint8_t)(RFLR_OPMODE_STANDBY | (currentMode & RFLR_OPMODE_MASK));
			return RFM95_writeRegister(me, REG_LR_OPMODE, wByte);
		}
	}

	return THT_TIMEOUT;
}

/**
 * @brief Enable receive packet continuous using intterupt
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_startReceiveIT(RFM95_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}
	/* Mapping DIO0 to rxDone */
	RFM95_mappingDIO0(me, RFLR_DIOMAPPING1_DIO0_00);

	uint8_t rByte = 0;
	uint8_t wByte = 0;

	RFM95_readRegister(me, REG_LR_DETECTOPTIMIZE, &rByte);

	if (me->parent.BW == LORA_BW_500)
	{
		wByte = (0x80 | (rByte & 0x7F));
	}
	else
	{
		wByte = (0x00 | (rByte & 0x7F));
	}

	RFM95_writeRegister(me, REG_LR_DETECTOPTIMIZE, wByte);

	switch (me->parent.BW)
	{
	case LORA_BW_7D8:
		RFM95_writeRegister(me, 0x27, 0x48);
		RFM95_writeRegister(me, 0x3F, 0x00);
		break;
	case LORA_BW_10D4:
	case LORA_BW_15D6:
	case LORA_BW_20D8:
	case LORA_BW_31D25:
	case LORA_BW_41D7:
		RFM95_writeRegister(me, 0x27, 0x44);
		RFM95_writeRegister(me, 0x3F, 0x00);
		break;
	case LORA_BW_62D5:
	case LORA_BW_125:
	case LORA_BW_250:
		RFM95_writeRegister(me, 0x27, 0x40);
		RFM95_writeRegister(me, 0x3F, 0x00);
		break;
	case LORA_BW_500:
	default:
		break;
	}

	wByte = (uint8_t)(RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_RECEIVER);

	return RFM95_writeRegister(me, REG_LR_OPMODE, wByte);
}

/**
 * @brief Receive packet on interrupt mode
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param pBuffer [out] A pointer to buffer used to store data
 * @param size [in] The size of the buffer
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_receiveIT(RFM95_HandleTypeDef *const me, uint8_t pBuffer[], uint8_t size)
{
	if (me == NULL || pBuffer == NULL)
	{
		return THT_ERROR;
	}
	uint8_t rByte = 0;
	uint8_t wByte = 0;

	RFM95_setMode(me, RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_STANDBY);

	RFM95_readRegister(me, REG_LR_IRQFLAGS, &rByte);

	if ((rByte & RFLR_IRQFLAGS_RXTIMEOUT) == RFLR_IRQFLAGS_RXTIMEOUT)
	{
		/* Clear RFLR_IRQFLAGS_RXTIMEOUT flag by write 1 */
		wByte = (uint8_t)(RFLR_IRQFLAGS_RXTIMEOUT | (rByte & RFLR_IRQFLAGS_RXTIMEOUT_MASK));
		RFM95_writeRegister(me, REG_LR_IRQFLAGS, wByte);
	}
	else if ((rByte & RFLR_IRQFLAGS_RXDONE) == RFLR_IRQFLAGS_RXDONE)
	{
		/* Clear RFLR_IRQFLAGS_RXDONE flag by write 1 */
		wByte = (uint8_t)(RFLR_IRQFLAGS_RXDONE | (rByte & RFLR_IRQFLAGS_RXDONE_MASK));
		RFM95_writeRegister(me, REG_LR_IRQFLAGS, wByte);

		RFM95_readRegister(me, REG_LR_IRQFLAGS, &rByte);

		/* Payload CRC not error - read data */
		if ((rByte & RFLR_IRQFLAGS_PAYLOADCRCERROR) != RFLR_IRQFLAGS_PAYLOADCRCERROR)
		{
			/* Read first byte position of last packet */
			RFM95_readRegister(me, REG_LR_FIFORXCURRENTADDR, &rByte);

			/* */
			RFM95_writeRegister(me, REG_LR_FIFOADDRPTR, rByte);

			RFM95_readRegister(me, REG_LR_RXNBBYTES, &rByte);
			/* May be use read burst or fifo read */

			for (size_t i = 0; i < (rByte < size ? rByte : size); i++)
			{
				RFM95_readRegister(me, REG_LR_FIFO, pBuffer++);
			}

			return THT_OK;
		}
		else
		{
			/* Clear RFLR_IRQFLAGS_PAYLOADCRCERROR flag by write 1 */
			wByte = (uint8_t)(RFLR_IRQFLAGS_PAYLOADCRCERROR | (rByte & RFLR_IRQFLAGS_PAYLOADCRCERROR_MASK));
			RFM95_writeRegister(me, REG_LR_IRQFLAGS, wByte);
		}
	}

	return THT_ERROR;
}

#if 0
THT_StatusTypeDef RFM95_receiveSingle(RFM95_HandleTypeDef * const me, uint8_t pBuffer[], uint8_t size, uint32_t timeout)
{
	if (me == NULL || pBuffer == NULL)
	{
		return THT_ERROR;
	}

	uint8_t rByte = 0;
	uint8_t wByte = 0;
	uint8_t currentMode = 0;

	RFM95_readRegister(me, REG_LR_OPMODE, &currentMode);

	/* Nếu đang trong chế độ RX single */
	if ((currentMode & RFLR_OPMODE_RECEIVER_SINGLE) != RFLR_OPMODE_RECEIVER_SINGLE)
	{
		return THT_ERROR;
	}
	/* 1. Set FifoPtrAddr to FifoRxPtrBase */
	RFM95_readRegister(me, REG_LR_FIFORXBASEADDR, &rByte);

	RFM95_writeRegister(me, REG_LR_FIFOADDRPTR, rByte);

	/* Static configuration register device can be written in either Sleep mode, Stand-by mode or FSRX mode. */
	RFM95_readRegister(me, REG_LR_FIFORXBASEADDR, &rByte);

	wByte = (uint8_t) (RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_RECEIVER_SINGLE);

	RFM95_writeRegister(me, REG_LR_OPMODE, wByte);

	uint32_t tickStart = HAL_GetTick();

	while (HAL_GetTick() - tickStart <= timeout)
	{
		RFM95_readRegister(me, REG_LR_IRQFLAGS, &rByte);

		if ((rByte & RFLR_IRQFLAGS_RXTIMEOUT) == RFLR_IRQFLAGS_RXTIMEOUT)
		{
			/* Clear RFLR_IRQFLAGS_RXTIMEOUT flag by write 1 */
			wByte = (uint8_t) (RFLR_IRQFLAGS_RXTIMEOUT | (rByte & RFLR_IRQFLAGS_RXTIMEOUT_MASK));
			RFM95_writeRegister(me, REG_LR_IRQFLAGS, wByte);

			return RFM95_writeRegister(me, REG_LR_OPMODE, currentMode);
		}
		else if ((rByte & RFLR_IRQFLAGS_RXDONE) == RFLR_IRQFLAGS_RXDONE)
		{
			/* Clear RFLR_IRQFLAGS_RXDONE flag by write 1 */
			wByte = (uint8_t) (RFLR_IRQFLAGS_RXDONE | (rByte & RFLR_IRQFLAGS_RXDONE_MASK));
			RFM95_writeRegister(me, REG_LR_IRQFLAGS, wByte);

			RFM95_readRegister(me, REG_LR_IRQFLAGS, &rByte);

			/* Payload CRC not error - read data */
			if ((rByte & RFLR_IRQFLAGS_PAYLOADCRCERROR) != RFLR_IRQFLAGS_PAYLOADCRCERROR)
			{
				/* May be use read burst or fifo read */
				for (size_t i = 0; i < size; i++)
				{
					RFM95_readRegister(me, REG_LR_FIFO, pBuffer++);
				}
			}
			else
			{
				/* Clear RFLR_IRQFLAGS_PAYLOADCRCERROR flag by write 1 */
				wByte = (uint8_t) (RFLR_IRQFLAGS_PAYLOADCRCERROR | (rByte & RFLR_IRQFLAGS_PAYLOADCRCERROR_MASK));
				RFM95_writeRegister(me, REG_LR_IRQFLAGS, wByte);
			}

			return RFM95_writeRegister(me, REG_LR_OPMODE, currentMode);
		}
	}

	return THT_TIMEOUT;
}
#endif

/**
 * @brief Receive packet continous polling and terminate when timeout occur
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param pBuffer [out]A pointer to buffer used to store data
 * @param size [in] The size of the bufffer
 * @param timeout [in] Timeout
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_receiveContinuous(RFM95_HandleTypeDef *const me, uint8_t pBuffer[], uint8_t size, uint32_t timeout)
{
	if (me == NULL || pBuffer == NULL)
	{
		return THT_ERROR;
	}

	uint8_t rByte = 0;
	uint8_t wByte = 0;
	uint8_t currentMode = 0;

	RFM95_readRegister(me, REG_LR_OPMODE, &currentMode);

	RFM95_readRegister(me, REG_LR_DETECTOPTIMIZE, &rByte);

	if (me->parent.BW == LORA_BW_500)
	{
		wByte = (0x80 | (rByte & 0x7F));
	}
	else
	{
		wByte = (0x00 | (rByte & 0x7F));
	}

	RFM95_writeRegister(me, REG_LR_DETECTOPTIMIZE, wByte);

	switch (me->parent.BW)
	{
	case LORA_BW_7D8:
		RFM95_writeRegister(me, 0x27, 0x48);
		RFM95_writeRegister(me, 0x3F, 0x00);
		break;
	case LORA_BW_10D4:
	case LORA_BW_15D6:
	case LORA_BW_20D8:
	case LORA_BW_31D25:
	case LORA_BW_41D7:
		RFM95_writeRegister(me, 0x27, 0x44);
		RFM95_writeRegister(me, 0x3F, 0x00);
		break;
	case LORA_BW_62D5:
	case LORA_BW_125:
	case LORA_BW_250:
		RFM95_writeRegister(me, 0x27, 0x40);
		RFM95_writeRegister(me, 0x3F, 0x00);
		break;
	case LORA_BW_500:
	default:
		break;
	}

	wByte = (uint8_t)(RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_RECEIVER);

	RFM95_writeRegister(me, REG_LR_OPMODE, wByte);

	uint32_t tickStart = HAL_GetTick();

	while (HAL_GetTick() - tickStart <= timeout)
	{
		RFM95_readRegister(me, REG_LR_IRQFLAGS, &rByte);

		if ((rByte & RFLR_IRQFLAGS_RXTIMEOUT) == RFLR_IRQFLAGS_RXTIMEOUT)
		{
			RFM95_writeRegister(me, REG_LR_OPMODE, currentMode);
			/* Clear RFLR_IRQFLAGS_RXTIMEOUT flag by write 1 */
			wByte = (uint8_t)(RFLR_IRQFLAGS_RXTIMEOUT | (rByte & RFLR_IRQFLAGS_RXTIMEOUT_MASK));
			RFM95_writeRegister(me, REG_LR_IRQFLAGS, wByte);
		}
		else if ((rByte & RFLR_IRQFLAGS_RXDONE) == RFLR_IRQFLAGS_RXDONE)
		{
			RFM95_writeRegister(me, REG_LR_OPMODE, currentMode);

			/* Clear RFLR_IRQFLAGS_RXDONE flag by write 1 */
			wByte = (uint8_t)(RFLR_IRQFLAGS_RXDONE | (rByte & RFLR_IRQFLAGS_RXDONE_MASK));
			RFM95_writeRegister(me, REG_LR_IRQFLAGS, wByte);

			RFM95_readRegister(me, REG_LR_IRQFLAGS, &rByte);

			/* Payload CRC not error - read data */
			if ((rByte & RFLR_IRQFLAGS_PAYLOADCRCERROR) != RFLR_IRQFLAGS_PAYLOADCRCERROR)
			{
				RFM95_writeRegister(me, REG_LR_OPMODE, currentMode);

				/* Read first byte position of last packet */
				RFM95_readRegister(me, REG_LR_FIFORXCURRENTADDR, &rByte);

				/* */
				RFM95_writeRegister(me, REG_LR_FIFOADDRPTR, rByte);

				RFM95_readRegister(me, REG_LR_RXNBBYTES, &rByte);
				/* May be use read burst or fifo read */

				for (size_t i = 0; i < (rByte < size ? rByte : size); i++)
				{
					RFM95_readRegister(me, REG_LR_FIFO, pBuffer++);
				}

				return THT_OK;
			}
			else
			{
				RFM95_writeRegister(me, REG_LR_OPMODE, currentMode);
				/* Clear RFLR_IRQFLAGS_PAYLOADCRCERROR flag by write 1 */
				wByte = (uint8_t)(RFLR_IRQFLAGS_PAYLOADCRCERROR | (rByte & RFLR_IRQFLAGS_PAYLOADCRCERROR_MASK));
				RFM95_writeRegister(me, REG_LR_IRQFLAGS, wByte);
			}
		}
	}

	return THT_TIMEOUT;
}

/**
 * @brief Reset module
 * @note When module is power on, call this function to reset the module
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 */
void RFM95_reset(RFM95_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return;
	}

	HAL_GPIO_WritePin(me->resetPort, me->resetPin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(me->resetPort, me->resetPin, GPIO_PIN_SET);
	HAL_Delay(10);

	uint8_t rByte = 0;
	RFM95_readRegister(me, REG_LR_VERSION, &rByte);

	if (rByte != 0x12)
	{
		//		while(1);
	}
}

#if 0
/**
 * @brief
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_channelActivityDetection(RFM95_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	return THT_OK;
}
#endif

/**
 * @brief Reset the FIFO base address register
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_resetFIFOBaseAddress(RFM95_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	/* Default is rx: 0x00, tx: 0x80 */
	RFM95_writeRegister(me, REG_LR_FIFOTXBASEADDR, RFLR_FIFOTXBASEADDR);
	RFM95_writeRegister(me, REG_LR_FIFORXBASEADDR, RFLR_FIFORXBASEADDR);

	return THT_OK;
}

/**
 * @brief Set power
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param paSelect [in] paSelect state
 *  @arg RFLR_PACONFIG_PASELECT_PABOOST
 *  @arg RFLR_PACONFIG_PASELECT_RFO
 * @param Pout Power value
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_setRFPowerAmplifier(RFM95_HandleTypeDef *const me, uint8_t paSelect, uint8_t Pout)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	uint8_t wByte = 0;
	uint8_t outputPower = 0;
	if (paSelect == RFLR_PACONFIG_PASELECT_PABOOST &&
		((Pout >= 15) && (Pout <= 20)))
	{
		if (Pout > 17)
		{
			RFM95_writeRegister(me, REG_LR_PADAC, RFLR_PADAC_20DBM_ON);
			Pout = 17;
		}

		outputPower = Pout - 17 + 15;
		wByte = (uint8_t)(paSelect | outputPower);
		return RFM95_writeRegister(me, REG_LR_PACONFIG, wByte);
	}
	else if (paSelect == RFLR_PACONFIG_PASELECT_RFO &&
			 ((Pout >= 0) && (Pout <= 14)))
	{
		uint8_t maxPower = 7; // chọn 7 để số tròn, hoặc có thể là 2, giá trị mặc định là 4
		outputPower = Pout;
		wByte = (uint8_t)(paSelect | (maxPower << 4) | outputPower);
		return RFM95_writeRegister(me, REG_LR_PACONFIG, wByte);
	}
	else
	{
		return THT_OK;
	}
}

/**
 * @brief Get packet RSSI
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @return int16_t
 */
int16_t RFM95_getRSSI(RFM95_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return 0;
	}

	uint8_t rByte = 0;
	THT_StatusTypeDef res;
	res = RFM95_readRegister(me, REG_LR_PKTRSSIVALUE, &rByte);

	enum
	{
		RF_ULF_BAND = 525, /* Page 79 RFM95W-V2.0 */
		RF_UHF_BAND = 860,
		RF_RSSI_UHL_OFFSET = 157,
		RF_RSSI_UFL_OFFSET = 164,
	};

	if (THT_OK == res)
	{
		if (me->parent.freq >= RF_UHF_BAND)
		{
			return rByte - RF_RSSI_UHL_OFFSET;
		}
		else if (me->parent.freq <= RF_ULF_BAND)
		{
			return rByte - RF_RSSI_UFL_OFFSET;
		}
	}

	return 0;
}

/**
 * @brief Get packet SNR
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @return int16_t
 */
int16_t RFM95_getSNR(RFM95_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return 0;
	}

	uint8_t rByte = 0;
	THT_StatusTypeDef res;
	res = RFM95_readRegister(me, REG_LR_PKTSNRVALUE, &rByte);

	if (THT_OK == res)
	{
		return rByte / 4;
	}

	return 0;
}

/**
 * @brief Mapping DIO0 pin mode
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @param mappingMode
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_mappingDIO0(RFM95_HandleTypeDef *const me, uint8_t mappingMode)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}
	uint8_t wByte = 0;

	wByte = (uint8_t)(mappingMode | RFLR_DIOMAPPING1_DIO0_MASK);

	return RFM95_writeRegister(me, REG_LR_DIOMAPPING1, wByte);
}

/**
 * @brief Initialize paramaters for module, these parameters is fixed
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_init(RFM95_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	RFM95_powerOn(me);
	HAL_Delay(100);
	RFM95_reset(me);
	RFM95_setMode(me, RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_SLEEP | RFLR_OPMODE_FREQMODE_ACCESS_HF);
	RFM95_setFrequency(me, 923);
	RFM95_setBandwidth(me, RFLR_MODEMCONFIG1_BW_125_KHZ);
	RFM95_setCRCMode(me, RFLR_MODEMCONFIG2_RXPAYLOADCRC_ON);
	RFM95_setLNA(me);
	RFM95_setSpreadingFactor(me, RFLR_MODEMCONFIG2_SF_10);
	RFM95_setRFPowerAmplifier(me, RFLR_PACONFIG_PASELECT_PABOOST, 20);

	return RFM95_setMode(me, RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_STANDBY);
}

/**
 * @brief Ennable power supply
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_powerOn(RFM95_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	HAL_GPIO_WritePin(me->powerEnPort, me->powerEnPin, GPIO_PIN_RESET);

	return THT_OK;
}

/**
 * @brief Shutdown module
 *
 * @param me [in] A pointer to RFM95_HandleTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef RFM95_shutdown(RFM95_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	HAL_GPIO_WritePin(me->powerEnPort, me->powerEnPin, GPIO_PIN_SET);

	return THT_OK;
}
