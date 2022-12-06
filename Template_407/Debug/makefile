################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/USB_CORE/Src/subdir.mk
-include Middlewares/USB_CLASS/Src/subdir.mk
-include Middlewares/USB_APP/Src/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Middlewares/FATFS/exfuns/subdir.mk
-include Middlewares/FATFS/Src/subdir.mk
-include Middlewares/Commu/subdir.mk
-include Drivers/SYSTEM/usart/subdir.mk
-include Drivers/SYSTEM/sys/subdir.mk
-include Drivers/SYSTEM/delay/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/Legacy/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/TIMER/subdir.mk
-include Drivers/BSP/STEPPER_MOTOR/subdir.mk
-include Drivers/BSP/RTC/subdir.mk
-include Drivers/BSP/RS232/subdir.mk
-include Drivers/BSP/PID/subdir.mk
-include Drivers/BSP/PHOTOE_SWITCH/subdir.mk
-include Drivers/BSP/LOG/subdir.mk
-include Drivers/BSP/LED/subdir.mk
-include Drivers/BSP/KEY/subdir.mk
-include Drivers/BSP/ADC/subdir.mk
-include Core/usr/src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Template_407
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Template_407.elf \

MAP_FILES += \
Template_407.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Template_407.list \

OBJCOPY_HEX += \
Template_407.hex \


# All Target
all: main-build

# Main-build Target
main-build: Template_407.elf secondary-outputs

# Tool invocations
Template_407.elf Template_407.map: $(OBJS) $(USER_OBJS) C:\Users\tianyu.zhao\STM32CubeIDE\workspace_1.10.1\Template_407\STM32F407IGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Template_407.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\tianyu.zhao\STM32CubeIDE\workspace_1.10.1\Template_407\STM32F407IGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Template_407.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Template_407.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Template_407.list"
	@echo 'Finished building: $@'
	@echo ' '

Template_407.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "Template_407.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Template_407.elf Template_407.hex Template_407.list Template_407.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
