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
PROJECT (external-software CXX C)

##  Software version number
SET (ExternalSoftware_VERSION_MAJOR ${QSCORES_VERSION_MAJOR})
SET (ExternalSoftware_VERSION_MINOR ${QSCORES_VERSION_MINOR})

##  List of files, excluding the main driver
SET (EXTERNAL_SOFTWARE_SRCFILES
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


########################################
##  Detect zlib and bzlib2 -- must be before the creation of the configuration file

FIND_PACKAGE (ZLIB)
FIND_PACKAGE (BZip2)

########################################
##  Create configuration file

##  Configure a header file to pass some of the CMake settings
##  to the source code
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/ExternalSoftwareConfig.hpp.in"
  "${TOPLEVEL_PATH}/ExternalSoftwareConfig.hpp"
  )

##  Add the binary tree to the search path for include files
INCLUDE_DIRECTORIES ("${TOPLEVEL_PATH}")


########################################
##  Create the targets

##  Create the test executable
IF (NOT TARGET external-software)
  ADD_EXECUTABLE (external-software main.cpp testing.cpp ${EXTERNAL_SOFTWARE_SRCFILES})
ENDIF (NOT TARGET external-software)

##  Create a library and change its output name
IF (NOT TARGET external_software_ar)
  ADD_LIBRARY (external_software_ar ${EXTERNAL_SOFTWARE_SRCFILES})
  SET_TARGET_PROPERTIES (external_software_ar PROPERTIES OUTPUT_NAME "external_software")
ENDIF (NOT TARGET external_software_ar)


########################################
##  Add dependencies and directories

INCLUDE_DIRECTORIES (../../libraries/systemcfg)

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
FIND_PACKAGE (Boost 1.42.0 REQUIRED COMPONENTS system filesystem)

IF (Boost_FOUND)
  LINK_DIRECTORIES ( ${Boost_LIBRARY_DIRS} )
  INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS})
  IF (TARGET external-software)
    TARGET_LINK_LIBRARIES (external-software ${Boost_LIBRARIES})
  ENDIF (TARGET external-software)
ENDIF ()


########################################
##  Set various values based on libraries found

IF (ZLIB_FOUND)
  INCLUDE_DIRECTORIES (${ZLIB_INCLUDE_DIRS})
  TARGET_LINK_LIBRARIES (external-software ${ZLIB_LIBRARIES})
ENDIF (ZLIB_FOUND)

IF (BZIP2_FOUND)
  INCLUDE_DIRECTORIES (${BZIP2_INCLUDE_DIR})
  TARGET_LINK_LIBRARIES (external-software ${BZIP2_LIBRARIES})
  IF (NOT BZIP2_NEED_PREFIX)
    MESSAGE (WARNING "BZIP2 with functions beginning with BZ2_ is needed.  Please upgrade to version 1.0 if you wish to use it.")
  ENDIF (NOT BZIP2_NEED_PREFIX)
ENDIF (BZIP2_FOUND)


########################################
##  Documentation

##  Add a target to generate Doxygen documentation
FIND_PACKAGE (Doxygen)
if (DOXYGEN_FOUND)
  CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  IF (NOT TARGET external-software-doc)
    ADD_CUSTOM_TARGET (external-software-doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Generating API documentation with Doxygen" VERBATIM)
    INSTALL (DIRECTORY ${MAIN_BINARY_PATH}/doc/external-software DESTINATION doc OPTIONAL)
  ENDIF (NOT TARGET external-software-doc)
ENDIF (DOXYGEN_FOUND)


############################################################
##  CTest
############################################################

ENABLE_TESTING ()
ADD_TEST (NAME ExternalSoftware-ShortInfo COMMAND external-software 1)
ADD_TEST (NAME ExternalSoftware-LongInfo COMMAND external-software 2)
ADD_TEST (NAME ExternalSoftware-GzipZlib-Small COMMAND external-software 3)
ADD_TEST (NAME ExternalSoftware-GzipZlib-Full COMMAND external-software 4 /usr/share/dict/words)
ADD_TEST (NAME ExternalSoftware-BzipBZlib-Small COMMAND external-software 5)
ADD_TEST (NAME ExternalSoftware-BzipBZlib-Full COMMAND external-software 6 /usr/share/dict/words)

