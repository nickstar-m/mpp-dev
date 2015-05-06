################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../port/portevent.c \
../port/portserial.c \
../port/porttimer.c 

OBJS += \
./port/portevent.o \
./port/portserial.o \
./port/porttimer.o 

C_DEPS += \
./port/portevent.d \
./port/portserial.d \
./port/porttimer.d 


# Each subdirectory must supply rules for building sources it contributes
port/%.o: ../port/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DSTM32F030 -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DSTM32F030 -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f0-stdperiph" -I"/media/D/MyProg/Robota/CM12/modbus/include" -I"/media/D/MyProg/Robota/CM12/modbus/ascii" -I"/media/D/MyProg/Robota/CM12/modbus/rtu" -I"/media/D/MyProg/Robota/CM12/modbus/tcp" -I"/media/D/MyProg/Robota/CM12/port" -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f0-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


