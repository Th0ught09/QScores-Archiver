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


if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
  message ("############################################################")
  message ("##  Configuring ${CMAKE_PROJECT_NAME} with CMake...")
  message ("############################################################")
  message ("Options available:\n")
  message ("* -DCMAKE_INSTALL_PREFIX=<top-path>")
  message ("    Changes the path where software is installed.")
  message ("* -DCMAKE_VERBOSE_MAKEFILE=ON")
  message ("    Make is more verbose.")
  message ("* -DBUILD_DOCS=ON")
  message ("    Build documentation.\n")
  message ("For example:")
  message ("  cmake -DCMAKE_INSTALL_PREFIX=~/tmp/qscores ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt\n")
  message ("See the man pages for cmake for more information!")
  message ("############################################################")
endif ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")

