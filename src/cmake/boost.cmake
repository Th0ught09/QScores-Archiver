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


##  Suppress warning about new versions of Boost
set (Boost_NO_WARN_NEW_VERSIONS 1)

if (NOT Boost_FOUND)
  ##  Set the root path for Boost from the environment variable
  # set (Boost_DEBUG OFF)
  set (BOOST_ROOT $ENV{BOOST_ROOT})

  if (NOT BOOST_ROOT)
    message (FATAL_ERROR "EE\tThe environment variable BOOST_ROOT has to be set.")
  endif ()

  set (Boost_NO_SYSTEM_PATHS ON)
  set (Boost_USE_STATIC_LIBS   OFF)
  set (Boost_USE_MULTITHREADED ON)
  set (Boost_USE_STATIC_RUNTIME OFF)

  ##  Disable search for boost-cmake, from Boost 1.70.0
  ##    Helpful if there are conflicts between locally and system-installed versions
  set (Boost_NO_BOOST_CMAKE OFF)

  set (BOOST_COMPILED_LIBRARIES program_options system filesystem serialization mpi)
  find_package (Boost 1.79.0 REQUIRED COMPONENTS ${BOOST_COMPILED_LIBRARIES})
endif ()

if (Boost_FOUND)
  link_directories (${Boost_LIBRARY_DIRS})
  target_include_directories (${TARGET_NAME_EXEC} PUBLIC "${Boost_INCLUDE_DIRS}")
  target_link_libraries (${TARGET_NAME_EXEC} ${Boost_LIBRARIES})
endif ()

