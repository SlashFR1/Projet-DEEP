################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/bsp/RFID/RFID.c 

OBJS += \
./drivers/bsp/RFID/RFID.o 

C_DEPS += \
./drivers/bsp/RFID/RFID.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/bsp/RFID/%.o drivers/bsp/RFID/%.su drivers/bsp/RFID/%.cyclo: ../drivers/bsp/RFID/%.c drivers/bsp/RFID/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../core/Inc -I../drivers/stm32g4xx_hal/Inc -I../drivers/stm32g4xx_hal/Inc/Legacy -I../drivers/cmsis/Device/ST/STM32G4xx/Include -I../drivers/cmsis/Include -I../app -I../drivers/bsp -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-drivers-2f-bsp-2f-RFID

clean-drivers-2f-bsp-2f-RFID:
	-$(RM) ./drivers/bsp/RFID/RFID.cyclo ./drivers/bsp/RFID/RFID.d ./drivers/bsp/RFID/RFID.o ./drivers/bsp/RFID/RFID.su

.PHONY: clean-drivers-2f-bsp-2f-RFID

