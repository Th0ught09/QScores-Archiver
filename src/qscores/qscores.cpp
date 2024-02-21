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
    \file qscores.cpp
    Constructor and destructor for QScores class definition .
*/
/*******************************************************************/

#include <vector>
#include <string>
#include <fstream>
#include <climits>  //  UINT_MAX
#include <iostream>
#include <cstdlib>

using namespace std;

#include "external-software.hpp"
#include "block-statistics.hpp"
#include "bitbuffer.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "qscores-settings.hpp"
#include "qscores-defn.hpp"
#include "qscores.hpp"

//!  Constructor that takes no arguments
QScores::QScores()
  : m_Debug (false),
    m_Verbose (false),
    m_Encode (false),
    m_Decode (false),
    m_ExternalSoftwareCheck (false),
    m_BinningCheck (false),
    m_UnbinningCheck (false),
    m_BitBuff_In (),
    m_BitBuff_Out (),
    m_Text_In (),
    m_Text_Out (),
    m_QScoresSettings (),
    m_ExternalSoftware (),
    m_Qscores (0),
    m_FileReadLength (0),
    m_FileBlockSize (0),
    m_BlockReadLength (0),
    m_Blocksize (INT_MAX),
    m_BlockMinimum (0),
    m_BlockStatistics (),
    m_CompressionParameter (UINT_MAX),
    m_CompressionParameter_2 (UINT_MAX)
{
}


//!  Destructor that takes no arguments
QScores::~QScores() {
}


/*!
     Initialization function for anything that the constructor could not do because it didn't have enough information at the time.

     \return Returns true on success, false on failure.
*/
bool QScores::Initialize () {
  bool encode = true;
  
  if (GetDecode ()) {
    encode = false;
  }

  m_ExternalSoftware.InitializePaths ();

  if (m_QScoresSettings.GetCompressionGzip ()) {
    m_ExternalSoftware.Initialize (e_EXTERNAL_METHOD_GZIP_ZLIB, encode);
  }
  else if (m_QScoresSettings.GetCompressionBzip ()) {
    m_ExternalSoftware.Initialize (e_EXTERNAL_METHOD_BZIP_BZLIB, encode);
  }
  
  return true;
}


