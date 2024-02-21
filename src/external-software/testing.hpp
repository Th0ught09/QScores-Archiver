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
    \file testing.cpp
    Functions used for testing.
*/
/*******************************************************************/

#ifndef EXTERNAL_SOFTWARE_TESTING_HPP
#define EXTERNAL_SOFTWARE_TESTING_HPP

//!  Size of a block to read from/write to
const unsigned int g_BLOCK_SIZE = 1024;

void DisplayChar (char *str, unsigned int len);
bool CompareChar (char *x, char *y, unsigned int len);
char* ReadFile (string fn, unsigned int& size);


#endif

