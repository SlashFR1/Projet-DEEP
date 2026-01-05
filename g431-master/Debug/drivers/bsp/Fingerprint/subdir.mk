################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/bsp/Fingerprint/fingerprint.c 

OBJS += \
./drivers/bsp/Fingerprint/fingerprint.o 

C_DEPS += \
./drivers/bsp/Fingerprint/fingerprint.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/bsp/Fingerprint/%.o drivers/bsp/Fingerprint/%.su drivers/bsp/Fingerprint/%.cyclo: ../drivers/bsp/Fingerprint/%.c drivers/bsp/Fingerprint/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../core/Inc -I../drivers/stm32g4xx_hal/Inc -I../drivers/stm32g4xx_hal/Inc/Legacy -I../drivers/cmsis/Device/ST/STM32G4xx/Include -I../drivers/cmsis/Include -I../app -I../drivers/bsp -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-drivers-2f-bsp-2f-Fingerprint

clean-drivers-2f-bsp-2f-Fingerprint:
	-$(RM) ./drivers/bsp/Fingerprint/fingerprint.cyclo ./drivers/bsp/Fingerprint/fingerprint.d ./drivers/bsp/Fingerprint/fingerprint.o ./drivers/bsp/Fingerprint/fingerprint.su

.PHONY: clean-drivers-2f-bsp-2f-Fingerprint

