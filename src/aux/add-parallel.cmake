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


IF (OPENMP_FOUND)
  IF (OpenMP_CXX_FLAGS)
    SET_TARGET_PROPERTIES (${TARGET_NAME} PROPERTIES LINK_FLAGS "${OpenMP_CXX_FLAGS}")
  ENDIF ()
ENDIF ()

IF (MPI_CXX_FOUND)
  INCLUDE_DIRECTORIES (${MPI_CXX_INCLUDE_PATH})

  TARGET_LINK_LIBRARIES (${TARGET_NAME} ${MPI_CXX_LIBRARIES})

  IF (MPI_CXX_COMPILE_FLAGS)
    SET_TARGET_PROPERTIES (${TARGET_NAME} PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
  ENDIF ()

  IF (MPI_CXX_LINK_FLAGS)
    SET_TARGET_PROPERTIES (${TARGET_NAME} PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
  ENDIF ()
ENDIF ()


