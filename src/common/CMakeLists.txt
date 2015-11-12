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
SET (TARGET_NAME "common")
SET (TARGET_NAME_EXEC "common-test")
SET (CURR_PROJECT_NAME "Common")

##  Source files for just the text executable
SET (TEST_FILES
  main.cpp
)

##  Header files
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
MESSAGE (STATUS "Setting up ${CURR_PROJECT_NAME}, version ${PROJECT_VERSION}...")

##  Check if this is the top-level project
IF ("${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")
  INCLUDE (initial-msg)
ENDIF ("${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")


########################################
##  Create the targets

##  Create the test executable
ADD_EXECUTABLE (${TARGET_NAME_EXEC} ${TEST_FILES})


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
ADD_TEST (NAME Common-Architecture COMMAND ${TARGET_NAME_EXEC})


