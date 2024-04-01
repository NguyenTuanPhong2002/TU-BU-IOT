################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APPLICATION/lora/lora.c 

OBJS += \
./APPLICATION/lora/lora.o 

C_DEPS += \
./APPLICATION/lora/lora.d 


# Each subdirectory must supply rules for building sources it contributes
APPLICATION/lora/%.o APPLICATION/lora/%.su APPLICATION/lora/%.cyclo: ../APPLICATION/lora/%.c APPLICATION/lora/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/TUBU_V4/APPLICATION/DUCATI" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/TUBU_V4/APPLICATION/lora" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/TUBU_V4/APPLICATION/lora/rfm95" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/TUBU_V4/APPLICATION/sim7600" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/TUBU_V4/APPLICATION/SD_CARD" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/TUBU_V4/midlib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-APPLICATION-2f-lora

clean-APPLICATION-2f-lora:
	-$(RM) ./APPLICATION/lora/lora.cyclo ./APPLICATION/lora/lora.d ./APPLICATION/lora/lora.o ./APPLICATION/lora/lora.su

.PHONY: clean-APPLICATION-2f-lora

