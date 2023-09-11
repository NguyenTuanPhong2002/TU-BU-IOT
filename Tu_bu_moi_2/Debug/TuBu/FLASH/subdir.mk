################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TuBu/FLASH/flash.c 

OBJS += \
./TuBu/FLASH/flash.o 

C_DEPS += \
./TuBu/FLASH/flash.d 


# Each subdirectory must supply rules for building sources it contributes
TuBu/FLASH/%.o TuBu/FLASH/%.su: ../TuBu/FLASH/%.c TuBu/FLASH/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/HP/STM32CubeIDE/Tu_Bu/Tu_bu_moi_2/TuBu/FLASH" -I"C:/Users/HP/STM32CubeIDE/Tu_Bu/Tu_bu_moi_2/TuBu/FOTA" -I"C:/Users/HP/STM32CubeIDE/Tu_Bu/Tu_bu_moi_2/TuBu/RS485" -I"C:/Users/HP/STM32CubeIDE/Tu_Bu/Tu_bu_moi_2/TuBu/SIM7x00" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TuBu-2f-FLASH

clean-TuBu-2f-FLASH:
	-$(RM) ./TuBu/FLASH/flash.d ./TuBu/FLASH/flash.o ./TuBu/FLASH/flash.su

.PHONY: clean-TuBu-2f-FLASH

