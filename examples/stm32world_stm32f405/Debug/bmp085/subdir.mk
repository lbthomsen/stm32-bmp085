################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bmp085/bmp085.c 

OBJS += \
./bmp085/bmp085.o 

C_DEPS += \
./bmp085/bmp085.d 


# Each subdirectory must supply rules for building sources it contributes
bmp085/%.o bmp085/%.su bmp085/%.cyclo: ../bmp085/%.c bmp085/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/lth/src/stm32-bmp085/examples/stm32world_stm32f405/bmp085" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-bmp085

clean-bmp085:
	-$(RM) ./bmp085/bmp085.cyclo ./bmp085/bmp085.d ./bmp085/bmp085.o ./bmp085/bmp085.su

.PHONY: clean-bmp085

