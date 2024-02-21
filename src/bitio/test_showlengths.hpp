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
    \file test_sequential.cpp
    Header file for test of BitIO to show bit lengths.
*/
/*******************************************************************/

#ifndef TESTING_SHOWLENGTHS_HPP
#define TESTING_SHOWLENGTHS_HPP

unsigned int TestUnaryShowLengths (unsigned int showlengths);
unsigned int TestBinaryShowLengths (unsigned int showlengths);
unsigned int TestGammaShowLengths (unsigned int showlengths);
unsigned int TestDeltaShowLengths (unsigned int showlengths);
unsigned int TestGolombShowLengths (unsigned int b, unsigned int showlengths);
unsigned int TestRiceShowLengths (unsigned int k, unsigned int showlengths);

#endif
