################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/STM32WorkSpace/STM32H747_Mastering/Utilities/CPU/cpu_utils.c 

OBJS += \
./Utilities/CPU/cpu_utils.o 

C_DEPS += \
./Utilities/CPU/cpu_utils.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/CPU/cpu_utils.o: D:/STM32WorkSpace/STM32H747_Mastering/Utilities/CPU/cpu_utils.c Utilities/CPU/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I"D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Utilities-2f-CPU

clean-Utilities-2f-CPU:
	-$(RM) ./Utilities/CPU/cpu_utils.d ./Utilities/CPU/cpu_utils.o ./Utilities/CPU/cpu_utils.su

.PHONY: clean-Utilities-2f-CPU

