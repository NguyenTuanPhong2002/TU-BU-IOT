/*
 * logData.c
 *
 *  Created on: Jan 25, 2024
 *      Author: Nguyen Tuan Phong
 * 		gmail: ntphong011102@gmail.com
 */
#include <cstring> // Include the header for strlen
#include <stdio.h>
#include <cstdarg> // Include the necessary header for va_list, va_start, va_end

#include "logData.h"

logData *logData::instancePtr = nullptr;

logData::logData(/* args */)
{
}

logData::~logData()
{
}

void logData::LogInfor(const char *format, ...)
{
    char data[500] = {};

    //sprintf(data, "[INFOR] %s  %s\r\n", format, args);

    va_list args;
    va_start(args, format);
    sprintf(data, "[INFOR] ");
    vsprintf(data + strlen(data), format, args);
    va_end(args);

    strcat(data, "\r\n");

    HAL_UART_Transmit(logData::logConfig.uart, (uint8_t *)data, strlen(data), HAL_MAX_DELAY);
}

void logData::LogError(const char *format, ...)
{
    char data[500] = {};

    va_list args;
    va_start(args, format);
    sprintf(data, "[ERROR] ");
    vsprintf(data + strlen(data), format, args);
    va_end(args);

    strcat(data, "\r\n");

    HAL_UART_Transmit(logData::logConfig.uart, (uint8_t *)data, strlen(data), HAL_MAX_DELAY);
}

void logData::LogWarning(const char *format, ...)
{
    char data[500] = {};

    // sprintf(data, "[INFOR] %s  %s\r\n", format, args);

    va_list args;
    va_start(args, format);
    sprintf(data, "[WARNING] ");
    vsprintf(data + strlen(data), format, args);
    va_end(args);

    strcat(data, "\r\n");

    HAL_UART_Transmit(logData::logConfig.uart, (uint8_t *)data, strlen(data), HAL_MAX_DELAY);
}

void logData::Log(const char *format, ...)
{
    char data[500] = {};

    // sprintf(data, "[INFOR] %s  %s\r\n", format, args);

    va_list args;
    va_start(args, format);
    sprintf(data, "[LOG] ");
    vsprintf(data + strlen(data), format, args);
    va_end(args);

    strcat(data, "\r\n");

    HAL_UART_Transmit(logData::logConfig.uart, (uint8_t *)data, strlen(data), HAL_MAX_DELAY);
}
