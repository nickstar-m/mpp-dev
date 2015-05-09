################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../modbus/tcp/mbtcp.c 

OBJS += \
./modbus/tcp/mbtcp.o 

C_DEPS += \
./modbus/tcp/mbtcp.d 


# Each subdirectory must supply rules for building sources it contributes
modbus/tcp/%.o: ../modbus/tcp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DSTM32F030 -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DSTM32F030 -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f0-stdperiph" -I"/media/D/MyProg/Robota/CM12/modbus/include" -I"/media/D/MyProg/Robota/CM12/modbus/ascii" -I"/media/D/MyProg/Robota/CM12/modbus/rtu" -I"/media/D/MyProg/Robota/CM12/modbus/tcp" -I"/media/D/MyProg/Robota/CM12/port" -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f0-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


