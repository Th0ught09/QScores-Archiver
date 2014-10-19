###########################################################################
##  QScores-Archiver
##  Software and library for transforming and compressing next generation
##    sequencing data in FASTQ format.
##  
##  Version 0.99 -- August 22, 2011
##  Version 1.00 -- November 6, 2011
##  
##  Copyright (C) 2011 by Raymond Wan, All rights reserved.
##  Contact:  r-wan@cb.k.u-tokyo.ac.jp
##  Organization:  Department of Computational Biology, Graduate School of
##                 Frontier Science, University of Tokyo and
##                 Computational Biology Research Center, AIST,
##                 Japan
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
##  CMake file defining global variables
##
##  Raymond Wan
##  Organizations
##    - Department of Computational Biology, Graduate School of
##      Frontier Science, University of Tokyo
##    - Computational Biology Research Center, AIST, Japan
##
##  $Id: global.cmake 224 2011-11-06 15:32:09Z rwan $
############################################################

##  Software versions
SET (GLOBAL_VERSION_MAJOR 1)
SET (GLOBAL_VERSION_MINOR 00)

SET (QSCORES_VERSION_MAJOR 1)
SET (QSCORES_VERSION_MINOR 00)


##  Testing compilation flags, some of which are suggested by the Valgrind 3.3 book
# SET (MY_CXX_FLAGS "-pedantic -Wno-long-long -g -fno-inline -O0 -Wall")
##  Release compilation flags, suggested by the Valgrind 3.3 book
SET (MY_CXX_FLAGS "-O3 -Wall")

