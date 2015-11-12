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
    \file qscores-single-defn.hpp
    Global definitions not part of any class.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: qscores-single-defn.hpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#ifndef QSCORES_SINGLE_DEFN_HPP
#define QSCORES_SINGLE_DEFN_HPP

/*!
     \enum e_QSCORES_MAP
     QScores mapping standard used.
*/
enum e_QSCORES_MAP {
  e_QSCORES_MAP_UNSET,  /*!< QScores standard not yet decided */
  e_QSCORES_MAP_SANGER,  /*!< Sanger encoding */
  e_QSCORES_MAP_SOLEXA,  /*!< Solexa encoding */
  e_QSCORES_MAP_ILLUMINA,  /*!< Illumina encoding */
  e_QSCORES_MAP_LAST /*!< Last QScores standard  */
};


//!  Global offset that defines the lowest value possible when stored internally as integers
const unsigned int g_GLOBAL_OFFSET = 2;


//!  Sanger offset
const unsigned int g_SANGER_OFFSET = 33;


//!  Sanger minimum
const unsigned int g_SANGER_MIN = 33;


//!  Sanger maximum
const unsigned int g_SANGER_MAX = 126;


//!  Solexa offset -- should be 64, but make it 59 so that the lowest value is 0 and not -5
const unsigned int g_SOLEXA_OFFSET = 59;


//!  Solexa minimum
const unsigned int g_SOLEXA_MIN = 59;


//!  Solexa maximum
const unsigned int g_SOLEXA_MAX = 126;


//!  Illumina offset
const unsigned int g_ILLUMINA_OFFSET = 64;


//!  Illumina minimum
const unsigned int g_ILLUMINA_MIN = 64;


//!  Illumina maximum
const unsigned int g_ILLUMINA_MAX = 126;


#endif
