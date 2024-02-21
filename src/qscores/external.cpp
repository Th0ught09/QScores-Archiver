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
    \file external.cpp
    Use of external compression programs.
*/
/*******************************************************************/

#include <string>
#include <vector>
#include <fstream>
#include <iomanip>  //  setw
#include <iostream>
#include <climits>  //  UINT_MAX

using namespace std;

//  Pull the configuration file in
#include "QScoresConfig.hpp"

#if ZLIB_FOUND
#include "zlib.h"
#endif

#if BZIP2_FOUND
#include "bzlib.h"
#endif

#include "external-software.hpp"
#include "block-statistics.hpp"
#include "bitbuffer.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "qscores-settings.hpp"
#include "qscores-local.hpp"
#include "qscores.hpp"


/*!
     Perform a "check" of external software -- show if they have been located successfully.
*/
void QScores::PerformExternalSoftwareCheck () {
  cerr << m_ExternalSoftware << endl;

  return;
}

