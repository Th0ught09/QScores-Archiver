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
    Header file for test of sequential sequence of numbers for BitIO.
*/
/*******************************************************************/

#ifndef TESTING_SEQUENTIAL_HPP
#define TESTING_SEQUENTIAL_HPP

//!  The number of test values to generate for the sequential tests
const unsigned int g_TEST_SIZE_SEQUENTIAL = 10;

int TestUnarySequential ();
int TestBinarySequential ();
int TestGammaSequential ();
int TestDeltaSequential ();
int TestGolombSequential (unsigned int b);
int TestRiceSequential (unsigned int k);

#endif
