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
    \file math_utils.hpp
    Header file for miscellaneous mathematical utilities.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
    $Id: math_utils.hpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP


/*!
     Definition of 0 as an unsigned long long int
*/
const unsigned long long int g_ZERO_ULL = 0ull;


/*!
     Definition of 1 as an unsigned long long int
*/
const unsigned long long int g_ONE_ULL = 1ull;


/*!
     Mask the highest bit of an unsigned int (i.e., 2147483648)
*/
const unsigned int g_MASK_HIGHEST_BIT = 0x80000000;

unsigned int FloorLog (unsigned int value);
unsigned int CeilLog (unsigned int x);
unsigned int CeilLogULL (unsigned long long int x);

#endif
