###########################################################################
##  Copyright 2011-2015, 2024-2025 by Raymond Wan (rwan.work@gmail.com)
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


########################################
##  Set the C++ standard

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

##  Use target_compile_features () for macOS
if (TARGET ${TARGET_NAME_LIB})
  target_compile_features (${TARGET_NAME_LIB} PUBLIC cxx_std_20)
endif ()

if (TARGET ${TARGET_NAME_EXEC})
  target_compile_features (${TARGET_NAME_EXEC} PUBLIC cxx_std_20)
endif ()


########################################
##  Additional flags

##  Testing compilation flags, some of which are suggested by the Valgrind 3.3 book
##  set (MY_CXX_FLAGS "-pedantic -Wno-long-long -g -fno-inline -O0 -Wall")

##  Release compilation flags, suggested by the Valgrind 3.3 book
##  Added the following because of warnings in Boost 1.59.0:
##    -Wno-unused-variable
##    -Wno-unused-but-set-variable
set (MY_CXX_FLAGS "-O3 -Wall -Wno-unused-variable -Wno-unused-but-set-variable")

##  Set compiler flags based on global variable (if there are any)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_CXX_FLAGS}")
