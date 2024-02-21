###########################################################################
##  Copyright 2011-2015, 2024 by Raymond Wan (rwan.work@gmail.com)
##    https://github.com/rwanwork/QScores-Archiver
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


##  Set the minimum required CMake version
cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

##  Set policy CMP0144 to "new" (Run "cmake --help-policy CMP0144" for details.)
cmake_policy (SET CMP0144 NEW)


########################################
##  Variables specific to this project
SET (TARGET_NAME "qscores-single")
SET (TARGET_NAME_EXEC "qscores-single-test")
SET (CURR_PROJECT_NAME "QScoresSingle")

##  Source files for both the test executable and library
SET (SRC_FILES
  binning.cpp
  compress.cpp
  io.cpp
  lossless.cpp
  lossy.cpp
  mapping.cpp
  qscores-single.cpp
)

##  Source files for just the text executable
SET (TEST_FILES
  main.cpp
  testing.cpp
)

##  Header files that are included by other libraries / programs
SET (HEADER_FILES
)


########################################
##  Set the main source path to the very top

##  Set the main path and where additional CMake modules are
IF (NOT DEFINED MAIN_SRC_PATH)
  SET (MAIN_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/..")

  ##  Locate where the shared CMake modules are
  SET (CMAKE_MODULE_PATH "${MAIN_SRC_PATH}/aux")
ENDIF (NOT DEFINED MAIN_SRC_PATH)

##  Include externally defined functions
INCLUDE (policies)
INCLUDE (global)
INCLUDE (add_subdirectory_once)


########################################
##  Set up the software

##  Project name
PROJECT (${CURR_PROJECT_NAME} VERSION 1.0 LANGUAGES CXX)
ENABLE_LANGUAGE (C)
MESSAGE (STATUS "Setting up ${CURR_PROJECT_NAME}, version ${PROJECT_VERSION}...")

##  Check if this is the top-level project
IF ("${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")
  INCLUDE (initial-msg)
ENDIF ("${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")


########################################
##  Create configuration file

##  Configure a header file to pass some of the CMake settings
##  to the source code.
##  
##  The output header file is placed at the top-level binary directory.
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/${CURR_PROJECT_NAME}Config.hpp.in"
  "${CMAKE_BINARY_DIR}/${CURR_PROJECT_NAME}Config.hpp"
  @ONLY
)


########################################
##  Create the targets

##  Set the output directory of the libraries to the top-level binary directory
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

##  Create the test executable
IF (NOT TARGET ${TARGET_NAME_EXEC})
  ADD_EXECUTABLE (${TARGET_NAME_EXEC} ${TEST_FILES} ${SRC_FILES})
  TARGET_LINK_LIBRARIES (${TARGET_NAME_EXEC} block-statistics bitio bitbuffer interpolative)
  INSTALL (TARGETS ${TARGET_NAME_EXEC} DESTINATION bin)
ENDIF (NOT TARGET ${TARGET_NAME_EXEC})

##  Create a library and change its output name
IF (NOT TARGET ${TARGET_NAME})
  ADD_LIBRARY (${TARGET_NAME} SHARED ${SRC_FILES})
  INSTALL (TARGETS ${TARGET_NAME} DESTINATION lib)
ENDIF (NOT TARGET ${TARGET_NAME})


########################################
##  Add dependencies and directories

##  Location of configuration files
INCLUDE_DIRECTORIES (${TARGET_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

##  Location of additional header files
INCLUDE_DIRECTORIES (${MAIN_SRC_PATH}/common)
INCLUDE_DIRECTORIES (${MAIN_SRC_PATH}/block-statistics)
INCLUDE_DIRECTORIES (${MAIN_SRC_PATH}/bitbuffer)
INCLUDE_DIRECTORIES (${MAIN_SRC_PATH}/bitio)
INCLUDE_DIRECTORIES (${MAIN_SRC_PATH}/interpolative)

##  Location of module dependencies
ADD_SUBDIRECTORY_ONCE (${MAIN_SRC_PATH}/block-statistics ${CMAKE_CURRENT_BINARY_DIR}/block-statistics)
ADD_SUBDIRECTORY_ONCE (${MAIN_SRC_PATH}/bitio ${CMAKE_CURRENT_BINARY_DIR}/bitio)
ADD_SUBDIRECTORY_ONCE (${MAIN_SRC_PATH}/bitbuffer ${CMAKE_CURRENT_BINARY_DIR}/bitbuffer)
ADD_SUBDIRECTORY_ONCE (${MAIN_SRC_PATH}/interpolative ${CMAKE_CURRENT_BINARY_DIR}/interpolative)


########################################
##  Set initial compilation flags

##  Set compiler flags based on global variable
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_CXX_FLAGS}")


########################################
##  Documentation

INCLUDE (doxygen)


########################################
##  Final message

##  Check if this is the top-level project
IF ("${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")
  INCLUDE (final-msg)
ENDIF ("${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")


############################################################
##  CTest
############################################################

ENABLE_TESTING ()
ADD_TEST (NAME QScoresSingle-ShortInfo COMMAND ${TARGET_NAME_EXEC} 1)
ADD_TEST (NAME QScoresSingle-SimpleTest COMMAND ${TARGET_NAME_EXEC} 2)
ADD_TEST (NAME QScoresSingle-IOTest COMMAND ${TARGET_NAME_EXEC} 3)
ADD_TEST (NAME QScoresSingle-MinShift1 COMMAND ${TARGET_NAME_EXEC} 4 "!!!~n(~")
ADD_TEST (NAME QScoresSingle-MinShift2 COMMAND ${TARGET_NAME_EXEC} 4 "BACCECE")
ADD_TEST (NAME QScoresSingle-FreqOrder1 COMMAND ${TARGET_NAME_EXEC} 5 "!!!~n(~")
ADD_TEST (NAME QScoresSingle-FreqOrder2 COMMAND ${TARGET_NAME_EXEC} 5 "BACCECE")
ADD_TEST (NAME QScoresSingle-GapTrans1 COMMAND ${TARGET_NAME_EXEC} 6 "!!!~n(~")
ADD_TEST (NAME QScoresSingle-GapTrans2 COMMAND ${TARGET_NAME_EXEC} 6 "BACCECE")
ADD_TEST (NAME QScoresSingle-LogBinning1 COMMAND ${TARGET_NAME_EXEC} 7 "!!!~n(~" 3)
ADD_TEST (NAME QScoresSingle-LogBinning2 COMMAND ${TARGET_NAME_EXEC} 7 "BACCECE" 3)
ADD_TEST (NAME QScoresSingle-UniBinning1 COMMAND ${TARGET_NAME_EXEC} 8 "!!!~n(~" 100)
ADD_TEST (NAME QScoresSingle-UniBinning2 COMMAND ${TARGET_NAME_EXEC} 8 "BACCECE" 100)
ADD_TEST (NAME QScoresSingle-LogBinningGapTrans1 COMMAND ${TARGET_NAME_EXEC} 9 "!!!~n(~" 3)
ADD_TEST (NAME QScoresSingle-LogBinningGapTrans2 COMMAND ${TARGET_NAME_EXEC} 9 "BACCECE" 3)


