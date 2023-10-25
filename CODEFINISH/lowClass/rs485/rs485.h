/*
 * rs485.h
 *
 *  Created on: Oct 7, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#ifndef RS485_RS485_H_
#define RS485_RS485_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

typedef enum
{
    RS485_OK,
    RS485_ERROR,
    RS485_TIMEOUT,
    RS485_BUSY,
} RS485_StatusTypeDef;

typedef enum
{
    Read_CoilStatus = 0x01,
    Read_InputStatus = 0x02,
    Read_HoldingRegister = 0x03,
    Read_InputRegister = 0x04,
    Force_SingleCoil = 0x05,
    Preset_SingleRegister = 0x06,
    Read_ExceptionStatus = 0x07,
    Fetch_CommEventCounter = 0x0B,
    Fetch_CommEventLog = 0x0C,
    Force_MultipleCoils = 0x0F,
    Preset_MultipleRegisters = 0x10,
    Report_SlaveID = 0x11,
    Read_GeneralReference = 0x14,
    Write_GeneralReference = 0x15,
    MaskWrite_4XRegister = 0x16,
    ReadWrite_4XRegisters = 0x17,
    Read_FIFOQueue = 0x24

} RS485_Function;

typedef enum
{
    RECEIVE = 0u,
    TRANSMIT
} RS485_PinState;

// Query struct
typedef struct
{
    uint8_t slaveAddress;
    RS485_Function mbFunction;
    uint16_t regAddress;
    uint16_t regAddress1;
    uint16_t regAddress2;
    uint16_t regCount;
    uint16_t crc;
} RS485Query_t;

// Hold Register struct
typedef struct
{
    uint8_t length;
    uint8_t slaveAddress;
    RS485_Function Function;
    uint16_t dataReg[3];
    uint8_t CoilStatus[20];
} RS485Data_t;

class rs485
{
private:
    /* data */
    GPIO_TypeDef *RS485_Power_Port;
    uint16_t RS485_Power_Pin;
    GPIO_TypeDef *RS485_DE_Port;
    uint16_t RS485_DE_Pin;
    GPIO_TypeDef *RS485_RE_Port;
    uint16_t RS485_RE_Pin;
    UART_HandleTypeDef *huart;
    char TxBuffer[256];
    char RxBuffer[256];

public:
    void RS485_enablePowerOn();
    void RS485_enablePowerOff();
    void RS485_enableTransmitMode();
    void RS485_enableReceiveMode();

    RS485_StatusTypeDef RS485_Transmit();
    RS485_StatusTypeDef RS485_Receive();

    uint16_t crcCalculation(uint8_t *rs485_data, int in_dx);
    void CONTACTOR_transmit(UART_HandleTypeDef *huart, RS485Query_t p_RS485Query);
    void RS485_CON();
    RS485Data_t RS485_analysis(RS485Query_t RS485_SentStruct, uint8_t *p_RS485Data, uint8_t length);
    void RS485_ForceSingleCoil(UART_HandleTypeDef *huart, uint8_t SlaveAddr, uint16_t Coil, uint8_t State);
    void RS485_Master_Receive(char Master_ReceivedBuff[], int length);

    char *GetTxBuffer();
    char *GetRxBuffer();
};

#endif /* RS485_RS485_H_ */
