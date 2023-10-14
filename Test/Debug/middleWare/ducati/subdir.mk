################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../middleWare/ducati/ducati.c 

C_DEPS += \
./middleWare/ducati/ducati.d 

OBJS += \
./middleWare/ducati/ducati.o 


# Each subdirectory must supply rules for building sources it contributes
middleWare/ducati/%.o middleWare/ducati/%.su middleWare/ducati/%.cyclo: ../middleWare/ducati/%.c middleWare/ducati/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/skttr/OneDrive/Trump/Learning/Tu bu IOT/TU-BU-IOT/Test/middleWare/ducati" -I"C:/Users/skttr/OneDrive/Trump/Learning/Tu bu IOT/TU-BU-IOT/Test/lowClass/rs485" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-middleWare-2f-ducati

clean-middleWare-2f-ducati:
	-$(RM) ./middleWare/ducati/ducati.cyclo ./middleWare/ducati/ducati.d ./middleWare/ducati/ducati.o ./middleWare/ducati/ducati.su

.PHONY: clean-middleWare-2f-ducati

