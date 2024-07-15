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
    Test driver for QScoresSettings class.
*/
/*******************************************************************/

#include <cstdlib>  //  EXIT_SUCCESS, EXIT_FAILURE
#include <string>
#include <vector>
#include <iostream>
#include <cstring>  //  strcmp
#include <fstream>
#include <climits>

using namespace std;

#include "bitbuffer.hpp"
#include "bitio-defn.hpp"
#include "qscores-single-defn.hpp"
#include "QScoresSettings_Config.hpp"
#include "qscores-settings.hpp"


/*!
     Main driver

     \param[in] argc Number of arguments
     \param[in] argv Arguments to program
     \return Returns 0 on success, 1 otherwise.
*/
int main(int argc, char **argv) {
  QScoresSettings qscores_settings;

  if (argc != 2) {
    return (EXIT_FAILURE);
  }

  if (strcmp (argv[1], "1") == 0) {
    cout << qscores_settings << endl;

    cout << "QScoresSettings coding version " << PROGRAM_VERSION << " compiled on:   " << __DATE__ <<  " (" << __TIME__ << ")" << endl;
    cout << "Git hash:  " << GIT_HASH << endl;
  }
  else if (strcmp (argv[1], "2") == 0) {
    bool result = qscores_settings.SetQScoresMapping ("sanger");
    if (!result) {
      cerr << "EE\tError while setting the mapping method." << endl;
      return (EXIT_FAILURE);
    }
    
    //  Too many compression methods set here, but it's a test anyway.
    //  qscores_settings.CheckSettings () would fail here.
    qscores_settings.SetCompressionGamma ();
    qscores_settings.SetCompressionDelta ();
    qscores_settings.SetCompressionInterP ();
    qscores_settings.SetCompressionGolomb ();
    qscores_settings.SetCompressionRice ();

    cout << qscores_settings << endl;

    cout << "QScoresSettings coding version " << PROGRAM_VERSION << " compiled on:   " << __DATE__ <<  " (" << __TIME__ << ")" << endl;
    cout << "Git hash:  " << GIT_HASH << endl;
  }

  return (EXIT_SUCCESS);
}

