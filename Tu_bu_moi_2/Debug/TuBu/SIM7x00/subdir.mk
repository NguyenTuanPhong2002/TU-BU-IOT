################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TuBu/SIM7x00/sim7x00.c 

OBJS += \
./TuBu/SIM7x00/sim7x00.o 

C_DEPS += \
./TuBu/SIM7x00/sim7x00.d 


# Each subdirectory must supply rules for building sources it contributes
TuBu/SIM7x00/%.o TuBu/SIM7x00/%.su TuBu/SIM7x00/%.cyclo: ../TuBu/SIM7x00/%.c TuBu/SIM7x00/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"G:/TU-BU-IOT/Tu_bu_moi_2/TuBu/FLASH" -I"G:/TU-BU-IOT/Tu_bu_moi_2/TuBu/FOTA" -I"G:/TU-BU-IOT/Tu_bu_moi_2/TuBu/RS485" -I"G:/TU-BU-IOT/Tu_bu_moi_2/TuBu/SIM7x00" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TuBu-2f-SIM7x00

clean-TuBu-2f-SIM7x00:
	-$(RM) ./TuBu/SIM7x00/sim7x00.cyclo ./TuBu/SIM7x00/sim7x00.d ./TuBu/SIM7x00/sim7x00.o ./TuBu/SIM7x00/sim7x00.su

.PHONY: clean-TuBu-2f-SIM7x00

