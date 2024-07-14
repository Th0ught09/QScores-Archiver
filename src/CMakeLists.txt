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
##  Define the project name and C++ standard

set (CURR_PROJECT_NAME "QScores-Archiver")

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)


########################################
##  Set the global path

##  If the MAIN_SRC_PATH has not been defined yet
if (NOT DEFINED MAIN_SRC_PATH)
  ##  Set the main source path to the very top
  set (MAIN_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/")

  ##  Locate where the shared CMake modules are
  list (APPEND CMAKE_MODULE_PATH "${MAIN_SRC_PATH}/cmake")
endif ()

##  Include externally defined modules
include (add_subdirectory_once)


########################################
##  Set up the software
include (initial-msg)

project (${CURR_PROJECT_NAME} VERSION 1.0 DESCRIPTION "QScores-Archiver" LANGUAGES CXX)
message (STATUS "Setting up ${CURR_PROJECT_NAME}...")


########################################
##  Add subdirectories to process

add_subdirectory_once (qscores)


########################################
##  Show final message
include (final-msg)


########################################
##  CTest

enable_testing ()


