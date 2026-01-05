################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/bsp/CODE/code.c 

OBJS += \
./drivers/bsp/CODE/code.o 

C_DEPS += \
./drivers/bsp/CODE/code.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/bsp/CODE/%.o drivers/bsp/CODE/%.su drivers/bsp/CODE/%.cyclo: ../drivers/bsp/CODE/%.c drivers/bsp/CODE/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../core/Inc -I../drivers/stm32g4xx_hal/Inc -I../drivers/stm32g4xx_hal/Inc/Legacy -I../drivers/cmsis/Device/ST/STM32G4xx/Include -I../drivers/cmsis/Include -I../app -I../drivers/bsp -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-drivers-2f-bsp-2f-CODE

clean-drivers-2f-bsp-2f-CODE:
	-$(RM) ./drivers/bsp/CODE/code.cyclo ./drivers/bsp/CODE/code.d ./drivers/bsp/CODE/code.o ./drivers/bsp/CODE/code.su

.PHONY: clean-drivers-2f-bsp-2f-CODE

