#----------------------------------------------------------------------------
# Setup the project
#

## Set the default build type
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Set build type (Debug is default)")

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(ROOT_GUI)

## Disable in-source builds!
if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(FATAL_ERROR "cmake cannot build PREX-counting in the root directory: "
    "${PROJECT_SOURCE_DIR}\nsince it conflicts with our old Makefile."
    " Create a build directory anywhere, change to it and re-run cmake."
    )
endif()


#8#---------------------------------------------------------------------------
file(GLOB sources analysis/*cpp analysis/*.cxx analysis/*.C GEMDetector/*.cpp msg/*cpp GUIDialog/*cpp src/*cpp)
file(GLOB header src/*.h analysis/*.h GUIDialog/*.h GEMDetector/*.h)
#8#----------------------------------------------------------------------------
## Set the sources which have a corresponding .h file here
#set(sources GUIDialog/UserGUIToolDialog.cpp GUIDialog/UserGUIWorkStatus.cpp
#        gui/UserRootGUI.cpp msg/SysMsgCenter.cpp root_gui/GUIConfigure.cpp
#        root_gui/GUIHelpDialog.cpp  root_gui/GUIInformation.cpp
#        root_gui/GUIMainFrame.cpp
#        root_gui/GUIMapWizard.cpp
#        root_gui/GUISignalSlot.cpp
#        root_gui/GUISysGeneral.cpp
#        root_gui/GUISysStatus.cpp
#        src/Benchmark.cpp
#        #src/ROOT_GUI.cpp
#        )
### Make corresponding header files
#string(REGEX REPLACE .cpp .h headers "${sources}")
## Append a few extra header files in the repository.  (But do not append the
## sbs_LinkDef.h header here, that is further down this cmake config.
#list(APPEND headers "DebugDef.h")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

## Setup default variables (copied from Makefile)
#FIXME: should be configure'd:
set(EXTRAWARN ON CACHE BOOL "g++ Wextra warn options if supported")

##----------------------------------------------------------------------------
set(VERBOSE ON CACHE BOOL "Compile extra code for printing verbose messages")
set(TESTCODE ON CACHE BOOL "Compile extra diagnostic code (extra computations and global variables")
set(MCDATA ON CACHE BOOL "Compile support code for MC input data")
#set(CXXMAXERRORS 0 CACHE INT "Maximum number of allowed errors before cxx stops")
list(APPEND PREXEXTRADEF_LIST VERBOSE TESTCODE MCDATA)

#----------------------------------------------------------------------------
# Find ROOT (using custom FindROOT.cmake taken from G4SBS)
set(minimum_root_version 5.10)
find_package(ROOT REQUIRED)
find_package(EVIO REQUIRED)
find_library(libconfig libconfig)

##----------------------------------------------------------------------------
## Check for SStream support
include(TestForSSTREAM)
if(CMAKE_HAS_ANSI_STRING_STREAM)
  set(HAS_SSTREAM ON)
  list(APPEND PREXEXTRADEF_LIST HAS_SSTREAM)
endif()

##----------------------------------------------------------------------------
## OS Type (not sure what exactly uses this, but since it's in the Makefile
## I'll just copy it here too...
set(PREX_LINKFLAGS "")
if(UNIX) ## Can be anything from Linux to Mac OS X
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")
  if(APPLE)
    set(MACVERS ON)
    list(APPEND PREXEXTRADEF_LIST MACVERS)
    set(PREX_LINKFLAGS "${PREX_LINKFLAGS} -Wl,-undefined,dynamic_lookup")
  else()
    set(LINUXVERS ON)
    list(APPEND PREXEXTRADEF_LIST LINUXVERS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual")
    if(EXTRAWARN AND CMAKE_COMPILER_IS_GNUCXX AND NOT
        CMAKE_CXX_COMPILER_VERSION LESS 4)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wno-missing-field-initializers")
    endif()
    if(CXXMAXERRORS GREATER 0)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmax-errors=${CXXMAXERRORS}")
    endif()
  endif()
endif() ## I guess no Windows support?



##----------------------------------------------------------------------------
## Copy over some flags we have set in the Makefile
if(VERBOSE)
  list(APPEND VERBOSE)
endif()

#include_directories(${ROOT_INCLUDE_DIR} ${Podd_INCLUDE_DIR} ${EVIO_INCLUDE_DIR})
include_directories(${ROOT_INCLUDE_DIR} ${EVIO_INCLUDE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")


ADD_EXECUTABLE(${PROJECT_NAME} ${sources} ${headers})

target_link_libraries(${PROJECT_NAME} PUBLIC ${ROOT_LIBRARIES} ${EVIO_LIBRARY})
target_link_libraries(${PROJECT_NAME} config++ Minuit)

message(AUTHOR_WARNING ${DEV_WARNING})

