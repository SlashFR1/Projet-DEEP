################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include drivers/stm32g4xx_hal/Src/subdir.mk
-include drivers/bsp/tft_ili9341/subdir.mk
-include drivers/bsp/epaper/Fonts/subdir.mk
-include drivers/bsp/epaper/subdir.mk
-include drivers/bsp/YX6300/subdir.mk
-include drivers/bsp/WS2812/subdir.mk
-include drivers/bsp/VL53L0X/subdir.mk
-include drivers/bsp/SD/FatFs/src/option/subdir.mk
-include drivers/bsp/SD/FatFs/src/drivers/subdir.mk
-include drivers/bsp/SD/FatFs/src/subdir.mk
-include drivers/bsp/SD/subdir.mk
-include drivers/bsp/RFID/subdir.mk
-include drivers/bsp/NFC03A1/lib_nfc/lib_pcd/subdir.mk
-include drivers/bsp/NFC03A1/lib_nfc/lib_NDEF/subdir.mk
-include drivers/bsp/NFC03A1/lib_nfc/common/subdir.mk
-include drivers/bsp/NFC03A1/CR95HF/subdir.mk
-include drivers/bsp/NFC03A1/subdir.mk
-include drivers/bsp/MotorDC/subdir.mk
-include drivers/bsp/MatrixKeyboard/subdir.mk
-include drivers/bsp/MPU6050/subdir.mk
-include drivers/bsp/MCP23S17/subdir.mk
-include drivers/bsp/MCP23017/subdir.mk
-include drivers/bsp/LD19/subdir.mk
-include drivers/bsp/HC-SR04/subdir.mk
-include drivers/bsp/HC-05/subdir.mk
-include drivers/bsp/GPS/subdir.mk
-include drivers/bsp/GACHE/subdir.mk
-include drivers/bsp/Fingerprint/subdir.mk
-include drivers/bsp/Etat_Machine/subdir.mk
-include drivers/bsp/DS18B20/subdir.mk
-include drivers/bsp/DHT11/subdir.mk
-include drivers/bsp/CODE/subdir.mk
-include drivers/bsp/BMP180/subdir.mk
-include drivers/bsp/BH1750FVI/subdir.mk
-include drivers/bsp/AS608/subdir.mk
-include drivers/bsp/APDS9960/subdir.mk
-include drivers/bsp/subdir.mk
-include core/Startup/subdir.mk
-include core/Src/subdir.mk
-include app/subdir.mk
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


BUILD_ARTIFACT_NAME := G431_base
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
G431_base.elf \

MAP_FILES += \
G431_base.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
G431_base.list \

OBJCOPY_BIN += \
G431_base.bin \


# All Target
all: main-build

# Main-build Target
main-build: G431_base.elf secondary-outputs

# Tool invocations
G431_base.elf G431_base.map: $(OBJS) $(USER_OBJS) /Users/baffourlouis/Documents/deep/g431-master/core/STM32G431KBTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "G431_base.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/Users/baffourlouis/Documents/deep/g431-master/core/STM32G431KBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="G431_base.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

G431_base.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "G431_base.list"
	@echo 'Finished building: $@'
	@echo ' '

G431_base.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "G431_base.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) G431_base.bin G431_base.elf G431_base.list G431_base.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
