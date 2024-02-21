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

#include <vector>
#include <string>
#include <iterator>
#include <iostream>
#include <string.h>
#include <cstdlib>
#include <cstdio>  //  fopen
#include <climits>
#include <fstream>

#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations

using namespace std;
namespace bfs = boost::filesystem;

#include "external-software-local.hpp"
#include "external-software-exception.hpp"
#include "external-software.hpp"


//  -----------------------------------------------------------------
//  Public functions
//  -----------------------------------------------------------------


/*!
     Display a character array (i.e., without a \0 character terminator).

     \param[in] str The string itself
     \param[in] len Length of the string
*/
void DisplayChar (char *str, unsigned int len) {
  for (unsigned int i = 0; i < len; i++) {
    printf ("%c", str[i]);
  }
  printf ("\n");
  
  return;
}


/*!
     Compare two character arrays, neither of which are terminated with a \0 character.

     \param[in] x The first string
     \param[in] y The second string
     \param[in] len Length of the two strings; characters beyond this value are not checked!
*/
bool CompareChar (char *x, char *y, unsigned int len) {
  for (unsigned int i = 0; i < len; i++) {
    if (x[i] != y[i]) {
      return false;
    }
  }
  
  return true;
}


char* ReadFile (string fn, unsigned int& size) {
  FILE *fp = NULL;
  char* str = NULL;
  
  fp = fopen (fn.c_str (), "rb");
  if (!fp) {
    cerr << "EE\tCould not read test file " << fn << "!" << endl;
    return NULL;
  }
  
  //  Determine the size of the file
  bfs::path path (fn);
  size = file_size (path);

  str = (char*) calloc (size, sizeof (char));
  unsigned int bytes_read = fread (str, sizeof (char), size, fp);
  if (bytes_read != size) {
    cerr << "EE\tUnable to read from the test file " << fn << "!" << endl;
    free (str);
    return NULL;
  }
  fclose (fp);
  
  return (str);
}

