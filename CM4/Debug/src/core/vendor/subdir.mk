################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/core/vendor/startup_stm32wl55jcix.s 

S_DEPS += \
./src/core/vendor/startup_stm32wl55jcix.d 

OBJS += \
./src/core/vendor/startup_stm32wl55jcix.o 


# Each subdirectory must supply rules for building sources it contributes
src/core/vendor/startup_stm32wl55jcix.o: ../src/core/vendor/startup_stm32wl55jcix.s
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -I"C:/Users/shang/STM32CubeIDE/workspace_1.4.0/test_yggdrasil/CM4/src" -x assembler-with-cpp -MMD -MP -MF"src/core/vendor/startup_stm32wl55jcix.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

