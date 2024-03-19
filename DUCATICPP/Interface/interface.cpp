/*
 * interface.cpp
 *
 *  Created on: Jan 25, 2024
 *      Author: Nguyen Tuan Phong
 * 		gmail: ntphong011102@gmail.com
 */

#include "interface.h"

interface *interface::instancePtr = nullptr;

interface::interface(/* args */)
{
}

interface::~interface()
{
}

void interface::initMain(void){
	LOGI("MCU %s","start");
}
