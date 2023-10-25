/*
 * fota.cpp
 *
 *  Created on: Oct 5, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#include "fota.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern CRC_HandleTypeDef hcrc;

/* RAMFUNCTION */
#include "stm32f4xx_hal.h"

extern FLASH_ProcessTypeDef pFlash;

/**
 * @brief
 *
 * @return __RAM_FUNC
 */
/**
 * The function `FLASHRAM_SetErrorCode` checks for various flash error flags and sets the corresponding
 * error codes in the `pFlash` structure.
 *
 * @return a value of type HAL_StatusTypeDef, which is a status enumeration indicating the success or
 * failure of the operation. In this case, it is returning HAL_OK, indicating that the operation was
 * successful.
 */
static __RAM_FUNC HAL_StatusTypeDef FLASHRAM_SetErrorCode(void)
{
	uint32_t flags = 0U;

	if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_WRPERR))
	{
		pFlash.ErrorCode |= HAL_FLASH_ERROR_WRP;
		flags |= FLASH_FLAG_WRPERR;
	}
	if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_PGAERR))
	{
		pFlash.ErrorCode |= HAL_FLASH_ERROR_PGA;
		flags |= FLASH_FLAG_PGAERR;
	}
//	if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERR))
//	{
//		pFlash.ErrorCode |= HAL_FLASH_ERROR_OPTV;
//		flags |= FLASH_FLAG_OPTVERR;
//	}

#if defined(FLASH_SR_RDERR)
	if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_RDERR))
	{
		pFlash.ErrorCode |= HAL_FLASH_ERROR_RD;
		flags |= FLASH_FLAG_RDERR;
	}
#endif /* FLASH_SR_RDERR */
#if defined(FLASH_SR_OPTVERRUSR)
	if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERRUSR))
	{
		pFlash.ErrorCode |= HAL_FLASH_ERROR_OPTVUSR;
		flags |= FLASH_FLAG_OPTVERRUSR;
	}
#endif /* FLASH_SR_OPTVERRUSR */

	/* Clear FLASH error pending bits */
	__HAL_FLASH_CLEAR_FLAG(flags);

	return HAL_OK;
}

/**
 * @brief
 *
 * @param Timeout
 * @return __RAM_FUNC
 */
/**
 * The function waits for a FLASH operation to complete by polling on the BUSY flag and returns the
 * status of the operation.
 *
 * @param Timeout The Timeout parameter is the maximum amount of time (in milliseconds) to wait for the
 * FLASH operation to complete. If the operation takes longer than the specified timeout, the function
 * will return HAL_TIMEOUT.
 *
 * @return a value of type HAL_StatusTypeDef.
 */
static __RAM_FUNC HAL_StatusTypeDef FLASHRAM_WaitForLastOperation(uint32_t Timeout)
{
	/* Wait for the FLASH operation to complete by polling on BUSY flag to be reset.
	   Even if the FLASH operation fails, the BUSY flag will be reset and an error
	   flag will be set */

	while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) && (Timeout != 0x00U))
	{
		Timeout--;
	}

	if (Timeout == 0x00U)
	{
		return HAL_TIMEOUT;
	}

	/* Check FLASH End of Operation flag  */
	if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_EOP))
	{
		/* Clear FLASH End of Operation pending bit */
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
	}

//	if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_WRPERR) ||
//		__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERR) ||
//#if defined(FLASH_SR_RDERR)
//		__HAL_FLASH_GET_FLAG(FLASH_FLAG_RDERR) ||
//#endif /* FLASH_SR_RDERR */
//#if defined(FLASH_SR_OPTVERRUSR)
//		__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERRUSR) ||
//#endif /* FLASH_SR_OPTVERRUSR */
//		__HAL_FLASH_GET_FLAG(FLASH_FLAG_PGAERR))
//	{
//		/*Save the error code*/
//		FLASHRAM_SetErrorCode();
//		return HAL_ERROR;
//	}

	/* There is no error flag set */
	return HAL_OK;
}

/**
 * @brief
 *
 * @param Address
 * @param pBuffer
 * @return __RAM_FUNC
 */
/**
 * The function `HAL_FLASHEx_DoublePageProgram` is used to program a double page in flash memory with
 * 32 different words.
 *
 * @param Address The starting address in flash memory where the data will be written.
 * @param pBuffer A pointer to the buffer containing the data to be programmed into flash memory.
 *
 * @return a value of type HAL_StatusTypeDef, which is a status enumeration indicating the result of
 * the double page program operation.
 */
__RAM_FUNC HAL_StatusTypeDef HAL_FLASHEx_DoublePageProgram(uint32_t Address,
														   uint32_t *pBuffer)
{
	uint32_t primask_bit;
	uint32_t count = 0U;
	HAL_StatusTypeDef status = HAL_OK;

	/* Wait for last operation to be completed */
	//status = FLASHRAM_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

	if (status == HAL_OK)
	{
		/* Disable all IRQs */
		primask_bit = __get_PRIMASK();
		HAL_Delay(50);
		__disable_irq();

		/* Proceed to program the new half page */
//		SET_BIT(FLASH->PECR, FLASH_PECR_FPRG);
//		SET_BIT(FLASH->PECR, FLASH_PECR_PROG);

		/* Write one half page directly with 32 different words */
		while (count < 128U)
		{
			*(__IO uint32_t *)((uint32_t)(Address + (4 * count))) = *pBuffer;
			pBuffer++;
			count++;
		}

		/* Wait for last operation to be completed */
		//status = FLASHRAM_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

		/* If the write operation is completed, disable the PROG and FPRG bits */
//		CLEAR_BIT(FLASH->PECR, FLASH_PECR_PROG);
//		CLEAR_BIT(FLASH->PECR, FLASH_PECR_FPRG);

		/* Enable IRQs */
		__set_PRIMASK(primask_bit);
	}

	/* Return the Write Status */
	return status;
}

//----------------------------------------------------------

/**
 * The function FOTA_SET_FLAG sets a flag by performing a bitwise OR operation with the input setFlag.
 *
 * @param setFlag The setFlag parameter is a uint8_t (unsigned 8-bit integer) that represents the flag
 * value to be set.
 *
 * @return the result of the bitwise OR operation between the current value of the `Flag` variable and
 * the `setFlag` parameter.
 */
uint8_t FOTA::FOTA_SET_FLAG(uint8_t setFlag)
{
    return this->Flag |= setFlag;
}

/**
 * The function FOTA_GET_FLAG checks if a specific flag is set in the Flag variable.
 *
 * @param setFlag The parameter `setFlag` is a uint8_t variable that represents a flag value.
 *
 * @return a boolean value.
 */
uint8_t FOTA::FOTA_GET_FLAG(uint8_t setFlag)
{
    return (this->Flag & setFlag) == setFlag;
}

/**
 * The function FOTA_CLR_FLAG clears a specific flag in the Flag variable.
 *
 * @param setFlag The parameter "setFlag" is a uint8_t variable that represents the flag that needs to
 * be cleared.
 *
 * @return the result of the bitwise AND operation between the current value of the `Flag` variable and
 * the complement of the `setFlag` variable.
 */
uint8_t FOTA::FOTA_CLR_FLAG(uint8_t setFlag)
{
    return this->Flag &= ~setFlag;
}

/**
 * The FOTA_setDownloadURL function sets the download URL for firmware updates, ensuring that the URL
 * size is within the limits.
 *
 * @param pURL A pointer to the download URL string.
 * @param URLSize URLSize is the size of the URL string passed as the pURL parameter.
 *
 * @return a value of type FOTA_StatusTypeDef.
 */
FOTA_StatusTypeDef FOTA::FOTA_setDownloadURL(const char *pURL, uint16_t URLSize)
{
    if (URLSize < sizeof(this->firmwareURL))
    {
        memset(this->firmwareURL, '\0', sizeof(this->firmwareURL));
        memcpy(this->firmwareURL, pURL, URLSize);
        this->firmwareURL[URLSize] = '\0';
        return FOTA_OK;
    }
    else
    {
        return FOTA_ERROR;
    }
}

/**
 * The function FOTA_getCurrentVersion retrieves the current version of the firmware from EEPROM and
 * stores it in an array.
 *
 * @param pVersion pVersion is a pointer to an array of uint16_t values. This array will be used to
 * store the current version information.
 * @param size The parameter "size" is the size of the array "pVersion". It indicates the number of
 * elements that can be stored in the array. In this case, it is expected to be 3.
 *
 * @return a value of type FOTA_StatusTypeDef.
 */
FOTA_StatusTypeDef FOTA::FOTA_getCurrentVersion(uint16_t pVersion[], size_t size)
{
    if (size != 3)
    {
        return FOTA_ERROR;
    }

    pVersion[0] = *(volatile uint16_t *)FOTA_EEPROM_CURRENT_MAJOR;
    pVersion[1] = *(volatile uint16_t *)FOTA_EEPROM_CURRENT_MINOR;
    pVersion[2] = *(volatile uint16_t *)FOTA_EEPROM_CURRENT_PATCH;
    return FOTA_OK;
}

/**
 * The function compares two versions and returns a status indicating if the new version is compatible
 * or not.
 *
 * @return a value of type FOTA_StatusTypeDef.
 */
FOTA_StatusTypeDef FOTA::FOTA_compareVersion()
{

    FOTA_getCurrentVersion(this->curVersion, 3);
    if (newVersion[0] > curVersion[0])
    {
        if ((curVersion[2] == 0 && newVersion[2] == 1) || (curVersion[2] == 1 && newVersion[2] == 0))
        {
            return FOTA_OK;
        }
        else
        {
            return FOTA_ERROR;
        }
    }
    else if (newVersion[0] < curVersion[0])
    {
        return FOTA_ERROR;
    }
    else
    {
        if (newVersion[1] > curVersion[1])
        {
            if ((curVersion[2] == 0 && newVersion[2] == 1) || (curVersion[2] == 1 && newVersion[2] == 0))
            {
                return FOTA_OK;
            }
            else
            {
                return FOTA_ERROR;
            }
        }
        else
        {
            return FOTA_ERROR;
        }
    }
}

/**
 * The function `FOTA_parseFirmwareInfo` parses the firmware filename to extract the firmware name and
 * version.
 *
 * @return a value of type FOTA_StatusTypeDef.
 */
FOTA_StatusTypeDef FOTA::FOTA_parseFirmwareInfo()
{
    /* Parse firmware filename */
    char *hPtr = NULL;
    char *tPtr = NULL;

    hPtr = strstr(this->firmwareURL, FOTA_FIRMWARE_HEADER_NAME);
    tPtr = strstr(hPtr, ".bin");

    if ((hPtr == NULL) || (tPtr == NULL))
    {
        // printf("Parse firmawre error!\n");
        return FOTA_ERROR;
    }
    else
    {
        memset(this->firmwareName, '\0', sizeof(this->firmwareName));
        uint8_t firmwareNameLength = tPtr + sizeof("bin") - hPtr;
        strncpy(this->firmwareName, hPtr, firmwareNameLength);
        // printf("Firmware name: %s\n", me->firmwareName);
    }

    /*Parse firmware version */
    /* vfasscontroller_1.4.0.bin */
    char *sPtr = NULL;

    /* Parse major */
    sPtr = strstr(this->firmwareName, "_");

    if (sPtr != NULL)
    {
        this->newVersion[0] = atoi(sPtr + 1);

        /* Parse minor */
        sPtr = strstr(sPtr + 1, ".");

        if (sPtr != NULL)
        {
            this->newVersion[1] = atoi(sPtr + 1);

            /* Parse patch */
            sPtr = strstr(sPtr + 1, ".");

            if (sPtr != NULL)
            {
                this->newVersion[2] = atoi(sPtr + 1);
            }
        }
    }
    // printf("New firmware version: %u.%u.%u\n", me->newVersion[0],
    //       me->newVersion[1], me->newVersion[2]);

    return FOTA_OK;
}

/**
 * The FOTA_downloadFirmware function downloads a firmware file using an HTTP request and returns a
 * status indicating success or failure.
 *
 * @return a value of type FOTA_StatusTypeDef.
 */
FOTA_StatusTypeDef FOTA::FOTA_downloadFirmware()
{
    if (httpDownloadFile(this->firmwareURL, this->firmwareName) != SIM_OK)
    {
        return FOTA_ERROR;
    }
    else
    {
        return FOTA_OK;
    }
}

/**
 * The function `FOTA_writeFirmware` writes firmware to a specific address in flash memory and verifies
 * the download by calculating the CRC32 checksum.
 *
 * @return a value of type FOTA_StatusTypeDef.
 */
FOTA_StatusTypeDef FOTA::FOTA_writeFirmware()
{

    this->curAddress = FOTA_getCurrentAddress();

    /* Set new firmware address */
    if (this->curAddress == FOTA_APP_1_START_ADDR)
    {
        this->newAddress = FOTA_APP_2_START_ADDR;
    }
    else
    {
        this->newAddress = FOTA_APP_1_START_ADDR;
    }

    char command[150] = {0};

    uint8_t counter = 0;
    do
    {
        HAL_FLASH_Unlock();

        FLASH_EraseInitTypeDef pEraseInit = {FLASH_TYPEERASE_SECTORS, this->newAddress, FOTA_NUMBER_FLASH_PAGES};

        uint32_t PageError;

        HAL_FLASHEx_Erase(&pEraseInit, &PageError);
        this->firmwareSize = SIM_getFileSize(this->firmwareName);
        if (this->firmwareSize >= (FOTA_APP_1_END_ADDR - FOTA_APP_1_START_ADDR))
        {
            FOTA_SET_FLAG(FOTA_FLAG_FFER);

            // printf(" File : %s\n", me->firmwareName);
            // printf(" OverSize of memory : %ld Bytes\n", me->firmwareSize);
            break;
        }

        uint32_t countdownSize = this->firmwareSize;
        uint32_t wBytes = 0U;
        uint32_t pageCnt = 0U;

        // printf("Memory Programming ...\n");
        // printf(" File : %s\n", me->firmwareName);
        // printf(" Size : %ld Bytes\n", me->firmwareSize);
        // printf(" Address : %lX\n\n", me->newAddress);

        while (countdownSize > 0U)
        {
            if (countdownSize >= 512U)
            {
                sprintf(command, "AT+CFTRANTX=\"F:/%s\",%lu,%u",
                        this->firmwareName, wBytes, 512U);
                wBytes += 512U;
                countdownSize -= 512U;
            }
            else
            {
                sprintf(command, "AT+CFTRANTX=\"F:/%s\",%lu,%lu",
                        this->firmwareName, wBytes, countdownSize);
                wBytes += countdownSize;
                countdownSize = 0;
            }
            // printf("Loading: %.2f%%\n", 100 * (double)wBytes / me->firmwareSize);
            if (sendATcommand(command, "+CFTRANTX:", 1000) == SIM_OK)
            {
                HAL_FLASHEx_DoublePageProgram(this->newAddress + (pageCnt++ * 512U), (uint32_t *)(SIM_getBuffer() + 23));
            }
            else
            {
                break;
            }
        }

        HAL_FLASH_Lock();

        this->CRC32 = HAL_CRC_Calculate(&hcrc, (uint32_t *)this->newAddress, this->firmwareSize / 4);

        if (this->CRC32 == 0UL)
        {
            //            printf("Check CRC right!\n"
            //                   "Download verified successfully\n\n\n");
            FOTA_SET_FLAG( FOTA_FLAG_UPOK);
        }
        else
        {
            //            printf("Check CRC wrong!\n"
            //                   "Download verified unsuccessfully\nTry again\n\n\n");
            FOTA_SET_FLAG( FOTA_FLAG_CRCER);
        }

        counter++;
    } while ((counter < 3) && (FOTA_GET_FLAG( FOTA_FLAG_UPOK) == 0));

    memset(command, '\0', sizeof(command));

    sprintf(command, "AT+FSDEL=\"/%s\"", this->firmwareName);
    sendATcommand(command, SIM_URC_OK, 3000);

    if (FOTA_GET_FLAG( FOTA_FLAG_UPOK))
    {
        this->curAddress = this->newAddress;
        this->curVersion[0] = this->newVersion[0];
        this->curVersion[1] = this->newVersion[1];
        this->curVersion[2] = this->newVersion[2];
        return FOTA_OK;
    }
    else
    {
        return FOTA_ERROR;
    }
}

/**
 * The function FOTA_getCurrentAddress returns the value stored at the address specified by
 * FOTA_EEPROM_CURRENT_ADDR.
 *
 * @return the value stored at the memory address specified by the macro FOTA_EEPROM_CURRENT_ADDR,
 * casted as a volatile uint32_t pointer.
 */
uint32_t FOTA_getCurrentAddress(void)
{
    return *(volatile uint32_t *)FOTA_EEPROM_CURRENT_ADDR;
}
