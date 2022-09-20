################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/usr/src/Unused.c \
../Core/usr/src/my_tasks.c \
../Core/usr/src/tsk_functions.c 

OBJS += \
./Core/usr/src/Unused.o \
./Core/usr/src/my_tasks.o \
./Core/usr/src/tsk_functions.o 

C_DEPS += \
./Core/usr/src/Unused.d \
./Core/usr/src/my_tasks.d \
./Core/usr/src/tsk_functions.d 


# Each subdirectory must supply rules for building sources it contributes
Core/usr/src/%.o Core/usr/src/%.su: ../Core/usr/src/%.c Core/usr/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/TestStand -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Commu -I../Drivers/SYSTEM/delay -I../Drivers/SYSTEM/sys -I../Drivers/SYSTEM/usart -I../Drivers/BSP/STEPPER_MOTOR -I../Drivers/BSP/TIMER -I../Drivers/BSP/KEY -I../Drivers/BSP/LED -I../Drivers/BSP/RS232 -I../Drivers/BSP/PID -I../Drivers/BSP/ADC -I../Core/usr/include -I../Core/usr/TestStand -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-usr-2f-src

clean-Core-2f-usr-2f-src:
	-$(RM) ./Core/usr/src/Unused.d ./Core/usr/src/Unused.o ./Core/usr/src/Unused.su ./Core/usr/src/my_tasks.d ./Core/usr/src/my_tasks.o ./Core/usr/src/my_tasks.su ./Core/usr/src/tsk_functions.d ./Core/usr/src/tsk_functions.o ./Core/usr/src/tsk_functions.su

.PHONY: clean-Core-2f-usr-2f-src

