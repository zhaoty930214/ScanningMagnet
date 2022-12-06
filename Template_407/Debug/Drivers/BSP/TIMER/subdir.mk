################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/TIMER/atim.c \
../Drivers/BSP/TIMER/btim.c \
../Drivers/BSP/TIMER/gtim.c \
../Drivers/BSP/TIMER/stepper_tim.c 

OBJS += \
./Drivers/BSP/TIMER/atim.o \
./Drivers/BSP/TIMER/btim.o \
./Drivers/BSP/TIMER/gtim.o \
./Drivers/BSP/TIMER/stepper_tim.o 

C_DEPS += \
./Drivers/BSP/TIMER/atim.d \
./Drivers/BSP/TIMER/btim.d \
./Drivers/BSP/TIMER/gtim.d \
./Drivers/BSP/TIMER/stepper_tim.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/TIMER/%.o Drivers/BSP/TIMER/%.su: ../Drivers/BSP/TIMER/%.c Drivers/BSP/TIMER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/TestStand -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Commu -I../Drivers/SYSTEM/delay -I../Drivers/SYSTEM/sys -I../Drivers/SYSTEM/usart -I../Drivers/BSP/STEPPER_MOTOR -I../Drivers/BSP/TIMER -I../Drivers/BSP/KEY -I../Drivers/BSP/LED -I../Drivers/BSP/RS232 -I../Drivers/BSP/PID -I../Drivers/BSP/ADC -I../Drivers/BSP/PHOTOE_SWITCH -I../Drivers/BSP/RTC -I../Drivers/BSP/LOG -I../Core/usr/include -I../Core/usr/TestStand -I../Middlewares/USB_APP/Inc -I../Middlewares/USB_CLASS/Inc -I../Middlewares/USB_CORE/Inc -I../Middlewares/FATFS/Inc -I../Middlewares/FATFS/exfuns -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-TIMER

clean-Drivers-2f-BSP-2f-TIMER:
	-$(RM) ./Drivers/BSP/TIMER/atim.d ./Drivers/BSP/TIMER/atim.o ./Drivers/BSP/TIMER/atim.su ./Drivers/BSP/TIMER/btim.d ./Drivers/BSP/TIMER/btim.o ./Drivers/BSP/TIMER/btim.su ./Drivers/BSP/TIMER/gtim.d ./Drivers/BSP/TIMER/gtim.o ./Drivers/BSP/TIMER/gtim.su ./Drivers/BSP/TIMER/stepper_tim.d ./Drivers/BSP/TIMER/stepper_tim.o ./Drivers/BSP/TIMER/stepper_tim.su

.PHONY: clean-Drivers-2f-BSP-2f-TIMER

