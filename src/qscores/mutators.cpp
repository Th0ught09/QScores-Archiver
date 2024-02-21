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
    \file mutators.cpp
    Mutating member functions for QScores class definition .
*/
/*******************************************************************/

#include <fstream>
#include <string>
#include <climits>  //  UINT_MAX
#include <iostream>
#include <vector>

using namespace std;

#include "external-software.hpp"
#include "block-statistics.hpp"
#include "bitbuffer.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "qscores-settings.hpp"
#include "qscores-defn.hpp"
#include "qscores.hpp"


//  -----------------------------------------------------------------
//  General options
//  -----------------------------------------------------------------

/*!
     Indicate debugging output is required.
*/
void QScores::SetDebug () {
  m_Debug = true;
  return;
}


/*!
     Indicate verbose output is required.
*/
void QScores::SetVerbose () {
  m_Verbose = true;
  return;
}


/*!
     Indicate program is in encode mode.
*/
void QScores::SetEncode () {
  m_Encode = true;
  return;
}


/*!
     Indicate program is in decode mode.
*/
void QScores::SetDecode () {
  m_Decode = true;
  return;
}


/*!
     Indicate program should check external software.
*/
void QScores::SetExternalSoftwareCheck () {
  m_ExternalSoftwareCheck = true;
  return;
}


/*!
     Indicate program should print the binning information.
*/
void QScores::SetBinningCheck () {
  m_BinningCheck = true;
  return;
}


/*!
     Indicate program should print the unbinning information.
*/
void QScores::SetUnbinningCheck () {
  m_UnbinningCheck = true;
  return;
}


/*!
     Set the blocksize.

     \param[in] x Blocksize
*/
void QScores::SetBlocksize (int x) {
  m_Blocksize = x;
  return;
}


