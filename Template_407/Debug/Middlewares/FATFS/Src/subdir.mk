################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/FATFS/Src/diskio.c \
../Middlewares/FATFS/Src/ff.c \
../Middlewares/FATFS/Src/ffsystem.c \
../Middlewares/FATFS/Src/ffunicode.c 

OBJS += \
./Middlewares/FATFS/Src/diskio.o \
./Middlewares/FATFS/Src/ff.o \
./Middlewares/FATFS/Src/ffsystem.o \
./Middlewares/FATFS/Src/ffunicode.o 

C_DEPS += \
./Middlewares/FATFS/Src/diskio.d \
./Middlewares/FATFS/Src/ff.d \
./Middlewares/FATFS/Src/ffsystem.d \
./Middlewares/FATFS/Src/ffunicode.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/FATFS/Src/%.o Middlewares/FATFS/Src/%.su: ../Middlewares/FATFS/Src/%.c Middlewares/FATFS/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/TestStand -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Commu -I../Drivers/SYSTEM/delay -I../Drivers/SYSTEM/sys -I../Drivers/SYSTEM/usart -I../Drivers/BSP/STEPPER_MOTOR -I../Drivers/BSP/TIMER -I../Drivers/BSP/KEY -I../Drivers/BSP/LED -I../Drivers/BSP/RS232 -I../Drivers/BSP/PID -I../Drivers/BSP/ADC -I../Drivers/BSP/RTC -I../Drivers/BSP/PHOTOE_SWITCH -I../Drivers/BSP/LOG -I../Core/usr/include -I../Core/usr/TestStand -I../Middlewares/USB_APP/Inc -I../Middlewares/USB_CLASS/Inc -I../Middlewares/USB_CORE/Inc -I../Middlewares/MALLOC -I../Middlewares/FATFS/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-FATFS-2f-Src

clean-Middlewares-2f-FATFS-2f-Src:
	-$(RM) ./Middlewares/FATFS/Src/diskio.d ./Middlewares/FATFS/Src/diskio.o ./Middlewares/FATFS/Src/diskio.su ./Middlewares/FATFS/Src/ff.d ./Middlewares/FATFS/Src/ff.o ./Middlewares/FATFS/Src/ff.su ./Middlewares/FATFS/Src/ffsystem.d ./Middlewares/FATFS/Src/ffsystem.o ./Middlewares/FATFS/Src/ffsystem.su ./Middlewares/FATFS/Src/ffunicode.d ./Middlewares/FATFS/Src/ffunicode.o ./Middlewares/FATFS/Src/ffunicode.su

.PHONY: clean-Middlewares-2f-FATFS-2f-Src

