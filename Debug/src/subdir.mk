################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ROOT_GUI.cpp \
../src/UserGuiMainFrame.cpp 

OBJS += \
./src/ROOT_GUI.o \
./src/UserGuiMainFrame.o 

CPP_DEPS += \
./src/ROOT_GUI.d \
./src/UserGuiMainFrame.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I"/home/newdriver/Source/ROOT_CERN/root-6.08.00/root-6.08.00-build/include" -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


