/*
 * application.h
 *
 *  Created on: Oct 26, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */


#include "application.h"

#include <string.h>



void APP_copyStringUntilToken(const char *src, char des[], uint16_t size, const char *token)
{
	if (src == NULL || des == NULL || size == 0)
	{
		des = NULL;
		return;
	}

	if (token == NULL)
	{
		uint16_t srcSize = strlen(src);
		if (srcSize < size)
		{
			memcpy(des, src, srcSize);
			des[srcSize] = '\0';
		}
		else
		{
			memcpy(des, src, size);
			des[size - 1] = '\0';
		}
	}
	else
	{
		char *sPtr = strstr(src, token);

		if (sPtr != NULL)
		{
			if (sPtr - src < size)
			{
				memcpy(des, src, sPtr - src);
				des[sPtr - src] = '\0';
			}
			else
			{
				memcpy(des, src, size);
				des[size - 1] = '\0';
			}
		}
		else
		{
			des = NULL;
		}
	}
}