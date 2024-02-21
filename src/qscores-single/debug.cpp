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
    \file debug.cpp
    Debugging functions for QScoresSingle class.
*/
/*******************************************************************/

#include <string>
#include <vector>
#include <ostream>
#include <iostream>
#include <fstream>

using namespace std;

#include "bitbuffer.hpp"
#include "qscores-mapping.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "debug.hpp"


/*!
     Make a backup of the private member variables.
*/
void QScoresSingle::Backup () {
  m_QScoreStr_Old = m_QScoreStr;
  m_QScoreInt_Old = m_QScoreInt;

  return;
}


/*!
     Compare the private member variables and print error messages if they do not match.
*/
void QScoresSingle::CompareBackup () {
  if (m_QScoreStr_Old.length () != m_QScoreStr.length ()) {
    cerr << "EE\tThe string representations do not match in length!" << endl;
  }
  else if (m_QScoreStr_Old != m_QScoreStr) {
    cerr << "EE\tThe strings do not match!" << endl;
    cerr << "Old:  [" << m_QScoreStr_Old << "]" << endl;
    cerr << "New:  [" << m_QScoreStr << "]" << endl;
  }

  if (m_QScoreInt_Old.size () != m_QScoreInt.size ()) {
    cerr << "EE\tThe integer representations do not match in length!" << endl;
  }
  else {
    for (unsigned int i = 0; i < m_QScoreInt_Old.size (); i++) {
      if (m_QScoreInt_Old[i] != m_QScoreInt[i]) {
        cerr << "EE\tMismatch in position " << i << ":  " << m_QScoreInt_Old[i] << " != " << m_QScoreInt[i] << endl;
      }
    }
  }

  return;
}
