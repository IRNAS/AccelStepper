################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/AccelStepper.c \
../src/main.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/AccelStepper.o \
./src/main.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/AccelStepper.d \
./src/main.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F4 -DSTM32 -DNUCLEO_F411RE -DSTM32F411RETx -DUSE_HAL_DRIVER -DSTM32F411xE -I"C:/Users/user/Desktop/nucleo_workspace/STM32_AccelStepper/inc" -I"C:/Users/user/Desktop/nucleo_workspace/STM32_AccelStepper/CMSIS/core" -I"C:/Users/user/Desktop/nucleo_workspace/STM32_AccelStepper/CMSIS/device" -I"C:/Users/user/Desktop/nucleo_workspace/STM32_AccelStepper/HAL_Driver/Inc/Legacy" -I"C:/Users/user/Desktop/nucleo_workspace/STM32_AccelStepper/HAL_Driver/Inc" -I"C:/Users/user/Desktop/nucleo_workspace/STM32_AccelStepper/Utilities/STM32F4xx-Nucleo" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


