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

##  Project name, enabled C also because of bzip2 which seems to need it
PROJECT (qscores-archiver CXX C)

##  Software version number
SET (QScores_VERSION_MAJOR ${QSCORES_VERSION_MAJOR})
SET (QScores_VERSION_MINOR ${QSCORES_VERSION_MINOR})

##  List of files, excluding the main driver
SET (QSCORES_SRCFILES 
  qscores.cpp 
  parameters.cpp 
  mutators.cpp 
  accessors.cpp 
  run.cpp io.cpp 
  transform.cpp 
  encode.cpp 
  decode.cpp 
  external.cpp 
  binning.cpp
)

########################################
##  Detect OpenMP, zlib, and bzlib2 -- must be before the creation of the configuration file

# FIND_PACKAGE (OpenMP)
# IF (OPENMP_FOUND)
#   SET (HAVE_OPENMP 1)
# ENDIF (OPENMP_FOUND)

FIND_PACKAGE (ZLIB)
FIND_PACKAGE (BZip2)

########################################
##  Create configuration file

##  Configure a header file to pass some of the CMake settings
##  to the source code
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/QScoresConfig.hpp.in"
  "${TOPLEVEL_PATH}/QScoresConfig.hpp"
  )

##  Add the binary tree to the search path for include files
INCLUDE_DIRECTORIES ("${TOPLEVEL_PATH}")


########################################
##  Create the targets

##  Create the test executable
ADD_EXECUTABLE (qscores-archiver main.cpp ${QSCORES_SRCFILES})
TARGET_LINK_LIBRARIES (qscores-archiver external_software_ar block_statistics_ar qscores_settings_ar qscores_single_ar bitio_ar bitbuffer_ar interpolative_ar huffman_ar ${Boost_LIBRARIES})
INSTALL (TARGETS qscores-archiver DESTINATION bin)


########################################
##  Add dependencies and directories

INCLUDE_DIRECTORIES (../../libraries/systemcfg ../../libraries/external-software ../../libraries/block-statistics ../../libraries/bitbuffer ../../libraries/bitio ../../libraries/huffman ../../libraries/interpolative ../../libraries/qscores-single ../../libraries/qscores-settings)
ADD_SUBDIRECTORY_ONCE (../../libraries/external-software ${CMAKE_CURRENT_BINARY_DIR}/external-software)
ADD_SUBDIRECTORY_ONCE (../../libraries/block-statistics ${CMAKE_CURRENT_BINARY_DIR}/block-statistics)
ADD_SUBDIRECTORY_ONCE (../../libraries/bitbuffer ${CMAKE_CURRENT_BINARY_DIR}/bitbuffer)
ADD_SUBDIRECTORY_ONCE (../../libraries/bitio ${CMAKE_CURRENT_BINARY_DIR}/bitio)
ADD_SUBDIRECTORY_ONCE (../../libraries/huffman ${CMAKE_CURRENT_BINARY_DIR}/huffman)
ADD_SUBDIRECTORY_ONCE (../../libraries/interpolative ${CMAKE_CURRENT_BINARY_DIR}/interpolative)
ADD_SUBDIRECTORY_ONCE (../../libraries/qscores-single ${CMAKE_CURRENT_BINARY_DIR}/qscores-single)
ADD_SUBDIRECTORY_ONCE (../../libraries/qscores-settings ${CMAKE_CURRENT_BINARY_DIR}/qscores-settings)


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
  IF (TARGET qscores-archiver)
    LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})
    INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES (qscores-archiver ${Boost_LIBRARIES})
  ENDIF (TARGET qscores-archiver)
ENDIF ()


########################################
##  Set various values based on libraries found

# IF (OPENMP_FOUND)
#   SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
# ENDIF (OPENMP_FOUND)

IF (ZLIB_FOUND)
  INCLUDE_DIRECTORIES (${ZLIB_INCLUDE_DIRS})
  TARGET_LINK_LIBRARIES (qscores-archiver ${ZLIB_LIBRARIES})
ENDIF (ZLIB_FOUND)

IF (BZIP2_FOUND)
  INCLUDE_DIRECTORIES (${BZIP2_INCLUDE_DIR})
  TARGET_LINK_LIBRARIES (qscores-archiver ${BZIP2_LIBRARIES})
  IF (NOT BZIP2_NEED_PREFIX)
    MESSAGE (WARNING "BZIP2 with functions beginning with BZ2_ is needed.  Please upgrade to version 1.0 if you wish to use it.")
  ENDIF (NOT BZIP2_NEED_PREFIX)
ENDIF (BZIP2_FOUND)


########################################
##  Documentation

##  Add a target to generate Doxygen documentation
FIND_PACKAGE (Doxygen)
IF (DOXYGEN_FOUND)
  CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  IF (NOT TARGET qscores-archiver-doc)
    ADD_CUSTOM_TARGET (qscores-archiver-doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Generating API documentation with Doxygen" VERBATIM)
    INSTALL (DIRECTORY ${MAIN_BINARY_PATH}/doc/qscores-archiver DESTINATION doc OPTIONAL)
  ENDIF (NOT TARGET qscores-archiver-doc)
ENDIF (DOXYGEN_FOUND)


############################################################
##  CTest
############################################################

ENABLE_TESTING ()
# ADD_TEST (NAME BitIO-ShowInfo COMMAND bitio-test --help)

