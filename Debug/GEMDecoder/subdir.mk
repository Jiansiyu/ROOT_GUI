################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../GEMDecoder/APVCrossTalkSearch.cpp \
../GEMDecoder/input_handler.cpp \
../GEMDecoder/raw_decoder.cpp 

OBJS += \
./GEMDecoder/APVCrossTalkSearch.o \
./GEMDecoder/input_handler.o \
./GEMDecoder/raw_decoder.o 

CPP_DEPS += \
./GEMDecoder/APVCrossTalkSearch.d \
./GEMDecoder/input_handler.d \
./GEMDecoder/raw_decoder.d 


# Each subdirectory must supply rules for building sources it contributes
GEMDecoder/%.o: ../GEMDecoder/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -pthread -I"/home/newdriver/Source/ROOT_CERN/root_v5.34.36/include" -I"/home/newdriver/Source/CODA/2.6.2/Linux-x86_64/include" -O2 -g3 -Wall -std=c++11 -pthread -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


