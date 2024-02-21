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
    \file delta.hpp
    Header file for delta encoding and decoding functions
    Delta encoding uses [gamma, binary], where the number x is greater than 0.

    See Managing Gigabytes, second edition, pages 117-118 for further details.
*/
/*******************************************************************/


#ifndef DELTA_HPP
#define DELTA_HPP

//  Encoding functions
unsigned int Delta_Encode (BitBuffer &bitbuffer, unsigned int value);
unsigned int DeltaLow_Encode (BitBuffer &bitbuffer, unsigned int value, unsigned int low);

//  Decoding functions
unsigned int Delta_Decode (BitBuffer &bitbuffer);
unsigned int DeltaLow_Decode (BitBuffer &bitbuffer, unsigned int low);

#endif

