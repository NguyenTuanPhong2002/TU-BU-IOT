################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lora_1/lora.c 

OBJS += \
./lora_1/lora.o 

C_DEPS += \
./lora_1/lora.d 


# Each subdirectory must supply rules for building sources it contributes
lora_1/%.o lora_1/%.su lora_1/%.cyclo: ../lora_1/%.c lora_1/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/LORA/handle" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/LORA/lora_1" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/LORA/rfm95" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lora_1

clean-lora_1:
	-$(RM) ./lora_1/lora.cyclo ./lora_1/lora.d ./lora_1/lora.o ./lora_1/lora.su

.PHONY: clean-lora_1

