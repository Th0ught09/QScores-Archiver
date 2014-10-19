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
    \file decode.cpp
    Decoding functions for Huffman coding class definition .

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: decode.cpp 224 2011-11-06 15:32:09Z rwan $

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
#include "bitio-defn.hpp"
#include "interpolative.hpp"
#include "huffman.hpp"


//  -----------------------------------------------------------------
//  Public functions
//  -----------------------------------------------------------------


/*!
     Commence decoding
     
     \param[in] bitbuffer The bitbuffer to read the bits from.
*/
void Huffman::DecodeBegin (BitBuffer &bitbuffer) {
//   if (GetDebug ()) {
//     cerr << "II\tDecodePrelude ()" << endl;
//   }
  DecodePrelude (bitbuffer);

  //  Set the m_W, m_Base, and m_Offset arrays
//   if (GetDebug ()) {
//     cerr << "II\tSetWBaseOffset ()" << endl;
//   }
  SetWBaseOffset ();
  
  PreDecodeMessage ();
  
  return;
}


/*!
     Decode part or all of the message
     
     \param[in] bitbuffer The bitbuffer to read the bits from.
     \param[in] len Length of the message to decode; can be less than m_MessageLength if we want to decode a piece at a time
     \return message as a vector
*/
vector<unsigned int> Huffman::DecodeMessage (BitBuffer &bitbuffer, unsigned int len) {
  vector<unsigned int> tmp;

  //  Ensure we aren't decoding too much
  if (m_MessageLengthDecoded + len > m_MessageLength) {
    len = m_MessageLength - m_MessageLengthDecoded;
  }
  
  for (unsigned int i = 0; i < len; i++) {
    unsigned int x = DecodeSymbol (bitbuffer);
    tmp.push_back (x);
  }
  
  m_MessageLengthDecoded += len;

  return (tmp);
}


/*!
     Finish decoding, read in the extra unsigned integer that was written out by EncodeFinish ()
     and make sure it has a value of 0.  See the documentation EncodeFinish () for an explanation.
     
     \param[in] bitbuffer The bitbuffer to write the bits to.
*/
void Huffman::DecodeFinish (BitBuffer &bitbuffer) {
  unsigned int bits_to_read = m_MaximumCodewordLen - m_VBits;
  
  m_V = (m_V << bits_to_read) + bitbuffer.ReadBits (bits_to_read);
  m_VBits += bits_to_read;
  
  return;
}


//  -----------------------------------------------------------------
//  Private functions
//  -----------------------------------------------------------------


/*!
     Modify the m_Table data structure in preparation for message decoding.
     
     Differs from the CACA pseudocode because m_Table is of size m_MaximumSymbol for both the encoder and the decoder.
     That is, we refer to symbols in the dense table as m_Table[m_SymsUsed[i]].  This makes the loop in the CACA
     pseudocode not work.  An easy fix is to just allocate a temporary array.  
     
     So, an additional array of m_DistinctSymbols words of memory is being used.
     
     Another difference with CACA implementation:  
       The first for-loop starts from i = 1 and not i = 2.  See Huffman::PreEncodeMessage ().
*/
void Huffman::PreDecodeMessage () {
  for (unsigned int i = 1; i <= m_MaximumCodewordLen; i++) {
    m_W[i] = m_Offset[i];
  }
  for (unsigned int i = 1; i <= m_DistinctSymbols; i++) {
    unsigned int codelen = m_Table[m_SymsUsed[i]];
    m_Table[m_SymsUsed[i]] = m_W[codelen];
    m_W[codelen]++;
  }

  //  Create a temporary copy of the array
  vector<unsigned int> tmp = m_SymsUsed;
  for (unsigned int i = 1; i <= m_DistinctSymbols; i++) {
    m_SymsUsed[m_Table[tmp[i]]] = tmp[i];
  }    
  
  return;
}


/*!
     Decode the prelude
     
     \param[in] bitbuffer The bitbuffer to read the bits from.
*/
void Huffman::DecodePrelude (BitBuffer &bitbuffer) {
  vector<unsigned int> tmp;

  //  Decode information about this block
  m_MessageLength = Delta_Decode (bitbuffer);
  m_MaximumSymbol = Delta_Decode (bitbuffer);
  m_DistinctSymbols = Delta_Decode (bitbuffer);
  m_MaximumCodewordLen = Delta_Decode (bitbuffer);

//   if (GetDebug ()) {
//     cerr << "II\tDecoding\t" << m_MessageLength << "\t" << m_MaximumSymbol << "\t" << m_DistinctSymbols << "\t" << m_MaximumCodewordLen << endl;
//   }
  
  //  Set the table sizes now that it is known; initialize everything to 0
  m_Table.resize (m_MaximumSymbol + 1, 0);
  
  //  Decode the sub-alphabet (symbols that appear in this block)
  Interpolative_Decode (bitbuffer, tmp, m_DistinctSymbols);
  for (unsigned int i = 0; i < tmp.size (); i++) {
//     cerr << "\t[5]\t" << i << "\t" << tmp[i] << endl;
    m_SymsUsed.push_back (tmp[i]);
  }

  //  Decode the codeword lengths
  for (unsigned int i = 1; i < m_SymsUsed.size (); i++) {
    unsigned int x = Unary_Decode (bitbuffer);
    m_Table[m_SymsUsed[i]] = (m_MaximumCodewordLen + 1) - x;
//     cerr << "\t[6]\t" << m_SymsUsed[i] << "\t" << m_Table[m_SymsUsed[i]] << endl;
  }

  return;
}


/*!
     Decode a single symbol using the calculated Huffman codes.
     
     \param[in] bitbuffer The bitbuffer to read the bits from.
     \return The symbol.
*/
unsigned int Huffman::DecodeSymbol (BitBuffer &bitbuffer) {
  unsigned int l = 1;
  
  if (m_VBits < m_MaximumCodewordLen) {
    unsigned int bits_to_read = m_MaximumCodewordLen - m_VBits;
    try {
      m_V = (m_V << bits_to_read) + bitbuffer.ReadBits (bits_to_read);
      m_VBits += bits_to_read;
    }
    catch (exception &BitBuffer_Input_Exception) {
      cerr << "EE\tOpps!" << endl;
    }
  }
  
  while (m_V >= m_LJLimit[l]) {
    l++;
  }
  
  unsigned int c = (m_V >> (m_MaximumCodewordLen - l));
  m_V = m_V - (c << (m_MaximumCodewordLen - l));
  unsigned int x = (c - m_Base[l]) + m_Offset[l];
  
  m_VBits -= l;

  return (m_SymsUsed[x]);
}

