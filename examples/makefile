##############################################
#	subdir makefile	
#	author: Siyu Jian 
#	emal:   jiansiyu@gmail.com
#
#		Physics Department of UNiversity of Virginia
#		Charlottesville, VA, 22903
#		United States
##############################################

#+++++++++++++++++++++++++++++++++++++++++++++++
# CERN ROOT Related configration
#+++++++++++++++++++++++++++++++++++++++++++++++
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLDFLAGS  := $(shell root-config --ldflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
ROOTHASTHREAD    := $(shell root-config --has-thread)
ROOTAUXLIB       := $(shell root-config --auxlibs)
ROOTAUXCFLAG     := $(shell root-config --auxcflags)
#+++++++++++++++++++++++++++++++++++++++++++++++

CXXFLAGS  +=${ROOTCFLAGS} ${CFLAGS} -I${EVIO_INC} -I./ 
LDFLAGS	  +=${ROOTLDFLAGS}

ifeq ($(shell uname -s), Linux)
LIBS      +=${ROOTLIBS} ${ROOTGLIBS} -lMinuit -L${EVIO_LIB}  -lexpat -levioxx -levio  -lstdc++ -lm
else
LIBS      +=${ROOTLIBS} ${ROOTGLIBS} -lMinuit -L${EVIO_LIB}  -lexpat ${EVIO_LIB}/libevioxx.dylib ${EVIO_LIB}/libevio.dylib 
endif

GLIBS     +=${ROOTGLIBS} ${SYSLIBS}
LINKOPTION += -pthread -lm -ldl -lconfig++


