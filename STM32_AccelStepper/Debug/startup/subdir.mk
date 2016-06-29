################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f411xe.s 

OBJS += \
./startup/startup_stm32f411xe.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo %cd%
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"C:/Users/user/Desktop/nucleo_workspace/test_gpio/inc" -I"C:/Users/user/Desktop/nucleo_workspace/test_gpio/CMSIS/core" -I"C:/Users/user/Desktop/nucleo_workspace/test_gpio/CMSIS/device" -I"C:/Users/user/Desktop/nucleo_workspace/test_gpio/HAL_Driver/Inc/Legacy" -I"C:/Users/user/Desktop/nucleo_workspace/test_gpio/HAL_Driver/Inc" -I"C:/Users/user/Desktop/nucleo_workspace/test_gpio/Utilities/STM32F4xx-Nucleo" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


