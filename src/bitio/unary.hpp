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
    \file unary.hpp
    Header file for unary encoding and decoding functions
    Unary encoding codes a number x (x > 0) as "x - 1" 1 bits, followed
    by a single 0 bit.

    See Managing Gigabytes, second edition, pages 117-118 for further details.
*/
/*******************************************************************/

#ifndef UNARY_HPP
#define UNARY_HPP

const unsigned int ALL_1_EXCEPT_LAST = (UINT_MAX - 1);

//  Encoding functions
unsigned int Unary_Encode (BitBuffer &bitbuffer, unsigned int value);
unsigned int UnaryLow_Encode (BitBuffer &bitbuffer, unsigned int value, unsigned int low);

//  Decoding functions
unsigned int Unary_Decode (BitBuffer &bitbuffer);
unsigned int UnaryLow_Decode (BitBuffer &bitbuffer, unsigned int low_arg);

#endif

