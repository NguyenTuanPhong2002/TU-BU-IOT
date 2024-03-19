/*
 * interface.h
 *
 *  Created on: Jan 25, 2024
 *      Author: Nguyen Tuan Phong
 * 		gmail: ntphong011102@gmail.com
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "main.h"

#include "logData/logData.h"
#include "pulsar/pulsar.h"
#include "Singleton.h"

class interface
{
private:
    static interface *instancePtr;

    interface(/* args */);
    ~interface();

public:
    static interface *getInstance()
    {
        if (instancePtr == nullptr)
        {
            instancePtr = new interface();
        }
        return instancePtr;
    }

    void initMain(void);
};

#endif /* INTERFACE_H_ */
