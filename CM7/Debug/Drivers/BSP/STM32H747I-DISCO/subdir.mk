################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery.c \
D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_bus.c \
D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_lcd.c \
D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_qspi.c \
D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_sdram.c \
D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_ts.c 

OBJS += \
./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery.o \
./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_bus.o \
./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_lcd.o \
./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_qspi.o \
./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_sdram.o \
./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_ts.o 

C_DEPS += \
./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery.d \
./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_bus.d \
./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_lcd.d \
./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_qspi.d \
./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_sdram.d \
./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_ts.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery.o: D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery.c Drivers/BSP/STM32H747I-DISCO/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I"D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_bus.o: D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_bus.c Drivers/BSP/STM32H747I-DISCO/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I"D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_lcd.o: D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_lcd.c Drivers/BSP/STM32H747I-DISCO/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I"D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_qspi.o: D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_qspi.c Drivers/BSP/STM32H747I-DISCO/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I"D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_sdram.o: D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_sdram.c Drivers/BSP/STM32H747I-DISCO/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I"D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_ts.o: D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_ts.c Drivers/BSP/STM32H747I-DISCO/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../../Drivers/BSP/Components -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Utilities/lcd -I../Core/Inc -I"D:/STM32WorkSpace/STM32H747_Mastering/Drivers/BSP/STM32H747I-DISCO" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-STM32H747I-2d-DISCO

clean-Drivers-2f-BSP-2f-STM32H747I-2d-DISCO:
	-$(RM) ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery.d ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery.o ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery.su ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_bus.d ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_bus.o ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_bus.su ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_lcd.d ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_lcd.o ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_lcd.su ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_qspi.d ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_qspi.o ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_qspi.su ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_sdram.d ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_sdram.o ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_sdram.su ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_ts.d ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_ts.o ./Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_ts.su

.PHONY: clean-Drivers-2f-BSP-2f-STM32H747I-2d-DISCO

