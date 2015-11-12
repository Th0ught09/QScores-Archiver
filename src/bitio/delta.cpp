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
    \file delta.cpp
    Delta encoding and decoding functions.
    Delta encoding uses [gamma, binary].
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: delta.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/


#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#include "bitbuffer.hpp"
#include "math_utils.hpp"
#include "gamma.hpp"
#include "delta.hpp"


//  -----------------------------------------------------------------
//  Encoding functions
//  -----------------------------------------------------------------

/*!
     Encode a value using delta code

     \param bitbuffer BitBuffer object where the bits are sent
     \param value Number to encode
     \return The number of bits written
*/
unsigned int Delta_Encode (BitBuffer &bitbuffer, unsigned int value) {
  unsigned int x = value;
  unsigned int logx = 0;
  unsigned int bits_written = 0;

  logx = FloorLog (x);

  bits_written = Gamma_Encode (bitbuffer, logx + 1);
  bitbuffer.WriteBits (x - (1 << logx), logx);
  bits_written += logx;
  
  return (bits_written);
}


/*!
     Encode a value, which is at least 'low', in delta code

     \param bitbuffer BitBuffer object where the bits are sent
     \param value Number to encode
     \param low Lower bound on x
     \return The number of bits written
*/
unsigned int DeltaLow_Encode (BitBuffer &bitbuffer, unsigned int value, unsigned int low) {
  unsigned int x = value;
  unsigned int logx = 0;
  unsigned int bits_written = 0;

  x -= low;
  logx = FloorLog (x);

  bits_written = GammaLow_Encode (bitbuffer, logx + 1, 0);
  bitbuffer.WriteBits (x - (1 << logx), logx);
  bits_written += logx;
  
  return (bits_written);
}


//  -----------------------------------------------------------------
//  Decoding functions
//  -----------------------------------------------------------------


/*!
     Decode a value using delta code

     \param bitbuffer BitBuffer object where the bits are from
     \return decoded value
*/
unsigned int Delta_Decode (BitBuffer &bitbuffer) {
  unsigned int length = Gamma_Decode (bitbuffer) - 1;

  return ((1 << length) + bitbuffer.ReadBits (length));
}


/*!
     Decode a value, which is at least 'low', in delta code

     \param bitbuffer BitBuffer object where the bits are sent
     \param low Lower bound on x
*/
unsigned int DeltaLow_Decode (BitBuffer &bitbuffer, unsigned int low) {
  unsigned int length = GammaLow_Decode (bitbuffer, 0) - 1;

  return (low + (1 << length) + bitbuffer.ReadBits (length));
}


