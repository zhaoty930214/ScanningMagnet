################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/FATFS/source/diskio.c \
../Middlewares/FATFS/source/ff.c \
../Middlewares/FATFS/source/ffsystem.c \
../Middlewares/FATFS/source/ffunicode.c 

OBJS += \
./Middlewares/FATFS/source/diskio.o \
./Middlewares/FATFS/source/ff.o \
./Middlewares/FATFS/source/ffsystem.o \
./Middlewares/FATFS/source/ffunicode.o 

C_DEPS += \
./Middlewares/FATFS/source/diskio.d \
./Middlewares/FATFS/source/ff.d \
./Middlewares/FATFS/source/ffsystem.d \
./Middlewares/FATFS/source/ffunicode.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/FATFS/source/%.o Middlewares/FATFS/source/%.su: ../Middlewares/FATFS/source/%.c Middlewares/FATFS/source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/TestStand -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Commu -I../Drivers/SYSTEM/delay -I../Drivers/SYSTEM/sys -I../Drivers/SYSTEM/usart -I../Drivers/BSP/STEPPER_MOTOR -I../Drivers/BSP/TIMER -I../Drivers/BSP/KEY -I../Drivers/BSP/LED -I../Drivers/BSP/RS232 -I../Drivers/BSP/PID -I../Drivers/BSP/ADC -I../Drivers/BSP/PHOTOE_SWITCH -I../Core/usr/include -I../Core/usr/TestStand -I../Middlewares/USB_APP/Inc -I../Middlewares/USB_CLASS/Inc -I../Middlewares/USB_CORE/Inc -I../Middlewares/MALLOC -I../Middlewares/FATFS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-FATFS-2f-source

clean-Middlewares-2f-FATFS-2f-source:
	-$(RM) ./Middlewares/FATFS/source/diskio.d ./Middlewares/FATFS/source/diskio.o ./Middlewares/FATFS/source/diskio.su ./Middlewares/FATFS/source/ff.d ./Middlewares/FATFS/source/ff.o ./Middlewares/FATFS/source/ff.su ./Middlewares/FATFS/source/ffsystem.d ./Middlewares/FATFS/source/ffsystem.o ./Middlewares/FATFS/source/ffsystem.su ./Middlewares/FATFS/source/ffunicode.d ./Middlewares/FATFS/source/ffunicode.o ./Middlewares/FATFS/source/ffunicode.su

.PHONY: clean-Middlewares-2f-FATFS-2f-source

