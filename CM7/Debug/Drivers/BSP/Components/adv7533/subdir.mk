################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/Components/adv7533/adv7533.c \
D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/Components/adv7533/adv7533_reg.c 

OBJS += \
./Drivers/BSP/Components/adv7533/adv7533.o \
./Drivers/BSP/Components/adv7533/adv7533_reg.o 

C_DEPS += \
./Drivers/BSP/Components/adv7533/adv7533.d \
./Drivers/BSP/Components/adv7533/adv7533_reg.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/adv7533/adv7533.o: D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/Components/adv7533/adv7533.c Drivers/BSP/Components/adv7533/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I../../Drivers/BSP/STM32H747I-DISCO -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/Components/adv7533/adv7533_reg.o: D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/Components/adv7533/adv7533_reg.c Drivers/BSP/Components/adv7533/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I../../Drivers/BSP/STM32H747I-DISCO -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-adv7533

clean-Drivers-2f-BSP-2f-Components-2f-adv7533:
	-$(RM) ./Drivers/BSP/Components/adv7533/adv7533.d ./Drivers/BSP/Components/adv7533/adv7533.o ./Drivers/BSP/Components/adv7533/adv7533.su ./Drivers/BSP/Components/adv7533/adv7533_reg.d ./Drivers/BSP/Components/adv7533/adv7533_reg.o ./Drivers/BSP/Components/adv7533/adv7533_reg.su

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-adv7533

