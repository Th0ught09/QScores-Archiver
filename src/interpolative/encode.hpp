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
    \file encode.hpp
    Header file for encoding functions for interpolative coding
*/
/*******************************************************************/


#ifndef ENCODE_HPP
#define ENCODE_HPP

//  Functions for unsigned ints
void Interpolative_Encode (BitBuffer &bitbuffer, const vector <unsigned int>& v);
void Recursive_Interpolative_Encode (BitBuffer &bitbuffer, const vector <unsigned int>& list, unsigned int list_size, unsigned int left, unsigned int right, unsigned int left_value, unsigned int right_value);

#endif
