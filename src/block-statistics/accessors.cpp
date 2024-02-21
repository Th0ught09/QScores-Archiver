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
    \file accessors.cpp
    Accessors for BlockStatistics class definition .
*/
/*******************************************************************/

#include <fstream>  //  ostream
#include <vector>
#include <climits>  //  UINT_MAX
#include <algorithm>  //  sort
#include <iostream>

using namespace std;

#include "common.hpp"
#include "block-statistics.hpp"


//  -----------------------------------------------------------------
//  Accessors
//  -----------------------------------------------------------------

/*!
     Return m_Debug

     \return Debug mode or not
*/
bool BlockStatistics::GetDebug () const {
  return m_Debug;
}


/*!
     Return a quality score in the frequency table

     \return Quality score
*/
unsigned int BlockStatistics::GetFrequencyTableQScore (unsigned int pos) const {
  return m_FrequencyTable[pos].qscore;
}


/*!
     Return a frequency in the frequency table

     \return Frequency
*/
unsigned int BlockStatistics::GetFrequencyTableFreq (unsigned int pos) const {
  return m_FrequencyTable[pos].freq;
}


/*!
     Return an ID in the frequency table

     \return ID
*/
unsigned int BlockStatistics::GetFrequencyTableID (unsigned int pos) const {
  return m_FrequencyTable[pos].id;
}


/*!
     Return the size of the lookup table

     \return Size of the lookup tables
*/
unsigned int BlockStatistics::GetIDsToQScoresSize () const {
  return m_IDsToQScoresSize;
}


/*!
     Return the remapping, represented as a vector of unsigned integers.  This remapping is
     the new IDs, sorted by quality score.

     \return Vector indicating remapping
*/
vector<unsigned int> BlockStatistics::GetQScoresToIDs () {
  vector<unsigned int> result;

  //  Ensure that we've sorted already
  if (!m_IsSorted) {
    ApplySort ();
  }

  return m_QScoresToIDs;
}


/*!
     Return the mapping of new IDs to quality scores.

     \return Vector indicating remapping
*/
vector<unsigned int> BlockStatistics::GetIDsToQScores () {
  return m_IDsToQScores;
}

