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
PROJECT (qscores-settings CXX)

##  Software version number
SET (QScoresSettings_VERSION_MAJOR ${QSCORES_VERSION_MAJOR})
SET (QScoresSettings_VERSION_MINOR ${QSCORES_VERSION_MINOR})

##  List of files, excluding the main driver
SET (QSCORES_SETTINGS_SRCFILES
  qscores-settings.cpp
  accessors.cpp
  mutators.cpp
)


########################################
##  Create configuration file

##  Configure a header file to pass some of the CMake settings
##  to the source code
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/QScoresSettingsConfig.hpp.in"
  "${TOPLEVEL_PATH}/QScoresSettingsConfig.hpp"
  )

##  Add the binary tree to the search path for include files
INCLUDE_DIRECTORIES ("${TOPLEVEL_PATH}")


########################################
##  Create the targets

##  Create the test executable
IF (NOT TARGET qscores-settings)
  ADD_EXECUTABLE (qscores-settings main.cpp ${QSCORES_SETTINGS_SRCFILES})
  TARGET_LINK_LIBRARIES (qscores-settings bitio_ar bitbuffer_ar)
ENDIF (NOT TARGET qscores-settings)

##  Create a library and change its output name
IF (NOT TARGET qscores_settings_ar)
  ADD_LIBRARY (qscores_settings_ar ${QSCORES_SETTINGS_SRCFILES})
  SET_TARGET_PROPERTIES (qscores_settings_ar PROPERTIES OUTPUT_NAME "qscores_settings")
ENDIF (NOT TARGET qscores_settings_ar)


########################################
##  Add dependencies and directories

INCLUDE_DIRECTORIES (../../libraries/systemcfg ../../libraries/bitbuffer ../../libraries/bitio ../../libraries/qscores-single)
ADD_SUBDIRECTORY_ONCE (../../libraries/bitbuffer ${CMAKE_CURRENT_BINARY_DIR}/bitbuffer)
ADD_SUBDIRECTORY_ONCE (../../libraries/bitio ${CMAKE_CURRENT_BINARY_DIR}/bitio)
ADD_SUBDIRECTORY_ONCE (../../libraries/qscores-single ${CMAKE_CURRENT_BINARY_DIR}/qscores-single)
ADD_DEPENDENCIES (qscores-settings ../../libraries/bitbuffer)
ADD_DEPENDENCIES (qscores-settings ../../libraries/bitio)
ADD_DEPENDENCIES (qscores-settings ../../libraries/qscores-single)

########################################
##  Set initial compilation flags

##  Set compiler flags based on global variable
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_CXX_FLAGS}")


########################################
##  Documentation

##  Add a target to generate Doxygen documentation
FIND_PACKAGE (Doxygen)
IF (DOXYGEN_FOUND)
  CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  IF (NOT TARGET qscores-settings-doc)
    ADD_CUSTOM_TARGET (qscores-settings-doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Generating API documentation with Doxygen" VERBATIM)
    INSTALL (DIRECTORY ${MAIN_BINARY_PATH}/doc/qscores-settings DESTINATION doc OPTIONAL)
  ENDIF (NOT TARGET qscores-settings-doc)
ENDIF (DOXYGEN_FOUND)


############################################################
##  CTest
############################################################

ENABLE_TESTING ()
ADD_TEST (NAME QScoresSettings-Default COMMAND qscores-settings 1)
ADD_TEST (NAME QScoresSettings-All-On COMMAND qscores-settings 2)

