# Copyright 2016 L. Pickering, P Stowell, R. Terri, C. Wilkinson, C. Wret

################################################################################
#    This file is part of NUISANCE.
#
#    NUISANCE is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    NUISANCE is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with NUISANCE.  If not, see <http://www.gnu.org/licenses/>.
################################################################################

################################################################################
#                            Check Dependencies
################################################################################

#################################  GENIE  ######################################
if("$ENV{GENIE} " STREQUAL " ")
  cmessage(FATAL_ERROR "Variable GENIE is not defined. "
    "The location of a pre-built GENIE install must be defined either as"
    " $ cmake -DGENIE=/path/to/GENIE or as an environment variable"
    " $ export GENIE=/path/to/GENIE")
endif()
SET(GENIE $ENV{GENIE})

execute_process(COMMAND genie-config --version
OUTPUT_VARIABLE GENIE_VER OUTPUT_STRIP_TRAILING_WHITESPACE)
cmessage(STATUS "genie_ver: ${GENIE_VER}")
if(GENIE_VER VERSION_GREATER 3.0.0)
  set(GENIE_V3 true)
  string(REPLACE "." "" GENIE_VERSION ${GENIE_VER})
  LIST(APPEND EXTRA_CXX_FLAGS -DIS_GENIE_V3)
endif()

execute_process (COMMAND genie-config
  --topsrcdir OUTPUT_VARIABLE GENIE_INCLUDES_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)

GETLIBDIRS(genie-config --libs GENIE_LIB_DIR)
GETLIBS(genie-config --libs GENIE_LIBS)

cmessage(STATUS "GENIE version : ${GENIE_VERSION}")
cmessage(STATUS "GENIE libdir  : ${GENIE_LIB_DIR}")
cmessage(STATUS "GENIE libs    : ${GENIE_LIBS}")

string(REGEX MATCH "ReinSeghal" WASMATCHED ${GENIE_LIBS})
if(WASMATCHED AND GENIE_VERSION STREQUAL "210")
  set(GENIE_SEHGAL ${GENIE_LIBS})
  STRING(REPLACE "ReinSeghal" "ReinSehgal" GENIE_LIBS ${GENIE_SEHGAL})
  cmessage(DEBUG "Fixed inconsistency in library naming: ${GENIE_LIBS}")
endif()

LIST(APPEND GENIE_LIBS -Wl,--end-group )
LIST(REVERSE GENIE_LIBS)
LIST(APPEND GENIE_LIBS -Wl,--start-group -Wl,--no-as-needed )
LIST(REVERSE GENIE_LIBS)

cmessage(DEBUG "GENIE_LIBS: ${GENIE_LIBS}")

################################  LHAPDF  ######################################
if("$ENV{LHAPDF_LIB} " STREQUAL " ")
  cmessage(FATAL_ERROR "Variable LHAPDF_LIB is not defined. The location of a pre-built lhapdf install must be defined either as $ cmake -DLHAPDF_LIB=/path/to/LHAPDF_libraries or as an environment variable $ export LHAPDF_LIB=/path/to/LHAPDF_libraries")
else()
  SET(LHAPDF_LIB $ENV{LHAPDF_LIB})
endif()

if("$ENV{LHAPDF_INC} " STREQUAL " ")
  cmessage(FATAL_ERROR "Variable LHAPDF_INC is not defined. The location of a pre-built lhapdf install must be defined either as $ cmake -DLHAPDF_INC=/path/to/LHAPDF_includes or as an environment variable $ export LHAPDF_INC=/path/to/LHAPDF_includes")
else()
  SET(LHAPDF_INC $ENV{LHAPDF_INC})
endif()

if("$ENV{LHAPATH} " STREQUAL " ")
  cmessage(FATAL_ERROR "Variable LHAPATH is not defined. The location of a the LHAPATH directory must be defined either as $ cmake -DLHAPATH=/path/to/LHAPATH or as an environment variable $ export LHAPATH=/path/to/LHAPATH")
else()
  SET(LHAPATH $ENV{LHAPATH})
endif()

################################  LIBXML  ######################################
if("$ENV{LIBXML2_LIB} " STREQUAL " ")
  GETLIBDIR(xml2-config --libs LIBXML2_LIB IGNORE_EMPTY_RESPONSE)
  if("${LIBXML2_LIB} " STREQUAL " ")
    message(WARNING "Variable LIBXML2_LIB is not defined, as xml2-config was found and didn't report a library include path, it is likely that libxml2.so can be found in the standard system location, lets hope so. Alternativly, a location can be forced by configering with -DLIBXML2_LIB=/path/to/LIBXML2_libraries or as an environment variable LIBXML2_LIB.")
  endif()
else()
  SET(LIBXML2_LIB $ENV{LIBXML2_LIB})
endif()

if("$ENV{LIBXML2_INC} " STREQUAL " ")
  GETINCDIR(xml2-config --cflags LIBXML2_INC IGNORE_EMPTY_RESPONSE)
  if("${LIBXML2_INC} " STREQUAL " ")
    message(WARNING "Variable LIBXML2_INC is not defined, as xml2-config was found and didn't report an include path, it is likely that libxml2.so can be found in the standard system location, lets hope so. Alternativly, a location can be forced by configering with -DLIBXML2_INC=/path/to/LIBXML2_includes or as an environment variable LIBXML2_INC.")
  endif()
else()
  SET(LIBXML2_INC $ENV{LIBXML2_INC})
endif()

###############################  log4cpp  ######################################
if("$ENV{LOG4CPP_LIB} " STREQUAL " ")
  GETLIBDIR(log4cpp-config --libs LOG4CPP_LIB IGNORE_EMPTY_RESPONSE)
  if("${LOG4CPP_LIB} " STREQUAL " ")
    message(WARNING "Variable LOG4CPP_LIB is not defined, as xml2-config was found and didn't report a library include path, it is likely that liblog4cpp.so can be found in the standard system location, lets hope so. Alternativly, a location can be forced by configering with -DLOG4CPP_LIB=/path/to/LOG4CPP_libraries or as an environment variable LOG4CPP_LIB.")
  endif()
else()
  SET(LOG4CPP_LIB $ENV{LOG4CPP_LIB})
endif()

if("$ENV{LOG4CPP_INC} " STREQUAL " ")
  GETINCDIR(log4cpp-config --cflags LOG4CPP_INC IGNORE_EMPTY_RESPONSE)
  if("${LOG4CPP_INC} " STREQUAL " ")
    message(WARNING "Variable LOG4CPP_LIB is not defined, as xml2-config was found and didn't report an include path, it is likely that log4cpp headers can be found in the standard system location, lets hope so. Alternativly, a location can be forced by configering with -DLOG4CPP_INC=/path/to/LOG4CPP_includes or as an environment variable LOG4CPP_INC.")
  endif()
else()
  SET(LOG4CPP_INC $ENV{LOG4CPP_INC})
endif()
################################################################################

LIST(APPEND EXTRA_LIBS ${GENIE_LIBS})

###############################  GSL  ######################################
if(GENIE_V3)
  if("$ENV{GSL_LIB} " STREQUAL " ")
    GETLIBDIR(gsl-config --libs GSL_LIB)
    if("${GSL_LIB} " STREQUAL " ")
      message(FATAL_ERROR "Variable GSL_LIB is not defined and could not be found with gsl-config. The location of a pre-built gsl install must be defined either as $ cmake -DGSL_LIB=/path/to/GSL_libraries or as an environment variable $ export GSL_LIB=/path/to/GSL_libraries")
    endif()
  else()
    SET(GSL_LIB $ENV{GSL_LIB})
  endif()

  if("$ENV{GSL_INC} " STREQUAL " ")
    GETINCDIR(gsl-config --cflags GSL_INC)
    if("${GSL_INC} " STREQUAL " ")
      message(FATAL_ERROR "Variable GSL_INC is not defined and could not be found with gsl-config. The location of a pre-built gsl install must be defined either as $ cmake -DGSL_INC=/path/to/GSL_includes or as an environment variable $ export GSL_INC=/path/to/GSL_includes")
    endif()
  else()
    SET(GSL_INC $ENV{GSL_INC})
  endif()

  GETLIBS(gsl-config --libs GSL_LIB_LIST)

endif()
################################################################################

LIST(APPEND EXTRA_LIBS LHAPDF xml2 log4cpp)

LIST(APPEND EXTRA_LINK_DIRS
  ${GENIE_LIB_DIR}
  ${LHAPDF_LIB}
  ${LOG4CPP_LIB})

include_directories(${GENIE}/src)

SET(NEED_ROOTPYTHIA6 true)
