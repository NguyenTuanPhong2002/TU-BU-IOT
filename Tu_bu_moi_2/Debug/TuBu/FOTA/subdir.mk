################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TuBu/FOTA/fota.c 

OBJS += \
./TuBu/FOTA/fota.o 

C_DEPS += \
./TuBu/FOTA/fota.d 


# Each subdirectory must supply rules for building sources it contributes
TuBu/FOTA/%.o TuBu/FOTA/%.su: ../TuBu/FOTA/%.c TuBu/FOTA/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/HP/STM32CubeIDE/Tu_Bu/Tu_bu_moi_2/TuBu/FLASH" -I"C:/Users/HP/STM32CubeIDE/Tu_Bu/Tu_bu_moi_2/TuBu/FOTA" -I"C:/Users/HP/STM32CubeIDE/Tu_Bu/Tu_bu_moi_2/TuBu/RS485" -I"C:/Users/HP/STM32CubeIDE/Tu_Bu/Tu_bu_moi_2/TuBu/SIM7x00" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TuBu-2f-FOTA

clean-TuBu-2f-FOTA:
	-$(RM) ./TuBu/FOTA/fota.d ./TuBu/FOTA/fota.o ./TuBu/FOTA/fota.su

.PHONY: clean-TuBu-2f-FOTA

