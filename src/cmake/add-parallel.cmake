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


if (OPENMP_FOUND)
  if (OpenMP_CXX_FLAGS)
    set_target_properties (${TARGET_NAME} PROPERTIES LINK_FLAGS "${OpenMP_CXX_FLAGS}")
  endif ()
endif ()

if (MPI_CXX_FOUND)
  include_directories (${MPI_CXX_INCLUDE_PATH})

  target_link_libraries (${TARGET_NAME} PRIVATE ${MPI_CXX_LIBRARIES})

  if (MPI_CXX_COMPILE_FLAGS)
    set_target_properties (${TARGET_NAME} PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
  endif ()

  if (MPI_CXX_LINK_FLAGS)
    set_target_properties (${TARGET_NAME} PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
  endif ()
endif ()


