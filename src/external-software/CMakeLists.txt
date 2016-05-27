###########################################################################
##  QScores-Archiver
##  Software and library for transforming and compressing next generation
##    sequencing data in FASTQ format.
##  
##  Version N/A (On Github) -- November 12, 2015
##  
##  Copyright (C) 2011-2015 by Raymond Wan, All rights reserved.
##  Contact:  rwan.work@gmail.com
##  Organization:  Division of Life Science, Faculty of Science, Hong Kong
##                 University of Science and Technology, Hong Kong
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
CMAKE_MINIMUM_REQUIRED (VERSION 3.2)


########################################
##  Variables specific to this project
SET (TARGET_NAME "external-software")
SET (TARGET_NAME_EXEC "external-software-test")
SET (CURR_PROJECT_NAME "ExternalSoftware")

##  Source files for both the test executable and library
SET (SRC_FILES
  external-software.cpp
  accessors.cpp
  mutators.cpp
  setup.cpp
  process.cpp
  gzip-zlib.cpp
  bzip-bzlib.cpp
  repair-shuff.cpp
  retrieve.cpp
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
##  Detect zlib and bzlib2 -- must be before the creation of the configuration file

FIND_PACKAGE (ZLIB)
FIND_PACKAGE (BZip2)


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
  INSTALL (TARGETS ${TARGET_NAME_EXEC} DESTINATION bin)
ENDIF (NOT TARGET ${TARGET_NAME_EXEC})
  
##  Create a library and change its output name
IF (NOT TARGET ${TARGET_NAME})
  ADD_LIBRARY (${TARGET_NAME} SHARED ${SRC_FILES})
  INSTALL (TARGETS ${TARGET_NAME} DESTINATION lib)
ENDIF (NOT TARGET ${TARGET_NAME})


########################################
##  Add dependencies and directories

##  Location of configuration file
INCLUDE_DIRECTORIES (${TARGET_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

##  Location of additional header files
INCLUDE_DIRECTORIES (${MAIN_SRC_PATH}/common)


########################################
##  Set initial compilation flags

##  Set compiler flags based on global variable
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_CXX_FLAGS}")


########################################
##  Set up for Boost

INCLUDE (boost)


########################################
##  Set various values based on libraries found

INCLUDE (post-compress)


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
ADD_TEST (NAME ExternalSoftware-ShortInfo COMMAND ${TARGET_NAME_EXEC} 1)
ADD_TEST (NAME ExternalSoftware-LongInfo COMMAND ${TARGET_NAME_EXEC} 2)
ADD_TEST (NAME ExternalSoftware-GzipZlib-Small COMMAND ${TARGET_NAME_EXEC} 3)
ADD_TEST (NAME ExternalSoftware-GzipZlib-Full COMMAND ${TARGET_NAME_EXEC} 4 /usr/share/dict/words)
ADD_TEST (NAME ExternalSoftware-BzipBZlib-Small COMMAND ${TARGET_NAME_EXEC} 5)
ADD_TEST (NAME ExternalSoftware-BzipBZlib-Full COMMAND ${TARGET_NAME_EXEC} 6 /usr/share/dict/words)

