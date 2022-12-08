################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Inc/CRSLib/src/debug.cpp 

OBJS += \
./Core/Inc/CRSLib/src/debug.o 

CPP_DEPS += \
./Core/Inc/CRSLib/src/debug.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/CRSLib/src/%.o Core/Inc/CRSLib/src/%.su: ../Core/Inc/CRSLib/src/%.cpp Core/Inc/CRSLib/src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++20 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103x6 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-CRSLib-2f-src

clean-Core-2f-Inc-2f-CRSLib-2f-src:
	-$(RM) ./Core/Inc/CRSLib/src/debug.d ./Core/Inc/CRSLib/src/debug.o ./Core/Inc/CRSLib/src/debug.su

.PHONY: clean-Core-2f-Inc-2f-CRSLib-2f-src

