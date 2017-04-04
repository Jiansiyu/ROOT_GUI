################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_UPPER_SRCS += \
../analysis/GEMTracking.C \
../analysis/GEMTree.C 

CXX_SRCS += \
../analysis/GEMCluster.cxx \
../analysis/GEMHit.cxx \
../analysis/GEMTCluster.cxx \
../analysis/GEMTreeGenerator.cxx 

CPP_SRCS += \
../analysis/GEMCalibration.cpp \
../analysis/GEMEffAnalysis.cpp \
../analysis/GEMTrackConstrcution.cpp 

OBJS += \
./analysis/GEMCalibration.o \
./analysis/GEMCluster.o \
./analysis/GEMEffAnalysis.o \
./analysis/GEMHit.o \
./analysis/GEMTCluster.o \
./analysis/GEMTrackConstrcution.o \
./analysis/GEMTracking.o \
./analysis/GEMTree.o \
./analysis/GEMTreeGenerator.o 

C_UPPER_DEPS += \
./analysis/GEMTracking.d \
./analysis/GEMTree.d 

CXX_DEPS += \
./analysis/GEMCluster.d \
./analysis/GEMHit.d \
./analysis/GEMTCluster.d \
./analysis/GEMTreeGenerator.d 

CPP_DEPS += \
./analysis/GEMCalibration.d \
./analysis/GEMEffAnalysis.d \
./analysis/GEMTrackConstrcution.d 


# Each subdirectory must supply rules for building sources it contributes
analysis/%.o: ../analysis/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -pthread -I"/home/newdriver/Source/ROOT_CERN/root_v5.34.36/include" -I"/home/newdriver/Source/CODA/2.6.2/Linux-x86_64/include" -O2 -g3 -Wall -std=c++11 -pthread -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

analysis/%.o: ../analysis/%.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -pthread -I"/home/newdriver/Source/ROOT_CERN/root_v5.34.36/include" -I"/home/newdriver/Source/CODA/2.6.2/Linux-x86_64/include" -O2 -g3 -Wall -std=c++11 -pthread -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

analysis/%.o: ../analysis/%.C
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -pthread -I"/home/newdriver/Source/ROOT_CERN/root_v5.34.36/include" -I"/home/newdriver/Source/CODA/2.6.2/Linux-x86_64/include" -O2 -g3 -Wall -std=c++11 -pthread -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


