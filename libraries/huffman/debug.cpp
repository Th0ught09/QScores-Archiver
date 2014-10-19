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
    \file debug.cpp
    Debugging functions for Huffman coding class definition .

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: debug.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <fstream>  //  ostream
#include <vector>
#include <climits>  //  UINT_MAX
#include <algorithm>  //  sort
#include <iostream>
#include <cassert>

using namespace std;

#include "systemcfg.hpp"
#include "bitbuffer.hpp"
#include "huffman.hpp"


//  -----------------------------------------------------------------
//  Public functions
//  -----------------------------------------------------------------

/*!
     Debugging function to convert array into non-increasing order by using cumulative sums
*/
void Huffman::DebugCumulativeSum () {
  int i = static_cast<int> (m_Table.size ()) - 2;
  while (i > 0) {
    m_Table[i] += m_Table[i + 1];
    i--;
  }

  return;
}


//  -----------------------------------------------------------------
//  Private functions
//  -----------------------------------------------------------------


/*!
     Verify that the array is correct -- the sentinel values should both be 0 (i.e., symbol 0 should not have occurred).
     
     \return true if the array is correct; false otherwise
*/
bool Huffman::VerifyFrequencies () {
  bool result = true;  //  "true" means the array is fine

  //  Check if the symbol 0 occurred
  if (m_Table[0] != 0) {
    return false;
  }

  //  Check if the symbol 0 occurred
  if (m_SymsUsed[0] != 0) {
    return false;
  }

//   for (unsigned int i = 2; i < m_Table.size (); i++) {
//     if (m_Table[i] > m_Table[i - 1]) {
//       result = false;
//       break;
//     }
//   }
  
  return (result);
}


