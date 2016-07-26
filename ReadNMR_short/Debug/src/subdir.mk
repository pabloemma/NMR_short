################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ReadNMR.cpp 

OBJS += \
./src/ReadNMR.o 

CPP_DEPS += \
./src/ReadNMR.d 


# Each subdirectory must supply rules for building sources it contributes
src/ReadNMR.o: ../src/ReadNMR.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/Users/klein/root_all/include -I"/Users/klein/git/NMR_short/ReadNMR_short/include" -O0 -g3 -ggdb -pg -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/ReadNMR.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


