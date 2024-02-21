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
    \file process.cpp
    Processing functions for Huffman coding class definition .
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
#include "bitbuffer.hpp"
#include "huffman.hpp"


//  -----------------------------------------------------------------
//  Public functions
//  -----------------------------------------------------------------

/*!
     Update the frequency table using a vector of integers

     \param[in] x The vector of integers to update the table with
*/
void Huffman::UpdateFrequencies (vector<unsigned int> x) {
  for (unsigned int i = 0; i < x.size (); i++) {
    unsigned int pos = x[i];
    if (pos >= m_Table.size ()) {
      m_Table.resize (pos + 1, 0);
    }
    if (m_Table[pos] == 0) {
      m_DistinctSymbols++;
      m_SymsUsed.push_back (pos);
    }
    m_Table[pos]++;
  }

  m_MessageLength += x.size ();

  return;
}


//  -----------------------------------------------------------------
//  Private functions
//  -----------------------------------------------------------------

/*!
     Calculate Huffman code.  Follows the pseudocode of Compression and Coding Algorithms by Moffat and Turpin, page 67 (Algorithm 4.2).
     
     Difference with CACA implementation:  
       Added an if-test if the number of distinct symbols is exactly 1.  Sets the m_Table and then returns.
*/
void Huffman::CalculateHuffmanCode () {
  //  Note:  Number of distinct symbols already excludes the sentinel
  unsigned int distinct = m_DistinctSymbols;
  unsigned int r = distinct;
  unsigned int s = distinct;
  
//   cerr << "CalculateHuffmanCode ()'s m_DistinctSymbols = " << distinct << endl;
  
//   for (unsigned int i = 0; i < m_SymsUsed.size (); i++) {
//     cerr << "\t\t[+]\t" << i << "\t" << m_Table[m_SymsUsed[i]] << endl;
//   }
  
  //  Added for the special case of an alphabet of one symbol
  if (distinct == 1) {
    m_Table[m_SymsUsed[1]] = 1;
    return;
  }
  
  for (unsigned int x = distinct; x >= 2; x--) {
    if ((s < 1) || ((r > x) && (m_Table[m_SymsUsed[r]] < m_Table[m_SymsUsed[s]]))) {
      m_Table[m_SymsUsed[x]] = m_Table[m_SymsUsed[r]];
      m_Table[m_SymsUsed[r]] = x;
      r--;
    }
    else {
      m_Table[m_SymsUsed[x]] = m_Table[m_SymsUsed[s]];
      s--;
    }
    if ((s < 1) || ((r > x) && (m_Table[m_SymsUsed[r]] < m_Table[m_SymsUsed[s]]))) {
      m_Table[m_SymsUsed[x]] = m_Table[m_SymsUsed[x]] + m_Table[m_SymsUsed[r]];
      m_Table[m_SymsUsed[r]] = x;
      r--;
    }
    else {
      m_Table[m_SymsUsed[x]] = m_Table[m_SymsUsed[x]] + m_Table[m_SymsUsed[s]];
      s--;
    }
  }

  m_Table[m_SymsUsed[2]] = 0;
  for (unsigned int x = 3; x <= distinct; x++) {
    m_Table[m_SymsUsed[x]] = m_Table[m_SymsUsed[m_Table[m_SymsUsed[x]]]] + 1;
  }

  unsigned int a = 1;
  unsigned int u = 0;
  unsigned int d = 0;
  r = 2;
  unsigned int x = 1;
  while (a > 0) {
    while ((r <= distinct) && (m_Table[m_SymsUsed[r]] == d)) {
      u++;
      r++;
    }
    while (a > u) {
      m_Table[m_SymsUsed[x]] = d;
      x++;
      a--;
    }
    a = 2 * u;
    d++;
    u = 0;
  }

  return;
}


/*!
     Set the m_W, m_Base, and m_Offset arrays for efficient encoding/decoding.
*/
void Huffman::SetWBaseOffset () {
  //  All arrays have a size of (m_L + 2) since position 0 is unused and a sentinel value is needed for m_Offset
  //  Also, initialize everything to 0.
  m_W.resize (m_MaximumCodewordLen + 2, 0);
  m_Base.resize (m_MaximumCodewordLen + 2, 0);
  m_Offset.resize (m_MaximumCodewordLen + 2, 0);
  m_LJLimit.resize (m_MaximumCodewordLen + 2, 0);
  
  //  For each i, record the number of symbols with code length i (skip position 0)
  for (unsigned int i = 1; i < m_SymsUsed.size (); i++) {
    m_W[m_Table[m_SymsUsed[i]]]++;
  }
  
  m_Base[1] = 0;
  m_Offset[1] = 1;
  m_Offset[m_MaximumCodewordLen + 1] = m_DistinctSymbols + 1;  //  Sentinel value

  for (unsigned int i = 2; i <= m_MaximumCodewordLen; i++) {
    m_Base[i] = 2 * (m_Base[i - 1] + m_W[i - 1]);
    m_Offset[i] = m_Offset[i - 1] + m_W[i - 1];
  }


  for (unsigned int i = 1; i <= m_MaximumCodewordLen; i++) {
    if (i == m_MaximumCodewordLen) {
      m_LJLimit[i] = 1 << m_MaximumCodewordLen;
    }
    else {
      m_LJLimit[i] = m_Base[i + 1] * (1 << (m_MaximumCodewordLen - i - 1));
    }
  }

//   if (GetDebug ()) {
//     for (unsigned int i = 1; i <= m_MaximumCodewordLen; i++) {
//       cerr << "\t[*]\t" << i << "\t" << m_W[i] << "\t" << m_Base[i] << "\t" << m_Offset[i] << "\t" << m_LJLimit[i] << endl;
//     }
//     cerr << "\t[**]\t" << m_MaximumCodewordLen + 1 << "\t" << m_W[m_MaximumCodewordLen + 1] << "\t" << "-" << "\t" << m_Offset[m_MaximumCodewordLen + 1] << "\t" << "-" << endl;
//   }
  
  return;
}


