///////////////////////////////////////////////////////////////////////////
//  QScores-Archiver
//  Software and library for transforming and compressing next generation
//    sequencing data in FASTQ format.
//  
//  Version 1.00 -- November 6, 2011
//  
//  Copyright (C) 2011 by Raymond Wan, All rights reserved.
//  Contact:  rwan@cuhk.edu.hk
//  Organization:  Department of Computational Biology, Graduate School of
//                 Frontier Science, University of Tokyo and
//                 Computational Biology Research Center, AIST,
//                 Japan
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
    \file block-statistics.cpp
    Constructor and destructor for BlockStatistics class definition .
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: block-statistics.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <fstream>  //  ostream
#include <vector>
#include <climits>  //  UINT_MAX
#include <algorithm>  //  sort
#include <iostream>

using namespace std;

#include "systemcfg.hpp"
#include "block-statistics.hpp"


//  -----------------------------------------------------------------
//  Constructors and destructors
//  -----------------------------------------------------------------

/*!
     Default constructor that takes one optional argument.
     
     \param[in] debug Set to true if in debug mode; false by default
*/
BlockStatistics::BlockStatistics (bool debug)
  : m_Debug (debug),
    m_IsSorted (false),
    m_FrequencyTable (),
    m_QScoresToIDs (),
    m_IDsToQScores (),
    m_IDsToQScoresSize (0)
{
  for (unsigned int i = 0; i < g_MAX_ASCII; i++) {
    m_FrequencyTable.push_back (FrequencyEntry (i));
  }

  m_QScoresToIDs.resize (g_MAX_ASCII, UINT_MAX);
  m_IDsToQScores.resize (g_MAX_ASCII, UINT_MAX);
}


/*!
     Destructor that takes no arguments
*/
BlockStatistics::~BlockStatistics () {
}


/*!
     Initialization function that sets everything to the defaults of FrequencyEntry ().  Also initializes the remapping lookup tables.
     
     Assumes the constructor was called already.
*/
void BlockStatistics::Initialize () {
  for (unsigned int i = 0; i < m_FrequencyTable.size (); i++) {
    m_FrequencyTable[i].qscore = i;
    m_FrequencyTable[i].freq = 0;
    m_FrequencyTable[i].id = UINT_MAX;
  }

  //  Initialize the lookup tables
  m_QScoresToIDs.resize (g_MAX_ASCII, UINT_MAX);
  m_IDsToQScores.resize (g_MAX_ASCII, UINT_MAX);
  m_IDsToQScoresSize = 0;

  m_IsSorted = false;
  
  return;
}


