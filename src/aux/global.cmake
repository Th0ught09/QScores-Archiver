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


##  Software versions
SET (GLOBAL_VERSION_MAJOR 1)
SET (GLOBAL_VERSION_MINOR 00)

SET (QSCORES_VERSION_MAJOR 1)
SET (QSCORES_VERSION_MINOR 00)


##  Testing compilation flags, some of which are suggested by the Valgrind 3.3 book
# SET (MY_CXX_FLAGS "-pedantic -Wno-long-long -g -fno-inline -O0 -Wall")
##  Release compilation flags, suggested by the Valgrind 3.3 book
##  Added the following because of warnings in Boost 1.59.0:
##    -Wno-unused-variable
##    -Wno-unused-but-set-variable
SET (MY_CXX_FLAGS "-O3 -Wall -Wno-unused-variable -Wno-unused-but-set-variable")

