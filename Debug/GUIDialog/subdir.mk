################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../GUIDialog/UserGUIHelpDialog.cpp \
../GUIDialog/UserGUIToolDialog.cpp 

OBJS += \
./GUIDialog/UserGUIHelpDialog.o \
./GUIDialog/UserGUIToolDialog.o 

CPP_DEPS += \
./GUIDialog/UserGUIHelpDialog.d \
./GUIDialog/UserGUIToolDialog.d 


# Each subdirectory must supply rules for building sources it contributes
GUIDialog/%.o: ../GUIDialog/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -pthread -I"/home/newdriver/Source/ROOT_CERN/root_v5.34.36/include" -I"/home/newdriver/Source/CODA/2.6.2/Linux-x86_64/include" -O2 -g3 -Wall -std=c++11 -pthread -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


