################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APPLICATION/DUCATI/ducati.c 

OBJS += \
./APPLICATION/DUCATI/ducati.o 

C_DEPS += \
./APPLICATION/DUCATI/ducati.d 


# Each subdirectory must supply rules for building sources it contributes
APPLICATION/DUCATI/%.o APPLICATION/DUCATI/%.su APPLICATION/DUCATI/%.cyclo: ../APPLICATION/DUCATI/%.c APPLICATION/DUCATI/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/TUBU_V4/APPLICATION/DUCATI" -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/TUBU_V4/APPLICATION/lora" -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/TUBU_V4/APPLICATION/lora/rfm95" -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/TUBU_V4/APPLICATION/sim7600" -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/TUBU_V4/APPLICATION/SD_CARD" -I"C:/Users/skttr/OneDrive/Desktop/Trump/DATN/TU-BU-IOT/TUBU_V4/midlib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-APPLICATION-2f-DUCATI

clean-APPLICATION-2f-DUCATI:
	-$(RM) ./APPLICATION/DUCATI/ducati.cyclo ./APPLICATION/DUCATI/ducati.d ./APPLICATION/DUCATI/ducati.o ./APPLICATION/DUCATI/ducati.su

.PHONY: clean-APPLICATION-2f-DUCATI

