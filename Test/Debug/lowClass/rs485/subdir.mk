################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lowClass/rs485/rs485.c 

C_DEPS += \
./lowClass/rs485/rs485.d 

OBJS += \
./lowClass/rs485/rs485.o 


# Each subdirectory must supply rules for building sources it contributes
lowClass/rs485/%.o lowClass/rs485/%.su lowClass/rs485/%.cyclo: ../lowClass/rs485/%.c lowClass/rs485/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/skttr/OneDrive/Trump/Learning/Tu bu IOT/TU-BU-IOT/Test/lowClass/rs485" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lowClass-2f-rs485

clean-lowClass-2f-rs485:
	-$(RM) ./lowClass/rs485/rs485.cyclo ./lowClass/rs485/rs485.d ./lowClass/rs485/rs485.o ./lowClass/rs485/rs485.su

.PHONY: clean-lowClass-2f-rs485

