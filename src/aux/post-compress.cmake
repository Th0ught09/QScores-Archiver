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


IF (ZLIB_FOUND)
  INCLUDE_DIRECTORIES (${ZLIB_INCLUDE_DIRS})
  TARGET_LINK_LIBRARIES (${TARGET_NAME_EXEC} ${ZLIB_LIBRARIES})
ENDIF (ZLIB_FOUND)

IF (BZIP2_FOUND)
  INCLUDE_DIRECTORIES (${BZIP2_INCLUDE_DIR})
  TARGET_LINK_LIBRARIES (${TARGET_NAME_EXEC} ${BZIP2_LIBRARIES})
  IF (NOT BZIP2_NEED_PREFIX)
    MESSAGE (WARNING "BZIP2 with functions beginning with BZ2_ is needed.  Please upgrade to version 1.0 if you wish to use it.")
  ENDIF (NOT BZIP2_NEED_PREFIX)
ENDIF (BZIP2_FOUND)

