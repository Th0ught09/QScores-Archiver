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
##  Define the project name and target(s)

set (CURR_PROJECT_NAME "QScores")
set (TARGET_NAME_EXEC "qscores")

add_executable (${TARGET_NAME_EXEC} "")


########################################
##  Set the C++ standard

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

##  Use target_compile_features () for macOS
target_compile_features (${TARGET_NAME_LIB} PUBLIC cxx_std_11)
target_compile_features (${TARGET_NAME_EXEC} PUBLIC cxx_std_11)


########################################
##  Define the source files

##  Source files for the main program and library
set (CPP_FILES
  accessors.cpp
  binning.cpp
  decode.cpp
  encode.cpp
  external.cpp
  io.cpp
  mutators.cpp
  parameters.cpp
  qscores.cpp
  run.cpp
  transform.cpp
)

##  Source files for just the main program
set (EXE_CPP_FILES
  main.cpp
)

##  Header files for the main program and library
set (HPP_FILES
)

##  Header files for just the main program
set (EXE_HPP_FILES
)


########################################
##  Set the global path

##  If the MAIN_SRC_PATH has not been defined yet
if (NOT DEFINED MAIN_SRC_PATH)
  ##  Set the main source path to the very top
  set (MAIN_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/..")

  ##  Locate where the shared CMake modules are
  list (APPEND CMAKE_MODULE_PATH "${MAIN_SRC_PATH}/cmake")
endif ()

##  Include externally defined modules
include (add_subdirectory_once)


########################################
##  Set up the software
include (initial-msg)

project (${CURR_PROJECT_NAME} VERSION 1.0 DESCRIPTION "QScores" LANGUAGES CXX)
message (STATUS "Setting up ${CURR_PROJECT_NAME}...")


########################################
##  Set initial compilation flags

include (compile-flags)


########################################
##  Obtain the Git hash and version

include (git-hash)
include (version)


########################################
##  Set up for Boost

include (boost)


########################################
##  Detect zlib, and bzlib2 -- must be before the creation of the configuration file

find_package (ZLIB)
find_package (BZip2)


########################################
##  Create configuration file

##  Configure a header file to pass some of the CMake settings
##  to the source code.
##
##  The output header file is placed at the top-level binary directory.
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/${CURR_PROJECT_NAME}_Config.hpp.in"
  "${CMAKE_BINARY_DIR}/generated/${CURR_PROJECT_NAME}_Config.hpp"
  @ONLY
)

##  Include the generated/ directory so that the created configuration
##    file can be located
include_directories (${CMAKE_BINARY_DIR}/generated)


########################################
##  Update the targets

##  Update an executable
if (TARGET ${TARGET_NAME_EXEC})
  ##  Add sources to the target
  target_sources (${TARGET_NAME_EXEC} PRIVATE ${CPP_FILES})
  target_sources (${TARGET_NAME_EXEC} PRIVATE ${EXE_CPP_FILES})
  target_sources (${TARGET_NAME_EXEC} PRIVATE ${HPP_FILES})
  target_sources (${TARGET_NAME_EXEC} PRIVATE ${EXE_HPP_FILES})

  ##  Rename the executable
  set_property (TARGET genomicfeatures_exe PROPERTY OUTPUT_NAME genomicfeatures)

  if (Boost_FOUND)
    target_include_directories (${TARGET_NAME_EXEC} PUBLIC "${Boost_INCLUDE_DIRS}")
    target_link_libraries (${TARGET_NAME_EXEC} ${Boost_LIBRARIES})
  endif ()

  target_link_libraries (${TARGET_NAME_EXEC}
    qscores-settings 
    qscores-single 
    bitio 
    bitbuffer 
    interpolative 
    huffman 
    external-software 
    block-statistics 
    ${Boost_LIBRARIES}
  )

  install (TARGETS ${TARGET_NAME_EXEC} DESTINATION bin)
endif ()

##  Set the output directory of the libraries to the top-level binary directory
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


########################################
##  Add dependencies and directories

##  Location of configuration files
# INCLUDE_DIRECTORIES (${TARGET_NAME_EXEC} PUBLIC "${PROJECT_BINARY_DIR}")

##  Location of additional header files
include_directories (
  ${MAIN_SRC_PATH}/common
  ${MAIN_SRC_PATH}/block-statistics
  ${MAIN_SRC_PATH}/bitbuffer
  ${MAIN_SRC_PATH}/biotio
  ${MAIN_SRC_PATH}/huffman
  ${MAIN_SRC_PATH}/interpolative
  ${MAIN_SRC_PATH}/qscores-single
  ${MAIN_SRC_PATH}/qscores-settings
  ${MAIN_SRC_PATH}/external-software
)

##  Location of module dependencies
add_subdirectory_once (${MAIN_SRC_PATH}/external-software ${CMAKE_BINARY_DIR}/external-software)
add_subdirectory_once (${MAIN_SRC_PATH}/block-statistics ${CMAKE_BINARY_DIR}/block-statistics)
add_subdirectory_once (${MAIN_SRC_PATH}/bitbuffer ${CMAKE_BINARY_DIR}/bitbuffer)
add_subdirectory_once (${MAIN_SRC_PATH}/bitio ${CMAKE_BINARY_DIR}/bitio)
add_subdirectory_once (${MAIN_SRC_PATH}/huffman ${CMAKE_BINARY_DIR}/huffman)
add_subdirectory_once (${MAIN_SRC_PATH}/interpolative ${CMAKE_BINARY_DIR}/interpolative)
add_subdirectory_once (${MAIN_SRC_PATH}/qscores-single ${CMAKE_BINARY_DIR}/qscores-single)
add_subdirectory_once (${MAIN_SRC_PATH}/qscores-settings ${CMAKE_BINARY_DIR}/qscores-settings)


########################################
##  Documentation

include (doxygen)


########################################
##  Show final message

include (final-msg)


########################################
##  Set various values based on libraries found

INCLUDE (post-compress)


########################################
##  Testing

enable_testing ()

