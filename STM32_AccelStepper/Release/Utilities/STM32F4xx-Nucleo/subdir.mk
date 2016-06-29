################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/STM32F4xx-Nucleo/stm32f4xx_nucleo.c 

OBJS += \
./Utilities/STM32F4xx-Nucleo/stm32f4xx_nucleo.o 

C_DEPS += \
./Utilities/STM32F4xx-Nucleo/stm32f4xx_nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/STM32F4xx-Nucleo/%.o: ../Utilities/STM32F4xx-Nucleo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F4 -DSTM32 -DNUCLEO_F411RE -DSTM32F411RETx -DUSE_HAL_DRIVER -DSTM32F411xE -I"C:/Users/user/Desktop/nucleo_workspace/test_gpio/inc" -I"C:/Users/user/Desktop/nucleo_workspace/test_gpio/CMSIS/core" -I"C:/Users/user/Desktop/nucleo_workspace/test_gpio/CMSIS/device" -I"C:/Users/user/Desktop/nucleo_workspace/test_gpio/HAL_Driver/Inc/Legacy" -I"C:/Users/user/Desktop/nucleo_workspace/test_gpio/HAL_Driver/Inc" -I"C:/Users/user/Desktop/nucleo_workspace/test_gpio/Utilities/STM32F4xx-Nucleo" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


