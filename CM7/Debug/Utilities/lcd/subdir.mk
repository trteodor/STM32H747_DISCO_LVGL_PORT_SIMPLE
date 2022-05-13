################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/STM32WorkSpace/STM32H747_Mastering/Utilities/lcd/stm32_lcd.c 

OBJS += \
./Utilities/lcd/stm32_lcd.o 

C_DEPS += \
./Utilities/lcd/stm32_lcd.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/lcd/stm32_lcd.o: D:/STM32WorkSpace/STM32H747_Mastering/Utilities/lcd/stm32_lcd.c Utilities/lcd/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I../../Drivers/BSP/STM32H747I-DISCO -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Utilities-2f-lcd

clean-Utilities-2f-lcd:
	-$(RM) ./Utilities/lcd/stm32_lcd.d ./Utilities/lcd/stm32_lcd.o ./Utilities/lcd/stm32_lcd.su

.PHONY: clean-Utilities-2f-lcd

