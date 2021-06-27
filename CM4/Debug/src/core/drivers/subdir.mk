################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/drivers/Clocks.cpp \
../src/core/drivers/Systick.cpp 

OBJS += \
./src/core/drivers/Clocks.o \
./src/core/drivers/Systick.o 

CPP_DEPS += \
./src/core/drivers/Clocks.d \
./src/core/drivers/Systick.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/drivers/Clocks.o: ../src/core/drivers/Clocks.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -DSTM32WL55xx -c -I"C:/Users/shang/STM32CubeIDE/workspace_1.4.0/test_yggdrasil/CM4/src" -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -std=gnu++17 -fstack-usage -MMD -MP -MF"src/core/drivers/Clocks.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/core/drivers/Systick.o: ../src/core/drivers/Systick.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -DSTM32WL55xx -c -I"C:/Users/shang/STM32CubeIDE/workspace_1.4.0/test_yggdrasil/CM4/src" -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -std=gnu++17 -fstack-usage -MMD -MP -MF"src/core/drivers/Systick.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

