################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sim7600/sim7600.c 

OBJS += \
./sim7600/sim7600.o 

C_DEPS += \
./sim7600/sim7600.d 


# Each subdirectory must supply rules for building sources it contributes
sim7600/%.o sim7600/%.su sim7600/%.cyclo: ../sim7600/%.c sim7600/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/GATEW/lora" -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/GATEW/midlib" -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/GATEW/sim7600" -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/GATEW/lora/rfm95" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-sim7600

clean-sim7600:
	-$(RM) ./sim7600/sim7600.cyclo ./sim7600/sim7600.d ./sim7600/sim7600.o ./sim7600/sim7600.su

.PHONY: clean-sim7600

