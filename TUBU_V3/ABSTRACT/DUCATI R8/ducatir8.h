/*
 * ducatir8.h
 *
 *  Created on: Oct 27, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#ifndef DUCATI_R8_DUCATIR8_H_
#define DUCATI_R8_DUCATIR8_H_

#include "ducati.h"

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

typedef struct
{
    uint8_t slaveAddress;
    RS485_Function mbFunction;
    uint16_t regAddress;
    uint16_t regAddress1;
    uint16_t regAddress2;
    uint16_t regCount;
    uint32_t crc;
} RS485Query_t;

typedef struct
{
    uint8_t length;
    uint8_t slaveAddress;
    RS485_Function Function;
    uint16_t dataReg[3];
    uint8_t CoilStatus[20];
} RS485Data_t;

typedef struct
{
    GPIO_TypeDef *RE_PORT;
    uint8_t RE_PIN;
    GPIO_TypeDef *DE_PORT;
    uint8_t DE_PIN;
    GPIO_TypeDef *PWR_PORT;
    uint8_t PWR_PIN;
    UART_HandleTypeDef *huart;
    DMA_HandleTypeDef *hdma;
} DUCATI_R8ptr;

class DUCATI_R8 : public DUCATI
{
private:
    DUCATI_R8ptr *ducatiR8;

    RS485Query_t msg_Query;

    void OnPower();
    void OffPower();
    void EnableTransmit();
    void EnableReceiver();
    void Disable();

    void DUCATI_transmit(RS485Query_t p_RS485Query);
    void DUCATI_forceSingleCoil(uint8_t SlaveAddr, uint16_t Coil, uint8_t State);
    void DUCATI_Master_Receive(char Master_ReceivedBuff[], int length);

    RS485Data_t DUCATI_analysis(RS485Query_t RS485_SentStruct, uint8_t *p_RS485Data, uint8_t length);

public:
    DUCATI_R8(DUCATI_R8ptr *ducati);
    ~DUCATI_R8();

    float DUCATI_getCosF();
    float DUCATI_getVoltage();
    float DUCATI_getCurrent();
    float DUCATI_getFrequency();

    uint16_t DUCATI_getAvQ();
    uint16_t DUCATI_getActivePower();
    uint16_t DUCATI_getReactivePower();
    uint16_t DUCATI_getAvP();
};

#endif /* DUCATI_R8_DUCATIR8_H_ */
