################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/bsp/GACHE/gache.c 

OBJS += \
./drivers/bsp/GACHE/gache.o 

C_DEPS += \
./drivers/bsp/GACHE/gache.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/bsp/GACHE/%.o drivers/bsp/GACHE/%.su drivers/bsp/GACHE/%.cyclo: ../drivers/bsp/GACHE/%.c drivers/bsp/GACHE/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../core/Inc -I../drivers/stm32g4xx_hal/Inc -I../drivers/stm32g4xx_hal/Inc/Legacy -I../drivers/cmsis/Device/ST/STM32G4xx/Include -I../drivers/cmsis/Include -I../app -I../drivers/bsp -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-drivers-2f-bsp-2f-GACHE

clean-drivers-2f-bsp-2f-GACHE:
	-$(RM) ./drivers/bsp/GACHE/gache.cyclo ./drivers/bsp/GACHE/gache.d ./drivers/bsp/GACHE/gache.o ./drivers/bsp/GACHE/gache.su

.PHONY: clean-drivers-2f-bsp-2f-GACHE

