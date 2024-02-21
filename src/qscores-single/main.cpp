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
    Test driver for QScoresSingle class.
*/
/*******************************************************************/

#include <cstdlib>  //  EXIT_SUCCESS, EXIT_FAILURE, exit ()
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
#include <climits>  //  UINT_MAX

using namespace std;

#include "bitbuffer.hpp"
#include "QScoresSingleConfig.hpp"
#include "qscores-single-defn.hpp"
#include "testing.hpp"
#include "qscores-single.hpp"


/*!
     Main driver

     \param[in] argc Number of arguments
     \param[in] argv Arguments to program
     \return Returns 0 on success, 1 otherwise.
*/
int main(int argc, char **argv) {
  e_QSCORES_MAP mode = e_QSCORES_MAP_SANGER;
  QScoresSingle qscores_single ("II$IIII\"IIIIIIIIIIIIII=IIIIIIIII&%IIIIIIIIIIIIII", true);
  vector<unsigned int> lookup (0xFF);
  unsigned int offset = 0;

  switch (mode) {
    case e_QSCORES_MAP_SANGER : 
      offset = g_SANGER_OFFSET;
      break;
    case e_QSCORES_MAP_SOLEXA :
      offset = g_SANGER_OFFSET;
      break;
    case e_QSCORES_MAP_ILLUMINA :
      offset = g_SANGER_OFFSET;
      break;
    default :
      cerr << "EE\tInvalid mapping mode in main ()." << endl;
      exit (EXIT_FAILURE);
  }
  
  //  Need at least one argument
  if (argc < 2) {
    return (EXIT_FAILURE);
  }

  if (strcmp (argv[1], "1") == 0) {
    cout << "QScoresSingle version " << QScoresSingle_VERSION_MAJOR << "." << QScoresSingle_VERSION_MINOR << ":  " << __DATE__ <<  " (" << __TIME__ << ")" << endl;
    return (EXIT_SUCCESS);
  }
  else if (strcmp (argv[1], "2") == 0) {
    cerr << qscores_single << endl;
    return (EXIT_SUCCESS);
  }
  else if (strcmp (argv[1], "3") == 0) {
    unsigned int buffer_size = 100;
    char* buffer = (char*) calloc (buffer_size, sizeof (char));

    cerr << qscores_single << endl;
    cerr << "\n===================================" << endl;

    qscores_single.PrintQScore ();
    cerr << "\n===================================" << endl;

    qscores_single.QScoreToInt ();
    qscores_single.PrintQScore ();
    cerr << "\n===================================" << endl;

    unsigned int len = qscores_single.GetQScoreIntAsBinary (buffer, buffer_size);
    cerr << "[";
    for (unsigned int i = 0; i < len; i++) {
      fprintf (stderr, "%c", buffer[i]);
    }
    cerr << "]" << endl;
    
    free (buffer);
    return (EXIT_SUCCESS);
  }
  
  
  //  Beyond here, we expect at least two arguments
  if (argc < 3) {
    return (EXIT_FAILURE);
  }
  string test (argv[2]);  //  Get the test string
  
  if (strcmp (argv[1], "4") == 0) {
    TestRescaling (test);
    return (EXIT_SUCCESS);
  }
  else if (strcmp (argv[1], "5") == 0) {
    TestRemapping (test);
    return (EXIT_SUCCESS);
  }
  else if (strcmp (argv[1], "6") == 0) {
    TestDifferenceCoding (test);
    return (EXIT_SUCCESS);
  }
  
  
  //  Beyond here, we expect at least three arguments
  if (argc < 4) {
    return (EXIT_FAILURE);
  }
  unsigned int param = atoi (argv[3]);  //  Get the lossy parameter

  if (strcmp (argv[1], "7") == 0) {
    TestQSBinning (test, mode, param);
  }
  else if (strcmp (argv[1], "8") == 0) {
    TestEPBinning (test, mode, param);
  }
  else if (strcmp (argv[1], "9") == 0) {
    TestQSBinningDifferenceCoding (test, mode, param);
  }

  return (EXIT_SUCCESS);
}


