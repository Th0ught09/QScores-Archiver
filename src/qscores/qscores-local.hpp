///////////////////////////////////////////////////////////////////////////
//  QScores-Archiver
//  Software and library for transforming and compressing next generation
//    sequencing data in FASTQ format.
//  
//  Version N/A (On Github) -- November 12, 2015
//  
//  Copyright (C) 2011-2015 by Raymond Wan, All rights reserved.
//  Contact:  rwan.work@gmail.com
//  Organization:  Division of Life Science, Faculty of Science, Hong Kong
//                 University of Science and Technology, Hong Kong
//  
//  This file is part of QScores-Archiver.
//  
//  QScores-Archiver is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public License 
//  as published by the Free Software Foundation; either version 
//  3 of the License, or (at your option) any later version.
//  
//  QScores-Archiver is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public 
//  License along with QScores-Archiver; if not, see 
//  <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////


/*******************************************************************/
/*!
    \file qscores-local.hpp
    Global definitions not part of this class and should not be included by any other class.

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan

    $Id: qscores-local.hpp 224 2011-11-06 15:32:09Z rwan $
*/
/*******************************************************************/

#ifndef QSCORES_LOCAL_HPP
#define QSCORES_LOCAL_HPP

//!  Name of configuration file
#define g_CFG_FILENAME "qscores.cfg"

//!  How often block status is printed out (in terms of number of blocks processed); useful only with the --debug option
const unsigned int g_BLOCK_STATUS_FREQUENCY = 100000;

//!  The size of the data buffer to/from the ExternalSoftware class
const unsigned int g_EXTERNAL_BUFFER_SIZE = 1048576;

//!  Special value indicating that the read length varies
const unsigned int g_READ_LENGTH_VARIABLE = UINT_MAX;

#endif

