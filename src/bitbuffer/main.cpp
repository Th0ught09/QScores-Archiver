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
    \file main.cpp
    Test driver for BitBuffer class.
*/
/*******************************************************************/

#include <iostream>
#include <fstream>

#include <cstdlib>  //  EXIT_SUCCESS, EXIT_FAILURE
#include <cstring>  //  strcmp

using namespace std;

#include "bitbuffer.hpp"
#include "testing.hpp"

/*!
     Main driver

     \param[in] argc Number of arguments
     \param[in] argv Arguments to program
     \return Returns 0 on success, 1 otherwise.
*/
int main(int argc, char **argv) {
  int result = 0;

  if (argc != 2) {
    cerr << "==\tError:  One [numeric] argument required!" << endl;
    return (EXIT_FAILURE);
  }

  if (strcmp (argv[1], "1") == 0) {
    result = ShowInfo ();
  }
  else if (strcmp (argv[1], "2") == 0) {
    result = GeneratePowerTwoLimit ();
  }
  else if (strcmp (argv[1], "3") == 0) {
    result = GeneratePowerTwo ();
  }
  else if (strcmp (argv[1], "4") == 0) {
    result = GenerateFixed ();
  }
  else if (strcmp (argv[1], "5") == 0) {
    result = GenerateVariable ();
  }
  else if (strcmp (argv[1], "6") == 0) {
    result = TestUnsignedInts ();
  }
  else if (strcmp (argv[1], "7") == 0) {
    result = TestUnsignedChars ();
  }
  else {
    cerr << "==\tError:  Test case unknown!" << endl;
    return (EXIT_FAILURE);
  }

  return (result);
}


