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
    \file testing.cpp
    Header file for testing functions.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
    $Id: testing.hpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#ifndef TESTING_HPP
#define TESTING_HPP

//!  The number of test values to generate
const int g_TEST_SIZE = 1000000;

//!  The number of test values to generate
const int g_TEST_SIZE_POW2 = 30;

//!  The maximum test value.
const int g_TEST_RANGE = (1 << 15);

int BitLength (int num);
int ShowInfo ();
int GeneratePowerTwoLimit ();
int GeneratePowerTwo ();
int GenerateFixed ();
int GenerateVariable ();
int TestUnsignedInts ();
int TestUnsignedChars ();

#endif

