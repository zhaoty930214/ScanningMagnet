################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/FATFS/documents/res/app1.c \
../Middlewares/FATFS/documents/res/app2.c \
../Middlewares/FATFS/documents/res/app3.c \
../Middlewares/FATFS/documents/res/app4.c \
../Middlewares/FATFS/documents/res/app5.c \
../Middlewares/FATFS/documents/res/app6.c 

OBJS += \
./Middlewares/FATFS/documents/res/app1.o \
./Middlewares/FATFS/documents/res/app2.o \
./Middlewares/FATFS/documents/res/app3.o \
./Middlewares/FATFS/documents/res/app4.o \
./Middlewares/FATFS/documents/res/app5.o \
./Middlewares/FATFS/documents/res/app6.o 

C_DEPS += \
./Middlewares/FATFS/documents/res/app1.d \
./Middlewares/FATFS/documents/res/app2.d \
./Middlewares/FATFS/documents/res/app3.d \
./Middlewares/FATFS/documents/res/app4.d \
./Middlewares/FATFS/documents/res/app5.d \
./Middlewares/FATFS/documents/res/app6.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/FATFS/documents/res/%.o Middlewares/FATFS/documents/res/%.su: ../Middlewares/FATFS/documents/res/%.c Middlewares/FATFS/documents/res/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/TestStand -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Commu -I../Drivers/SYSTEM/delay -I../Drivers/SYSTEM/sys -I../Drivers/SYSTEM/usart -I../Drivers/BSP/STEPPER_MOTOR -I../Drivers/BSP/TIMER -I../Drivers/BSP/KEY -I../Drivers/BSP/LED -I../Drivers/BSP/RS232 -I../Drivers/BSP/PID -I../Drivers/BSP/ADC -I../Drivers/BSP/PHOTOE_SWITCH -I../Core/usr/include -I../Core/usr/TestStand -I../Middlewares/USB_APP/Inc -I../Middlewares/USB_CLASS/Inc -I../Middlewares/USB_CORE/Inc -I../Middlewares/MALLOC -I../Middlewares/FATFS/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-FATFS-2f-documents-2f-res

clean-Middlewares-2f-FATFS-2f-documents-2f-res:
	-$(RM) ./Middlewares/FATFS/documents/res/app1.d ./Middlewares/FATFS/documents/res/app1.o ./Middlewares/FATFS/documents/res/app1.su ./Middlewares/FATFS/documents/res/app2.d ./Middlewares/FATFS/documents/res/app2.o ./Middlewares/FATFS/documents/res/app2.su ./Middlewares/FATFS/documents/res/app3.d ./Middlewares/FATFS/documents/res/app3.o ./Middlewares/FATFS/documents/res/app3.su ./Middlewares/FATFS/documents/res/app4.d ./Middlewares/FATFS/documents/res/app4.o ./Middlewares/FATFS/documents/res/app4.su ./Middlewares/FATFS/documents/res/app5.d ./Middlewares/FATFS/documents/res/app5.o ./Middlewares/FATFS/documents/res/app5.su ./Middlewares/FATFS/documents/res/app6.d ./Middlewares/FATFS/documents/res/app6.o ./Middlewares/FATFS/documents/res/app6.su

.PHONY: clean-Middlewares-2f-FATFS-2f-documents-2f-res

