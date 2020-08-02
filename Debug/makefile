################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32L4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Core/Libs/step_motor/subdir.mk
-include Core/Libs/ssd1306/subdir.mk
-include Core/Libs/hcsr04_IC/subdir.mk
-include Core/Libs/encoder/subdir.mk
-include Core/Libs/echo_radar/subdir.mk
-include Core/Libs/dwt_delay/subdir.mk
-include Core/Libs/dma_uasrt/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
STM32L452RE_RADAR.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
STM32L452RE_RADAR.list \

OBJCOPY_BIN += \
STM32L452RE_RADAR.bin \


# All Target
all: STM32L452RE_RADAR.elf secondary-outputs

# Tool invocations
STM32L452RE_RADAR.elf: $(OBJS) $(USER_OBJS) C:\Users\ar725\Desktop\MC\work_space_A\STM32L452RE_RADAR\STM32L452RETX_FLASH.ld
	arm-none-eabi-gcc -o "STM32L452RE_RADAR.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\ar725\Desktop\MC\work_space_A\STM32L452RE_RADAR\STM32L452RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="STM32L452RE_RADAR.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

STM32L452RE_RADAR.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "STM32L452RE_RADAR.list"
	@echo 'Finished building: $@'
	@echo ' '

STM32L452RE_RADAR.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "STM32L452RE_RADAR.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
