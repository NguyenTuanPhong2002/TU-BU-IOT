################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../midlib/http.c \
../midlib/sim.c \
../midlib/wlv_def.c 

OBJS += \
./midlib/http.o \
./midlib/sim.o \
./midlib/wlv_def.o 

C_DEPS += \
./midlib/http.d \
./midlib/sim.d \
./midlib/wlv_def.d 


# Each subdirectory must supply rules for building sources it contributes
midlib/%.o midlib/%.su midlib/%.cyclo: ../midlib/%.c midlib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/TUBU_V4/APPLICATION/DUCATI" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/TUBU_V4/APPLICATION/lora" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/TUBU_V4/APPLICATION/lora/rfm95" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/TUBU_V4/APPLICATION/sim7600" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/TUBU_V4/APPLICATION/SD_CARD" -I"C:/Users/skttr/OneDrive/Desktop/TU-BU-IOT/TUBU_V4/midlib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-midlib

clean-midlib:
	-$(RM) ./midlib/http.cyclo ./midlib/http.d ./midlib/http.o ./midlib/http.su ./midlib/sim.cyclo ./midlib/sim.d ./midlib/sim.o ./midlib/sim.su ./midlib/wlv_def.cyclo ./midlib/wlv_def.d ./midlib/wlv_def.o ./midlib/wlv_def.su

.PHONY: clean-midlib

