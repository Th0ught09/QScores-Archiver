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
    \file decode.hpp
    Header file for decoding functions for interpolative coding
*/
/*******************************************************************/


#ifndef DECODE_HPP
#define DECODE_HPP

//  Functions for unsigned ints
void Interpolative_Decode (BitBuffer &bitbuffer, vector <unsigned int> &v, unsigned int v_size);
void Recursive_Interpolative_Decode (BitBuffer& bitbuffer, std::vector< unsigned int, std::allocator< unsigned int > > &list, unsigned int list_size, unsigned int left, unsigned int right, unsigned int left_value, unsigned int right_value);

//  Functions for unsigned long long ints

#endif
