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


############################################################
##  CMake file defining add_subdirectory_once
##
##  Raymond Wan
##  Organizations
##    - Department of Computational Biology, Graduate School of
##      Frontier Science, University of Tokyo
##    - Computational Biology Research Center, AIST, Japan
##
##  $Id: add_subdirectory_once.cmake 217 2011-08-23 05:25:48Z rwan $
############################################################


##  Source code for this function from 
##    Rolf Eike Beer on the CMake mailing list
##    (2010/12/02)
function (Add_Subdirectory_Once SUBDIRECTORY)
  get_filename_component (FULLPATH ${SUBDIRECTORY} REALPATH)

  get_property (_INCLUDED_DIRS GLOBAL PROPERTY ADD_SUBDIRECTORY_ONCE_INCLUDED)
  list (FIND _INCLUDED_DIRS "${FULLPATH}" _USED_INDEX)

  if (_USED_INDEX EQUAL -1)
    set_property (GLOBAL PROPERTY ADD_SUBDIRECTORY_ONCE_INCLUDED "${_INCLUDED_DIRS}" "${FULLPATH}")
    if(${ARGC} EQUAL 1)
      add_subdirectory (${SUBDIRECTORY})
    else(${ARGC} EQUAL 1)
      add_subdirectory (${SUBDIRECTORY} ${ARGV1})
    endif (${ARGC} EQUAL 1)
  endif (_USED_INDEX EQUAL -1)
endfunction (Add_Subdirectory_Once)



