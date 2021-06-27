################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/yggdrasil/kernel/Event.cpp \
../src/yggdrasil/kernel/Mutex.cpp \
../src/yggdrasil/kernel/Scheduler.cpp \
../src/yggdrasil/kernel/Task.cpp 

OBJS += \
./src/yggdrasil/kernel/Event.o \
./src/yggdrasil/kernel/Mutex.o \
./src/yggdrasil/kernel/Scheduler.o \
./src/yggdrasil/kernel/Task.o 

CPP_DEPS += \
./src/yggdrasil/kernel/Event.d \
./src/yggdrasil/kernel/Mutex.d \
./src/yggdrasil/kernel/Scheduler.d \
./src/yggdrasil/kernel/Task.d 


# Each subdirectory must supply rules for building sources it contributes
src/yggdrasil/kernel/Event.o: ../src/yggdrasil/kernel/Event.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -DSTM32WL55xx -c -I"C:/Users/shang/STM32CubeIDE/workspace_1.4.0/test_yggdrasil/CM4/src" -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -std=gnu++17 -fstack-usage -MMD -MP -MF"src/yggdrasil/kernel/Event.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/yggdrasil/kernel/Mutex.o: ../src/yggdrasil/kernel/Mutex.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -DSTM32WL55xx -c -I"C:/Users/shang/STM32CubeIDE/workspace_1.4.0/test_yggdrasil/CM4/src" -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -std=gnu++17 -fstack-usage -MMD -MP -MF"src/yggdrasil/kernel/Mutex.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/yggdrasil/kernel/Scheduler.o: ../src/yggdrasil/kernel/Scheduler.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -DSTM32WL55xx -c -I"C:/Users/shang/STM32CubeIDE/workspace_1.4.0/test_yggdrasil/CM4/src" -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -std=gnu++17 -fstack-usage -MMD -MP -MF"src/yggdrasil/kernel/Scheduler.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/yggdrasil/kernel/Task.o: ../src/yggdrasil/kernel/Task.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -DSTM32WL55xx -c -I"C:/Users/shang/STM32CubeIDE/workspace_1.4.0/test_yggdrasil/CM4/src" -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -std=gnu++17 -fstack-usage -MMD -MP -MF"src/yggdrasil/kernel/Task.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

