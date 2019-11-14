cmake_minimum_required (VERSION 2.6 FATAL_ERROR)

project(NuFluxTools)

include(ExternalProject)

set (NuFluxTools_VERSION_MAJOR 0)
set (NuFluxTools_VERSION_MINOR 1)
set (NuFluxTools_VERSION_REVISION 0)

set (NuFluxTools_VERSION_STRING "v${NuFluxTools_VERSION_MAJOR}r${NuFluxTools_VERSION_MINOR}p${NuFluxTools_VERSION_REVISION}")

#Set this to TRUE to enable build debugging messages
set(BUILD_DEBUG_MSGS TRUE)
set(VERBOSE TRUE)
set(USE_DK2NU TRUE)
set(USE_FHICL TRUE)

include(${CMAKE_SOURCE_DIR}/cmake/cmessage.cmake)

#Changes default install path to be a subdirectory of the build dir.
#Can set build dir at configure time with -DCMAKE_INSTALL_PREFIX=/install/path
if(CMAKE_INSTALL_PREFIX STREQUAL "" OR CMAKE_INSTALL_PREFIX STREQUAL
  "/usr/local")
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
elseif(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE DEBUG)
elseif(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE DEBUG)
endif()

if(NOT DEFINED USE_GENIE)
  set(USE_GENIE false)
endif()

################################################################################
#                            Check Dependencies
################################################################################

SET(EXTRA_CXX_FLAGS)
SET(INCDIRS)
SET(EXTRA_LINK_DIRS)
SET(EXTRA_LIBS)

include(cmake/parseConfigApp.cmake)

###############################  dk2nu  #####################################
include(${CMAKE_SOURCE_DIR}/cmake/dk2nuSetup.cmake)

###############################  genie  #####################################
if(USE_GENIE)
  include(${CMAKE_SOURCE_DIR}/cmake/GENIESetup.cmake)
endif()

##################################  ROOT  ######################################
include(${CMAKE_SOURCE_DIR}/cmake/ROOTSetup.cmake)

################################  Prob3++  #####################################
# include(${CMAKE_SOURCE_DIR}/cmake/Prob3++Setup.cmake)

###############################  fhiclcpp  #####################################
include(${CMAKE_SOURCE_DIR}/cmake/fhiclcppSetup.cmake)

include_directories(src)
include_directories(${CMAKE_INSTALL_PREFIX}/include)

################################## COMPILER ####################################
include(${CMAKE_SOURCE_DIR}/cmake/c++CompilerSetup.cmake)

################################## DOXYGEN ####################################
#include(${CMAKE_SOURCE_DIR}/cmake/doxSetup.cmake)

add_subdirectory(src)
add_subdirectory(gdml)

configure_file(cmake/setup.sh.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/setup.sh" @ONLY)
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/setup.sh" DESTINATION
  ${CMAKE_INSTALL_PREFIX})
