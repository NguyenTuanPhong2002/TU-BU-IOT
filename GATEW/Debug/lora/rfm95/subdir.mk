################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lora/rfm95/rfm95.c 

OBJS += \
./lora/rfm95/rfm95.o 

C_DEPS += \
./lora/rfm95/rfm95.d 


# Each subdirectory must supply rules for building sources it contributes
lora/rfm95/%.o lora/rfm95/%.su lora/rfm95/%.cyclo: ../lora/rfm95/%.c lora/rfm95/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/GATEW/lora" -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/GATEW/midlib" -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/GATEW/sim7600" -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/GATEW/lora/rfm95" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lora-2f-rfm95

clean-lora-2f-rfm95:
	-$(RM) ./lora/rfm95/rfm95.cyclo ./lora/rfm95/rfm95.d ./lora/rfm95/rfm95.o ./lora/rfm95/rfm95.su

.PHONY: clean-lora-2f-rfm95

