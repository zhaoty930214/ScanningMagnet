################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/FATFS/exfuns/exfuns.c \
../Middlewares/FATFS/exfuns/fattester.c 

OBJS += \
./Middlewares/FATFS/exfuns/exfuns.o \
./Middlewares/FATFS/exfuns/fattester.o 

C_DEPS += \
./Middlewares/FATFS/exfuns/exfuns.d \
./Middlewares/FATFS/exfuns/fattester.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/FATFS/exfuns/%.o Middlewares/FATFS/exfuns/%.su: ../Middlewares/FATFS/exfuns/%.c Middlewares/FATFS/exfuns/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/TestStand -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Commu -I../Drivers/SYSTEM/delay -I../Drivers/SYSTEM/sys -I../Drivers/SYSTEM/usart -I../Drivers/BSP/STEPPER_MOTOR -I../Drivers/BSP/TIMER -I../Drivers/BSP/KEY -I../Drivers/BSP/LED -I../Drivers/BSP/RS232 -I../Drivers/BSP/PID -I../Drivers/BSP/ADC -I../Drivers/BSP/RTC -I../Drivers/BSP/PHOTOE_SWITCH -I../Drivers/BSP/LOG -I../Core/usr/include -I../Core/usr/TestStand -I../Middlewares/USB_APP/Inc -I../Middlewares/USB_CLASS/Inc -I../Middlewares/USB_CORE/Inc -I../Middlewares/MALLOC -I../Middlewares/FATFS/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-FATFS-2f-exfuns

clean-Middlewares-2f-FATFS-2f-exfuns:
	-$(RM) ./Middlewares/FATFS/exfuns/exfuns.d ./Middlewares/FATFS/exfuns/exfuns.o ./Middlewares/FATFS/exfuns/exfuns.su ./Middlewares/FATFS/exfuns/fattester.d ./Middlewares/FATFS/exfuns/fattester.o ./Middlewares/FATFS/exfuns/fattester.su

.PHONY: clean-Middlewares-2f-FATFS-2f-exfuns

