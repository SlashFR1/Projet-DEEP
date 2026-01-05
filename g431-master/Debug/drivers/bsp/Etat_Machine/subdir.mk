################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/bsp/Etat_Machine/Etat_Machine.c 

OBJS += \
./drivers/bsp/Etat_Machine/Etat_Machine.o 

C_DEPS += \
./drivers/bsp/Etat_Machine/Etat_Machine.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/bsp/Etat_Machine/%.o drivers/bsp/Etat_Machine/%.su drivers/bsp/Etat_Machine/%.cyclo: ../drivers/bsp/Etat_Machine/%.c drivers/bsp/Etat_Machine/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../core/Inc -I../drivers/stm32g4xx_hal/Inc -I../drivers/stm32g4xx_hal/Inc/Legacy -I../drivers/cmsis/Device/ST/STM32G4xx/Include -I../drivers/cmsis/Include -I../app -I../drivers/bsp -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-drivers-2f-bsp-2f-Etat_Machine

clean-drivers-2f-bsp-2f-Etat_Machine:
	-$(RM) ./drivers/bsp/Etat_Machine/Etat_Machine.cyclo ./drivers/bsp/Etat_Machine/Etat_Machine.d ./drivers/bsp/Etat_Machine/Etat_Machine.o ./drivers/bsp/Etat_Machine/Etat_Machine.su

.PHONY: clean-drivers-2f-bsp-2f-Etat_Machine

