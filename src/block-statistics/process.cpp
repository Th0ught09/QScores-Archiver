///////////////////////////////////////////////////////////////////////////
//  QScores-Archiver
//  Software and library for transforming and compressing next generation
//    sequencing data in FASTQ format.
//  
//  Version N/A (On Github) -- November 12, 2015
//  
//  Copyright (C) 2011-2015 by Raymond Wan, All rights reserved.
//  Contact:  rwan.work@gmail.com
//  Organization:  Division of Life Science, Faculty of Science, Hong Kong
//                 University of Science and Technology, Hong Kong
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
///////////////////////////////////////////////////////////////////////////


/*******************************************************************/
/*!
    \file process.cpp
    Processing functions for BlockStatistics class definition .

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: process.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <fstream>  //  ostream
#include <vector>
#include <climits>  //  UINT_MAX
#include <algorithm>  //  sort
#include <iostream>
#include <cassert>

using namespace std;

#include "common.hpp"
#include "block-statistics.hpp"


//  -----------------------------------------------------------------
//  Friends and other non-class functions
//  -----------------------------------------------------------------


/*!
     Overloaded << operator defined as a friend of BlockStatistics for debugging purposes.

     \param[in] os Output stream
     \param[in] qs BlockStatistics passed as reference
     \return Output stream
*/
ostream &operator<< (ostream &os, const BlockStatistics& bs) {
  for (unsigned int i = 0; i < bs.m_FrequencyTable.size (); i++) {
    if (bs.GetFrequencyTableFreq (i) != 0) {
      os << bs.GetFrequencyTableQScore (i) << "\t" << bs.GetFrequencyTableFreq (i) << "\t" << bs.GetFrequencyTableID (i) << endl;
    }
  }

  return os;
}


/*!
     Non-class comparison function used to sort table by frequency

     \param[in] lhs The left-hand entry
     \param[in] rhs The right-hand entry     
*/
bool SortByFrequency (FrequencyEntry const& lhs, FrequencyEntry const& rhs) {
  if (lhs.freq != rhs.freq) {
    return lhs.freq > rhs.freq;
  }
  return lhs.qscore < rhs.qscore;
}


/*!
     Non-class comparison function used to sort table by quality score
     
     \param[in] lhs The left-hand entry
     \param[in] rhs The right-hand entry     
*/
bool SortByQScore (FrequencyEntry const& lhs, FrequencyEntry const& rhs) {
  return lhs.qscore < rhs.qscore;
}


//  -----------------------------------------------------------------
//  Main processing functions -- public
//  -----------------------------------------------------------------

/*!
     Update the frequency table using a vector of integers and the designated read terminator
     
     \param[in] x The vector of integers to update the table with
*/
void BlockStatistics::UpdateFrequencyTable (vector<unsigned int> x) {
  for (unsigned int i = 0; i < x.size (); i++) {
    unsigned int pos = x[i];
    m_FrequencyTable[pos].freq++;
  }

  return;
}


/*!
     Copy the remapping table into the private member variable for decoding
     
     \param[in] x The remapping table
*/
void BlockStatistics::CopyIDsToQScores (vector<unsigned int> x) {
  for (unsigned int i = 0; i <  x.size (); i++) {
    m_IDsToQScores[i] = x[i];
  }
  m_IDsToQScoresSize = x.size ();

  return;
}


//  -----------------------------------------------------------------
//  Main processing function -- private
//  -----------------------------------------------------------------


/*!
     Sort the frequency table three times:
       1)  Sort by frequency.
       2)  Assign IDs in sequential order.
       3)  Store these IDs as they will need to be written for the decoder.
       4)  Sort by quality score.
       5)  Store these IDs as we will need them now for encoding
*/
void BlockStatistics::ApplySort () {
  //  Don't need to sort again
  if (m_IsSorted) {
    return;
  }

  //  Sort the table by frequency and assign IDs to each entry
  sort (m_FrequencyTable.begin (), m_FrequencyTable.end (), &SortByFrequency);
  for (unsigned int i = 0; i < m_FrequencyTable.size (); i++) {
    m_FrequencyTable[i].id = i + 1;
  }

  //  Record this mapping of IDs to QScore and count how many IDs are actually in use
  for (unsigned int i = 0; i < g_MAX_ASCII; i++) {
    m_IDsToQScores[i] = m_FrequencyTable[i].qscore;
    if (m_FrequencyTable[i].freq != 0) {
      m_IDsToQScoresSize++;
    }
  }

  //  Sort the table by QScore
  sort (m_FrequencyTable.begin (), m_FrequencyTable.end (), &SortByQScore);

  //  Record this mapping of QScore to IDs; we have to do this one for all 0xFF entries as the gaps
  //  are important
  for (unsigned int i = 0; i < g_MAX_ASCII; i++) {
    m_QScoresToIDs[i] = m_FrequencyTable[i].id;
  }

  //  Indicate that we've sorted
  m_IsSorted = true;

  return;
}


