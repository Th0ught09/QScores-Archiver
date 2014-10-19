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
    \file gamma.cpp
    Gamma encoding and decoding functions.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: gamma.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>  //  exit
#include <climits>

using namespace std;

#include "bitbuffer.hpp"
#include "math_utils.hpp"
#include "unary.hpp"
#include "binary.hpp"
#include "gamma.hpp"


//  -----------------------------------------------------------------
//  Encoding functions
//  -----------------------------------------------------------------

/*!
     Encode a value using gamma code

     \param bitbuffer BitBuffer object where the bits are sent
     \param value Number to encode
     \return The number of bits written
*/
unsigned int Gamma_Encode (BitBuffer &bitbuffer, unsigned int value) {
  unsigned int x = value;
  unsigned int logx = 0;
  unsigned int bits_written = 0;

  logx = FloorLog (x);

  bits_written = Unary_Encode (bitbuffer, logx + 1);
  bitbuffer.WriteBits (x - (1 << logx), logx);
  bits_written += logx;

  return (bits_written);
}


/*!
     Encode a value, which is at least 'low', in gamma

     \param bitbuffer BitBuffer object where the bits are sent
     \param value Number to encode
     \param low Lower bound on x
     \return The number of bits written
*/
unsigned int GammaLow_Encode (BitBuffer &bitbuffer, unsigned int value, unsigned int low) {
  unsigned int x = value;
  unsigned int bits_written = 0;

  x -= low;
  bits_written = Gamma_Encode (bitbuffer, x);

  return (bits_written);
}


/*!
     Encode an unsigned int value which is between 'low' and 'high' using gamma/binary coding.

     \param bitbuffer BitBuffer object where the bits are sent
     \param value Number to encode
     \param low Lower bound on x
     \param high Upper bound on x
     \return The number of bits written     
*/
unsigned int GammaLowHigh_Encode (BitBuffer &bitbuffer, unsigned int value, unsigned int low, unsigned int high) {
  unsigned int range = high - low + 1;
  unsigned int x = value - low;
  unsigned int bits_written = 0;

  if (range > 2) {
    bits_written = Gamma_Encode (bitbuffer, x + 1);
  }
  else {
    bits_written = BinaryHigh_Encode (bitbuffer, x + 1, range);
  }

  return (bits_written);
}


//  -----------------------------------------------------------------
//  Decoding functions
//  -----------------------------------------------------------------


/*!
     Decode a value using gamma code

     \param bitbuffer BitBuffer object where the bits are from
     \return decoded value
*/
unsigned int Gamma_Decode (BitBuffer &bitbuffer) {
  unsigned int length = Unary_Decode (bitbuffer) - 1;
  unsigned int temp = 0;
  unsigned int temp2 = 0;

  temp2 = bitbuffer.ReadBits (length);
  temp = (1 << length) + temp2;

  return (temp);
}


/*!
     Decode a value, which is at least 'low', in gamma code

     \param bitbuffer BitBuffer object where the bits are sent
     \param low Lower bound on x
*/
unsigned int GammaLow_Decode (BitBuffer &bitbuffer, unsigned int low) {
  unsigned int temp = 0;

  temp = Gamma_Decode (bitbuffer) + low;

  return (temp);
}


/*!
     Decode an unsigned int value which is between 'low' and 'high' using gamma/binary coding.

     \param bitbuffer BitBuffer object where the bits are received from
     \param low Lower bound on x
     \param high Upper bound on x
     \return decoded value
*/
unsigned int GammaLowHigh_Decode (BitBuffer &bitbuffer, unsigned int low, unsigned int high) {
  unsigned int range = high - low + 1;
  unsigned int temp = 0;

  if (range > 2) {
    temp = Gamma_Decode (bitbuffer);
  }
  else {
    temp = BinaryHigh_Decode (bitbuffer, 2);
  }

  temp = temp + low - 1;

  return (temp);
}

