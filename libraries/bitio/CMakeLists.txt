###########################################################################
##  QScores-Archiver
##  Software and library for transforming and compressing next generation
##    sequencing data in FASTQ format.
##  
##  Version 1.00 -- November 6, 2011
##  
##  Copyright (C) 2011 by Raymond Wan, All rights reserved.
##  Contact:  rwan@cuhk.edu.hk
##  Organization:  Department of Computational Biology, Graduate School of
##                 Frontier Science, University of Tokyo and
##                 Computational Biology Research Center, AIST,
##                 Japan
##  
##  This file is part of QScores-Archiver.
##  
##  QScores-Archiver is free software; you can redistribute it and/or 
##  modify it under the terms of the GNU Lesser General Public License 
##  as published by the Free Software Foundation; either version 
##  3 of the License, or (at your option) any later version.
##  
##  QScores-Archiver is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU Lesser General Public License for more details.
##  
##  You should have received a copy of the GNU Lesser General Public 
##  License along with QScores-Archiver; if not, see 
##  <http://www.gnu.org/licenses/>.
###########################################################################


############################################################
##  CMake file
##
##  Raymond Wan
##  Organizations
##    - Department of Computational Biology, Graduate School of
##      Frontier Science, University of Tokyo
##    - Computational Biology Research Center, AIST, Japan
##
##  $Id: CMakeLists.txt 224 2011-11-06 15:32:09Z rwan $
############################################################
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)


########################################
##  Include externally defined functions

##  Set the main path and where additional CMake modules are
IF (NOT DEFINED MAIN_SRC_PATH)
  SET (MAIN_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../..)
  SET (CMAKE_MODULE_PATH ${MAIN_SRC_PATH}/cmake)
ENDIF (NOT DEFINED MAIN_SRC_PATH)

INCLUDE (global)
INCLUDE (add_subdirectory_once)

##  Set the top-level path if it isn't set already
IF (NOT DEFINED TOPLEVEL_PATH)
  SET (TOPLEVEL_PATH ${CMAKE_CURRENT_BINARY_DIR})
ENDIF (NOT DEFINED TOPLEVEL_PATH)


########################################
##  Set up the software

##  Project name
PROJECT (bitio CXX)

##  Software version number
SET (BitIO_VERSION_MAJOR ${QSCORES_VERSION_MAJOR})
SET (BitIO_VERSION_MINOR ${QSCORES_VERSION_MINOR})

##  List of files, excluding the main driver
SET (BITIO_SRCFILES
  binary.cpp
  delta.cpp
  gamma.cpp
  unary.cpp
  golomb.cpp
  rice.cpp
  bytecode.cpp
  math_utils.cpp
)


########################################
##  Create configuration file

##  Configure a header file to pass some of the CMake settings
##  to the source code
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/BitIOConfig.hpp.in"
  "${TOPLEVEL_PATH}/BitIOConfig.hpp"
  )

##  Add the binary tree to the search path for include files
INCLUDE_DIRECTORIES ("${TOPLEVEL_PATH}")


##########################################
##  Create the targets

##  Create the test executable
IF (NOT TARGET bitio-test)
  ADD_EXECUTABLE (bitio-test main-test.cpp parameters-test.cpp test_sequential.cpp test_random.cpp test_showlengths.cpp ${BITIO_SRCFILES})
  TARGET_LINK_LIBRARIES (bitio-test bitbuffer_ar)
ENDIF (NOT TARGET bitio-test)

##  Create a library and change its output name
IF (NOT TARGET bitio_ar)
  ADD_LIBRARY (bitio_ar ${BITIO_SRCFILES})
  SET_TARGET_PROPERTIES (bitio_ar PROPERTIES OUTPUT_NAME "bitio")
ENDIF (NOT TARGET bitio_ar)


########################################
##  Add dependencies and directories

INCLUDE_DIRECTORIES (../systemcfg ../bitbuffer)
ADD_SUBDIRECTORY_ONCE (../bitbuffer ${CMAKE_CURRENT_BINARY_DIR}/bitbuffer)
ADD_DEPENDENCIES (bitio-test ../bitbuffer)


########################################
##  Set initial compilation flags

##  Set compiler flags based on global variable
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_CXX_FLAGS}")


########################################
##  Set up for Boost

##  Set the module path to get the latest version of it
SET (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

##  Set the root path for Boost from the environment variable
# SET (Boost_DEBUG OFF)
SET (BOOST_ROOT $ENV{BOOST_ROOT})
SET (Boost_NO_SYSTEM_PATHS true)
SET (Boost_USE_STATIC_LIBS   ON)
SET (Boost_USE_MULTITHREADED ON)
SET (Boost_USE_STATIC_RUNTIME OFF)
FIND_PACKAGE (Boost 1.42.0 REQUIRED COMPONENTS program_options system filesystem)

IF (Boost_FOUND)
  IF (TARGET bitio-test)
    LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})
    INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES (bitio-test ${Boost_LIBRARIES})
  ENDIF (TARGET bitio-test)
ENDIF ()


########################################
##  Documentation

##  Add a target to generate Doxygen documentation
FIND_PACKAGE (Doxygen)
IF (DOXYGEN_FOUND)
  CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  IF (NOT TARGET bitio-doc)
    ADD_CUSTOM_TARGET (bitio-doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Generating API documentation with Doxygen" VERBATIM)
    INSTALL (DIRECTORY ${MAIN_BINARY_PATH}/doc/bitio DESTINATION doc OPTIONAL)
  ENDIF (NOT TARGET bitio-doc)
ENDIF (DOXYGEN_FOUND)


############################################################
##  CTest
############################################################

ENABLE_TESTING ()
ADD_TEST (NAME BitIO-ShowInfo COMMAND bitio-test --showinfo)
ADD_TEST (NAME BitIO-Unary-Sequential COMMAND bitio-test --method unary)
ADD_TEST (NAME BitIO-Binary-Sequential COMMAND bitio-test --method binary)
ADD_TEST (NAME BitIO-Gamma-Sequential COMMAND bitio-test --method gamma)
ADD_TEST (NAME BitIO-Delta-Sequential COMMAND bitio-test --method delta)
ADD_TEST (NAME BitIO-Golomb-Sequential COMMAND bitio-test --method golomb --param 5)
ADD_TEST (NAME BitIO-Rice-Sequential COMMAND bitio-test --method rice --param 2)
ADD_TEST (NAME BitIO-Unary-Random COMMAND bitio-test --method unary --random)
ADD_TEST (NAME BitIO-Binary-Random COMMAND bitio-test --method binary --random)
ADD_TEST (NAME BitIO-Gamma-Random COMMAND bitio-test --method gamma --random)
ADD_TEST (NAME BitIO-Delta-Random COMMAND bitio-test --method delta --random)
ADD_TEST (NAME BitIO-Golomb-Random COMMAND bitio-test --method golomb --random --param 5)
ADD_TEST (NAME BitIO-Rice-Random COMMAND bitio-test --method rice --random --param 2)
ADD_TEST (NAME BitIO-Unary-ShowLengths COMMAND bitio-test --method unary --showlengths 255)
ADD_TEST (NAME BitIO-Binary-ShowLengths COMMAND bitio-test --method binary --showlengths 255)
ADD_TEST (NAME BitIO-Gamma-ShowLengths COMMAND bitio-test --method gamma --showlengths 255)
ADD_TEST (NAME BitIO-Delta-ShowLengths COMMAND bitio-test --method delta --showlengths 255)
ADD_TEST (NAME BitIO-Golomb-ShowLengths COMMAND bitio-test --method golomb --showlengths 255 --param 5)
ADD_TEST (NAME BitIO-Rice-ShowLengths COMMAND bitio-test --method rice --showlengths 255 --param 2)

