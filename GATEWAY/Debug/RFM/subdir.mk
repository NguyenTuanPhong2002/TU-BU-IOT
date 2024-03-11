################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RFM/rfm.c 

OBJS += \
./RFM/rfm.o 

C_DEPS += \
./RFM/rfm.d 


# Each subdirectory must supply rules for building sources it contributes
RFM/%.o RFM/%.su RFM/%.cyclo: ../RFM/%.c RFM/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xE -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/GATEWAY/handle" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/GATEWAY/Lora" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-RFM

clean-RFM:
	-$(RM) ./RFM/rfm.cyclo ./RFM/rfm.d ./RFM/rfm.o ./RFM/rfm.su

.PHONY: clean-RFM

