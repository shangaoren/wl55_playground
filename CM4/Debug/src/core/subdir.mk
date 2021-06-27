################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/Core.cpp 

OBJS += \
./src/core/Core.o 

CPP_DEPS += \
./src/core/Core.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/Core.o: ../src/core/Core.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -DSTM32WL55xx -c -I"C:/Users/shang/STM32CubeIDE/workspace_1.4.0/test_yggdrasil/CM4/src" -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -std=gnu++17 -fstack-usage -MMD -MP -MF"src/core/Core.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

