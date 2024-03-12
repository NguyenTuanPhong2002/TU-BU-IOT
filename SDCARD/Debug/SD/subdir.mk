################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SD/fatfs_sd.c \
../SD/sd.c 

OBJS += \
./SD/fatfs_sd.o \
./SD/sd.o 

C_DEPS += \
./SD/fatfs_sd.d \
./SD/sd.d 


# Each subdirectory must supply rules for building sources it contributes
SD/%.o SD/%.su SD/%.cyclo: ../SD/%.c SD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/SDCARD/SD" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/SDCARD/midlib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SD

clean-SD:
	-$(RM) ./SD/fatfs_sd.cyclo ./SD/fatfs_sd.d ./SD/fatfs_sd.o ./SD/fatfs_sd.su ./SD/sd.cyclo ./SD/sd.d ./SD/sd.o ./SD/sd.su

.PHONY: clean-SD

