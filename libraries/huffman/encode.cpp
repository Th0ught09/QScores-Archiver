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
    \file encode.cpp
    Encoding functions for Huffman coding class definition .

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: encode.cpp 224 2011-11-06 15:32:09Z rwan $

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
     Perform the preliminary calculations in preparation for encoding.
     
     \param[in] bitbuffer The bitbuffer to write the bits to.
*/
void Huffman::EncodeBegin (BitBuffer &bitbuffer) {
  if (!VerifyFrequencies ()) {
    cerr << "EE\tHuffman's calculation array is incorrect!  Values should be non-increasing!" << endl;
    exit (EXIT_FAILURE);
  }

  //  Sort the symbols by decreasing frequency
//   cerr << "II\tSortSymsUsed_Frequency ()" << endl;
//   for (unsigned int i = 0; i < m_SymsUsed.size (); i++) {
//     cerr << "\t[0]\t" << i << "\t" << m_SymsUsed[i] << "\t(" << m_Table[m_SymsUsed[i]] << ")" << endl;
//   }
  SortSymsUsed_Frequency ();
//   for (unsigned int i = 0; i < m_SymsUsed.size (); i++) {
//     cerr << "\t[1]\t" << i << "\t" << m_SymsUsed[i] << "\t(" << m_Table[m_SymsUsed[i]] << ")" << endl;
//   }

  //  Calculate the Huffman codeword lengths 
//   cerr << "II\tCalculateHuffmanCode ()" << endl;
  CalculateHuffmanCode ();
//   for (unsigned int i = 0; i < m_SymsUsed.size (); i++) {
//     cerr << "\t[2]\t" << i << "\t" << m_Table[m_SymsUsed[i]] << endl;
//   }
  
  //  Initialize the codewords to 0, assign them, and then record the longest codeword length
//   cerr << "II\t\tm_DistinctSymbols = " << m_DistinctSymbols << endl;
  m_MaximumCodewordLen = m_Table[m_SymsUsed[m_DistinctSymbols]];
//   cerr << "II\t\tm_MaximumCodewordLen = " << m_MaximumCodewordLen << endl;

  //  Sort the symbols by value and then record the maximum possible symbol
//   cerr << "II\tSortSymsUsed_Value ()" << endl;
  SortSymsUsed_Value ();
  m_MaximumSymbol = m_SymsUsed[m_DistinctSymbols];
//   cerr << "II\t\tm_MaximumSymbol = " << m_MaximumSymbol << endl;

  //  Set the m_W, m_Base, and m_Offset arrays
//   cerr << "II\tSetWBaseOffset ()" << endl;
  SetWBaseOffset ();
//   for (unsigned int i = 0; i < m_W.size (); i++) {
//     cerr << "\t[4]\t" << i << "\t" << m_W[i] << "\t" << m_Base[i]  << "\t" << m_Offset[i] << endl;
//   }
  
//   cerr << "II\tEncodePrelude ()" << endl;
  EncodePrelude (bitbuffer);
//   for (unsigned int i = 0; i < m_SymsUsed.size (); i++) {
//     cerr << "\t[5]\t" << i << "\t" << m_Table[m_SymsUsed[i]] << endl;
//   }

  PreEncodeMessage ();
//   for (unsigned int i = 0; i < m_SymsUsed.size (); i++) {
//     cerr << "\t[6]\t" << i << "\t" << m_Table[m_SymsUsed[i]] << endl;
//   }
  
  return;
}


/*!
     Encode a vector x using the calculated Huffman codes.
     
     \param[in] bitbuffer The bitbuffer to write the bits to.
     \param[in] x The vector to encode.
*/
void Huffman::EncodeMessage (BitBuffer &bitbuffer, vector<unsigned int> x) {
//   cerr << "[Huffman::EncodeMessage**] size --\t" << x.size () << endl;
//   cerr << "[Huffman::EncodeMessage*]\t10\t" << m_Table[10] << endl;
//   cerr << "[Huffman::EncodeMessage*]\t70\t" << m_Table[70] << endl;
//   cerr << "[Huffman::EncodeMessage*]\t90\t" << m_Table[90] << endl;
  for (unsigned int i = 0; i < x.size (); i++) {
//     cerr << "[Huffman::EncodeMessage]\t" << i << "\t" << x[i] << "\t" << m_Table[x[i]] << endl;
    EncodeSymbol (bitbuffer, m_Table[x[i]]);
  }
  
  return;
}


/*!
     Finish encoding, so write out a 0 of length m_MaximumCodewordLen.  The reason is that 
     the decoder is reading in bits into a buffer and if it reads too much, then somehow,
     it has to put the bits back to BitBuffer (i.e., some kind of ungetc ()).  This is more 
     difficult to do since there is a small chance that there is no room ot unget to.
     
     As an alternative, we write this value out to signal we are finished and read it in
     when we finish decoding the block.  As a "bonus" if the value is a 0, then we have
     more confidence that things were decoded properly.
     
     \param[in] bitbuffer The bitbuffer to write the bits to.
*/
void Huffman::EncodeFinish (BitBuffer &bitbuffer) {
  //  Write out a 0 occupying m_MaximumCodewordLen bits
  bitbuffer.WriteBits (0, m_MaximumCodewordLen);
  
  return;
}


//  -----------------------------------------------------------------
//  Private functions
//  -----------------------------------------------------------------

/*!
     Modify the m_Table data structure in preparation for message encoding.
     
     Difference with CACA implementation:  
       The first for-loop starts from i = 1 and not i = 2.  This seems necessary for the case of encoding 
       data made up of an alphabet of 2 symbols.
*/
void Huffman::PreEncodeMessage () {
//   cerr << "[PreEncodeMessage]\t" << m_W[1] << "\t" << m_Offset[1] << endl;
  for (unsigned int i = 1; i <= m_MaximumCodewordLen; i++) {
    m_W[i] = m_Offset[i];
  }
  for (unsigned int i = 1; i < m_SymsUsed.size (); i++) {
    unsigned int sym = m_SymsUsed[i];
    unsigned int codelen = m_Table[sym];
//     cerr << "\t[8]\t" << sym << "\t" << m_W[codelen] << endl;
    m_Table[sym] = m_W[codelen];
    m_W[codelen]++;
  }

  return;
}


/*!
     Encode the prelude
     
     \param[in] bitbuffer The bitbuffer to write the bits to.
*/
void Huffman::EncodePrelude (BitBuffer &bitbuffer) {
  unsigned int bits_written = 0;
  vector<unsigned int> tmp;
  
  //  Encode information about this block
//   cerr << "II\t[EncodePrelude]\t" << m_MessageLength << "\t" << m_MaximumSymbol << "\t" << m_DistinctSymbols << "\t" << m_MaximumCodewordLen << endl;
  bits_written += Delta_Encode (bitbuffer, m_MessageLength);
  bits_written += Delta_Encode (bitbuffer, m_MaximumSymbol);
  bits_written += Delta_Encode (bitbuffer, m_DistinctSymbols);
  bits_written += Delta_Encode (bitbuffer, m_MaximumCodewordLen);

  //  Encode the sub-alphabet (symbols that appear in this block)
  for (unsigned int i = 1; i < m_SymsUsed.size (); i++) {
//     cerr << "\t[encode.cpp::EncodePrelude]\t" << i << "\t" << m_SymsUsed[i] << endl;
    tmp.push_back (m_SymsUsed[i]);
  }
  Interpolative_Encode (bitbuffer, tmp);

  //  Encode the codeword lengths
  for (unsigned int i = 1; i < m_SymsUsed.size (); i++) {
//     cerr << "\t[6]\t" << i << "\t" << m_Table[m_SymsUsed[i]] << endl;
    bits_written += Unary_Encode (bitbuffer, ((m_MaximumCodewordLen + 1) - m_Table[m_SymsUsed[i]]));
  }

  return;
}


/*!
     Encode a single symbol using the calculated Huffman codes.
     
     \param[in] bitbuffer The bitbuffer to write the bits to.
     \param[in] x The symbol to encode.
*/
void Huffman::EncodeSymbol (BitBuffer &bitbuffer, unsigned int x) {
  unsigned int l = 1;
  while (x >= m_Offset[l + 1]) {
    l++;
  }
  
  unsigned int c = ((x - m_Offset[l]) + m_Base[l]);

//   cerr << "\t" << c << "\t(length " << l << ")" << endl;
  bitbuffer.WriteBits (c, l);  

  return;
}


