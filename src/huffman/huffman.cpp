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
    \file huffman.cpp
    Constructor and destructor for Huffman class definition .
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: huffman.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <fstream>  //  ostream
#include <vector>
#include <iostream>
#include <cstdlib>  //  EXIT_SUCCESS, EXIT_FAILURE, exit ()

using namespace std;

#include "common.hpp"
#include "bitbuffer.hpp"
#include "huffman.hpp"


//  -----------------------------------------------------------------
//  Friends
//  -----------------------------------------------------------------


/*!
     Overloaded << operator defined as a friend of Huffman for debugging purposes.

     \param[in] os Output stream
     \param[in] hc Huffman passed as reference
     \return Output stream
*/
ostream &operator<< (ostream &os, const Huffman& hc) {
//   os << hm.m_Table[0] << " (" << hm.m_Codewords[0] << ")";
//   for (unsigned int i = 0; i < hm.m_Table.size (); i++) {
//     os << "\t" << hm.m_Table[i] << " (" << hm.m_Codewords[i] << ")";
//   }
//   os << endl;

  return os;
}


//  -----------------------------------------------------------------
//  Constructors and destructors
//  -----------------------------------------------------------------


/*!
     Default constructor that takes one optional argument.
     
     \param[in] debug Set to true if in debug mode; false by default
*/
Huffman::Huffman (bool debug)
  : m_Debug (debug),
    m_MessageLength (0),
    m_MessageLengthDecoded (0),
    m_MaximumSymbol (0),
    m_DistinctSymbols (0),
    m_MaximumCodewordLen (0),
    m_SymsUsed (),
    m_Table (),
    m_W (),
    m_Base (),
    m_Offset (),
    m_LJLimit (),
    m_V (0),
    m_VBits (0)
{
  //  Add symbol 0 with 0 frequency as a sentinel value.  Only positions from 1 are used.
  m_Table.push_back (0);
  m_SymsUsed.push_back (0);
}


/*!
     Destructor that takes no arguments
*/
Huffman::~Huffman () {
}


//  -----------------------------------------------------------------
//  Constructors and destructors
//  -----------------------------------------------------------------

/*!
     Return m_Debug

     \return Debug mode or not
*/
bool Huffman::GetDebug () const {
  return m_Debug;
}


/*!
     Set m_Debug to true
*/
void Huffman::SetDebug () {
  m_Debug = true;
}


/*!
     Return m_MessageLength

     \return Length of the encoded message (same as "one/current" block)
*/
unsigned int Huffman::GetMessageLength () const {
  return m_MessageLength;
}


/*!
     Return a value in m_Table

     \return An unsigned int value
*/
unsigned int Huffman::GetTableValue (unsigned int pos) const {
  return m_Table[pos];
}

