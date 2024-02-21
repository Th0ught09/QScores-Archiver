//  ###########################################################################
//  Copyright 2011-2015, 2024 by Raymond Wan (rwan.work@gmail.com)
//    https://github.com/rwanwork/QScores-Archiver
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
//  ###########################################################################


/*******************************************************************/
/*!
    \file math_utils.hpp
    Header file for miscellaneous mathematical utilities.
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
