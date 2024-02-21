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
    \file math_utils.cpp
    Miscellaneous mathematical utilities.
*/
/*******************************************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>  //  exit
#include <cmath>

using namespace std;

#include "math_utils.hpp"

/*!
     Calculate the floor of the binary log of a value [lb (x)].

     \param[in] value Value to calculate the lb () of
     \return floor (lb (x))
*/
unsigned int FloorLog (unsigned int value) {
  unsigned int x = value;
  unsigned int y = 0;

  while (x != 0) {
    x = x >> 1;
    y++;
  }

  if (y == 0) {
    cerr << "EE\tCannot calculate log2 (0) in " << __FILE__ << "::FloorLog!" << endl;
    exit (EXIT_FAILURE);
  }

  return (y - 1);
}


/*!
     Calculate the ceiling of the binary log of a value [i.e., lb (x)]

     \param[in] x Value to calculate the lb () of
     \return ceil (lb (x))
*/
unsigned int CeilLog (unsigned int x) {
  unsigned int y = 0;

  if (x == 0) {
    cerr << "EE\tCannot calculate log2 (0) in " << __FILE__ << "::CeilLog!" << endl;
    exit (EXIT_FAILURE);
  }

  x--;
  while (x != 0) {
    x >>= 1;
    y++;
  }

  return (y);
}


/*!
     Calculate the ceiling of the binary log of a value [i.e., lb (x)], where x is an unsigned long long int.

     \param[in] x Value to calculate the lb () of
     \return ceil (lb (x))
*/
unsigned int CeilLogULL (unsigned long long int x) {
  unsigned int y = 0;

  if (x == 0) {
    cerr << "EE\tCannot calculate log2 (0) in " << __FILE__ << "::CeilLogULL!" << endl;
    exit (EXIT_FAILURE);
  }

  x--;
  while (x != 0) {
    x >>= g_ONE_ULL;
    y++;
  }

  return (y);
}

