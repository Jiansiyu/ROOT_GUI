##############################################
#	SUBDIRS MAKEFILE	
#	author: Siyu Jian 
#	emal:   jiansiyu@gmail.com
#
#		Physics Department of UNiversity of Virginia
#		Charlottesville, VA, 22903
#		United States
##############################################


#+++++++++++++++++++++++++++++++++++++++++++
# You make need to modify the following paths according to you system setting




#+++++++++++++++++++++++++++++++++++++++++++

CXX           =
ObjSuf        = o
SrcSuf        := .cxx .C .cpp
ExeSuf        =
DllSuf        = so
OutPutOpt     = -o # keep whitespace after "-o"
bindir	      = bin
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLDFLAGS  := $(shell root-config --ldflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs) -lMinuit
HASTHREAD    := $(shell root-config --has-thread)

CXX           = g++
CXXFLAGS      = -fPIC -Wall -O3 -g3
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
LDFLAGS      += $(ROOTLDFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)

ROOTFLAGS    := ${LIBS} ${GLIBS} ${CXXFLAGS}


OBJS	+= $(addprefix ./bin/analysis/, $(notdir ${patsubst %.cpp, %.o, ${wildcard ./analysis/*.cpp}}))
OBJS    += $(addprefix ./bin/decoder/, $(notdir ${patsubst %.cpp, %.o, ${wildcard ./decoder/*.cpp}}))

TARGET = GEM_Analysis
all: ${TARGET}
THIS_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

GEM_Analysis: ${OBJS}
	@echo ${THIS_DIR}
	@echo ${OBJS}
	@echo 'Building target: $@' 
	@$(CC) -pthread -g -o $@ $^ ${ROOTFLAGS} -lstdc++ -lm  -lSpectrum
	@echo 'Finish building: $<'
	@echo 


./bin/analysis/%.o : ./analysis/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@$(CC) -std=c++0x ${ROOTCFLAGS} -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finish building: $<'
	@echo
./bin/decoder/%.o : ./decoder/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@$(CC) -std=c++0x ${ROOTCFLAGS} -c  -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finish building: $<'
	@echo
PHONY: clean

clean:
	${RM} ${OBJS} ${TARGET} *~
