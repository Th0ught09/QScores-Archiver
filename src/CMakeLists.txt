###########################################################################
##  QScores-Archiver
##  Software and library for transforming and compressing next generation
##    sequencing data in FASTQ format.
##  
##  Copyright (C) 2011-2020 by Raymond Wan, All rights reserved.
##  Contact:  rwan.work@gmail.com
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

PROJECT ("${PROJECT_NAME}")
CMAKE_MINIMUM_REQUIRED (VERSION 3.2)

########################################
##  Set the main source path to the very top

##  Set the main path and where additional CMake modules are
IF (NOT DEFINED MAIN_SRC_PATH)
  SET (MAIN_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

  ##  Locate where the shared CMake modules are
  SET (CMAKE_MODULE_PATH "${MAIN_SRC_PATH}/aux")
ENDIF (NOT DEFINED MAIN_SRC_PATH)

##  Include externally defined functions
INCLUDE (policies)
INCLUDE (global)
INCLUDE (add_subdirectory_once)


########################################
##  Set up the software

##  Check if this is the top-level project
IF ("${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")
  INCLUDE (initial-msg)
ENDIF ("${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")


########################################
##  Create the targets

##  Set the output directory of the libraries to the top-level binary directory
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


########################################
##  Add dependencies and directories

##  Location of configuration files
INCLUDE_DIRECTORIES (${TARGET_NAME_EXEC} PUBLIC "${PROJECT_BINARY_DIR}")

##  Location of additional header files
INCLUDE_DIRECTORIES (${MAIN_SRC_PATH}/qscores)

##  Location of module dependencies
ADD_SUBDIRECTORY_ONCE (${MAIN_SRC_PATH}/qscores ${CMAKE_BINARY_DIR}/qscores)


########################################
##  Set initial compilation flags

##  Set compiler flags based on global variable
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_CXX_FLAGS}")


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

