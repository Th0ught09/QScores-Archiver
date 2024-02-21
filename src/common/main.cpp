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


#include <iostream>
#include <cstdlib>  //  EXIT_SUCCESS, EXIT_FAILURE

#include "common.hpp"

using namespace std;


/*!
     Main driver that checks the architecture of the machine

     \param[in] argc Number of arguments
     \param[in] argv Arguments to program
     \return Returns 0 on success, 1 otherwise.
*/
int main(int argc, char **argv) {
  bool result = true;

  //  Perform two checks -- basically unnecessary since either both are true or both are false...
  if (sizeof (unsigned int) != g_UINT_SIZE_BYTES) {
    cerr << "WW\tMismatch in size of an unsigned integer:  " << sizeof (unsigned int) << " : " << g_UINT_SIZE_BYTES << endl;
    result = false;
  }

  if (sizeof (unsigned int) * g_CHAR_SIZE_BITS != g_UINT_SIZE_BITS) {
    cerr << "WW\tMismatch in number if bits per unsigned integer:  " << sizeof (unsigned int) * g_CHAR_SIZE_BITS << " : " << g_UINT_SIZE_BITS << endl;
    result = false;
  }

  if (result) {
    cerr << "II\tComputer architecture passes all tests!" << endl;
    return (EXIT_SUCCESS);
  }

  cerr << "EE\tComputer architecture has failed some tests!" << endl;
  return (EXIT_FAILURE);
}

