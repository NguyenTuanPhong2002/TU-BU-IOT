################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lora/lora.c 

OBJS += \
./lora/lora.o 

C_DEPS += \
./lora/lora.d 


# Each subdirectory must supply rules for building sources it contributes
lora/%.o lora/%.su lora/%.cyclo: ../lora/%.c lora/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/GATEW/lora" -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/GATEW/midlib" -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/GATEW/sim7600" -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/GATEW/lora/rfm95" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lora

clean-lora:
	-$(RM) ./lora/lora.cyclo ./lora/lora.d ./lora/lora.o ./lora/lora.su

.PHONY: clean-lora

