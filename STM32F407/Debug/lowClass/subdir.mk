################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lowClass/sim7600.cpp 

OBJS += \
./lowClass/sim7600.o 

CPP_DEPS += \
./lowClass/sim7600.d 


# Each subdirectory must supply rules for building sources it contributes
lowClass/%.o lowClass/%.su lowClass/%.cyclo: ../lowClass/%.cpp lowClass/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lowClass

clean-lowClass:
	-$(RM) ./lowClass/sim7600.cyclo ./lowClass/sim7600.d ./lowClass/sim7600.o ./lowClass/sim7600.su

.PHONY: clean-lowClass

