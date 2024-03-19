/*
 * logData.h
 *
 *  Created on: Jan 25, 2024
 *      Author: Nguyen Tuan Phong
 * 		gmail: ntphong011102@gmail.com
 */

#ifndef LOGDATA_H_
#define LOGDATA_H_

#include "main.h"

extern UART_HandleTypeDef huart6;
#pragma once

#define LOGI(comment, ...) logData::getInstance()->LogInfor(comment, __VA_ARGS__)
#define LOGW(comment, ...) logData::getInstance()->LogWarning(comment, __VA_ARGS__)
#define LOGE(comment, ...) logData::getInstance()->LogError(comment, __VA_ARGS__)
#define LOG(comment, ...) logData::getInstance()->Log(comment, __VA_ARGS__)

class logData
{
private:
    /* data */
    static logData *instancePtr;

    struct logDataConfig
    {
        /* data */
        UART_HandleTypeDef *uart{&huart6};
    };

    logDataConfig logConfig;

    logData(/* args */);
    ~logData();

public:
    static logData *getInstance(void)
    {
        if (instancePtr == nullptr)
        {
            instancePtr = new logData;
        }
        return instancePtr;
    }
    
    void LogInfor(const char *format, ...);
    void LogWarning(const char *format, ...);
    void LogError(const char *format, ...);
    void Log(const char *format, ...);
};

#endif /* LOGDATA_H_ */
