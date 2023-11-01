/*
 * ducati.h
 *
 *  Created on: Oct 27, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#ifndef DUCATI_DUCATI_H_
#define DUCATI_DUCATI_H_

#include "application.h"

#define MAX_BUFFER_DUCATI 500

typedef struct
{
    char DUCATI_CosF[9];
    char DUCATI_I[9];
    char DUCATI_V[9];
    char DUCATI_P[9];
    char DUCATI_Q[9];
    char DUCATI_avrP[9];
    char DUCATI_avrQ[9];
    char DUCATI_PE[9];
    char DUCATI_QE[9];
    char DUCATI_coil[7];
    char DUCATI_CON1[9];
    char DUCATI_BT[9];
    char DUCATI_F[9];
    char DUCATI_Temp[9];
} DUCATI_VALUE;

class DUCATI
{
private:
protected:
    uint8_t buffer[MAX_BUFFER_DUCATI];

public:
    DUCATI();
    ~DUCATI();

    virtual float DUCATI_getCosF();
    virtual float DUCATI_getVoltage();
    virtual float DUCATI_getCurrent();
    virtual float DUCATI_getFrequency();

    virtual uint16_t DUCATI_getAvQ();
    virtual uint16_t DUCATI_getActivePower();
    virtual uint16_t DUCATI_getReactivePower();
    virtual uint16_t DUCATI_getAvP();
};

#endif /* DUCATI_DUCATI_H_ */
