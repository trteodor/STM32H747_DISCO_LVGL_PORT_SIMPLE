################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/Components/is42s32800j/is42s32800j.c 

OBJS += \
./Drivers/BSP/Components/is42s32800j/is42s32800j.o 

C_DEPS += \
./Drivers/BSP/Components/is42s32800j/is42s32800j.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/is42s32800j/is42s32800j.o: D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/Components/is42s32800j/is42s32800j.c Drivers/BSP/Components/is42s32800j/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I"D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-is42s32800j

clean-Drivers-2f-BSP-2f-Components-2f-is42s32800j:
	-$(RM) ./Drivers/BSP/Components/is42s32800j/is42s32800j.d ./Drivers/BSP/Components/is42s32800j/is42s32800j.o ./Drivers/BSP/Components/is42s32800j/is42s32800j.su

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-is42s32800j

