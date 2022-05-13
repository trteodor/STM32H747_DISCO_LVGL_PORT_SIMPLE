################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/STM32WorkSpace/STM32H747_Mastering/Utilities/Fonts/font12.c \
D:/STM32WorkSpace/STM32H747_Mastering/Utilities/Fonts/font16.c \
D:/STM32WorkSpace/STM32H747_Mastering/Utilities/Fonts/font20.c \
D:/STM32WorkSpace/STM32H747_Mastering/Utilities/Fonts/font24.c \
D:/STM32WorkSpace/STM32H747_Mastering/Utilities/Fonts/font8.c 

OBJS += \
./Utilities/Fonts/font12.o \
./Utilities/Fonts/font16.o \
./Utilities/Fonts/font20.o \
./Utilities/Fonts/font24.o \
./Utilities/Fonts/font8.o 

C_DEPS += \
./Utilities/Fonts/font12.d \
./Utilities/Fonts/font16.d \
./Utilities/Fonts/font20.d \
./Utilities/Fonts/font24.d \
./Utilities/Fonts/font8.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/Fonts/font12.o: D:/STM32WorkSpace/STM32H747_Mastering/Utilities/Fonts/font12.c Utilities/Fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I"D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Utilities/Fonts/font16.o: D:/STM32WorkSpace/STM32H747_Mastering/Utilities/Fonts/font16.c Utilities/Fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I"D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Utilities/Fonts/font20.o: D:/STM32WorkSpace/STM32H747_Mastering/Utilities/Fonts/font20.c Utilities/Fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I"D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Utilities/Fonts/font24.o: D:/STM32WorkSpace/STM32H747_Mastering/Utilities/Fonts/font24.c Utilities/Fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I"D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Utilities/Fonts/font8.o: D:/STM32WorkSpace/STM32H747_Mastering/Utilities/Fonts/font8.c Utilities/Fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I"D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Utilities-2f-Fonts

clean-Utilities-2f-Fonts:
	-$(RM) ./Utilities/Fonts/font12.d ./Utilities/Fonts/font12.o ./Utilities/Fonts/font12.su ./Utilities/Fonts/font16.d ./Utilities/Fonts/font16.o ./Utilities/Fonts/font16.su ./Utilities/Fonts/font20.d ./Utilities/Fonts/font20.o ./Utilities/Fonts/font20.su ./Utilities/Fonts/font24.d ./Utilities/Fonts/font24.o ./Utilities/Fonts/font24.su ./Utilities/Fonts/font8.d ./Utilities/Fonts/font8.o ./Utilities/Fonts/font8.su

.PHONY: clean-Utilities-2f-Fonts

