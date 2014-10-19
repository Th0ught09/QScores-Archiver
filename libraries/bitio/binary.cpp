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
    \file binary.cpp
    Binary encoding and decoding functions.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: binary.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#include "systemcfg.hpp"
#include "bitbuffer.hpp"
#include "math_utils.hpp"
#include "binary.hpp"


//  -----------------------------------------------------------------
//  Encoding functions
//  -----------------------------------------------------------------


/*!
     Encode a value using a minimal binary code that is in the range 1 <= x <= high .

     Source:  Compression and Coding Algorithms by A. Moffat and A. Turpin, page 31.
              One notable change is the addition of the case when high == 1, nothing is written.

     \param bitbuffer BitBuffer object where the bits are sent
     \param value Number to encode
     \param high Upper limit of the value
     \return The number of bits written     
*/
unsigned int BinaryHigh_Encode (BitBuffer &bitbuffer, unsigned int value, unsigned int high) {
  
  unsigned int x = value;
  unsigned int b = CeilLog (high);
  unsigned int d = (1 << b) - high;
  unsigned int bits_written = 0;

  if (high == 1) {
    bits_written = 0;
  }
  else if (x > d) {
    bitbuffer.WriteBits (x - 1 + d, b);
    bits_written = b;
  }
  else {
    bitbuffer.WriteBits (x - 1, b - 1);
    bits_written = b - 1;
  }

  return (bits_written);
}


/*!
     Encode an unsigned int value which is between 'low' and 'high' using binary coding.

     \param bitbuffer BitBuffer object where the bits are sent
     \param value Number to encode
     \param low Lower bound on x
     \param high Upper bound on x
     \return The number of bits written     
*/
unsigned int BinaryLowHigh_Encode (BitBuffer &bitbuffer, unsigned int value, unsigned int low, unsigned int high) {
  unsigned int bits_written = 0;

  bits_written = BinaryHigh_Encode (bitbuffer, value - low + 1, high - low + 1);
  
  return (bits_written);
}


//  -----------------------------------------------------------------
//  Decoding functions
//  -----------------------------------------------------------------


/*!
     Decode a value using binary code that is in the range 1 <= x <= high
     
     Source:  Compression and Coding Algorithms by A. Moffat and A. Turpin, page 31.
              One notable change is the addition of the case when high == 1, nothing 
                is written but 1 was assumed to be encoded.

     \param bitbuffer BitBuffer object where the bits are from
     \param high Upper limit of the value
     \return decoded value
*/
unsigned int BinaryHigh_Decode (BitBuffer &bitbuffer, unsigned int high) {
  unsigned int b = 0;
  unsigned int d = 0;
  unsigned int x = 0;
  
  if (high == 1) {
    return 1;
  }

  b = CeilLog (high);
  d = (1 << b) - high;
  
  if (b != 0) {
    x = bitbuffer.ReadBits (b - 1);
  }
  
  if (x + 1 > d) {
    x = 2 * x + bitbuffer.ReadBits (1);
    x = x - d;
  }

  return (x + 1);
}


/*!
     Decode an unsigned int value which is between 'low' and 'high' using binary coding.

     \param bitbuffer BitBuffer object where the bits are received from
     \param low Lower bound on x
     \param high Upper bound on x
*/
unsigned int BinaryLowHigh_Decode (BitBuffer &bitbuffer, unsigned int low, unsigned int high) {
  unsigned int x = 0;

  if (high == low) {
    return (low);
  }
  x = BinaryHigh_Decode (bitbuffer, high - low + 1);
  x = x + low - 1;

  return (x);
}


