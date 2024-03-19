################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Interface/interface.cpp 

OBJS += \
./Interface/interface.o 

CPP_DEPS += \
./Interface/interface.d 


# Each subdirectory must supply rules for building sources it contributes
Interface/%.o Interface/%.su Interface/%.cyclo: ../Interface/%.cpp Interface/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/DUCATICPP/Interface" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/DUCATICPP/Interface/logData" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/DUCATICPP/Interface/pulsar" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Interface

clean-Interface:
	-$(RM) ./Interface/interface.cyclo ./Interface/interface.d ./Interface/interface.o ./Interface/interface.su

.PHONY: clean-Interface

