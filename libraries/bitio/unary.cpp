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
    \file unary.cpp
    Unary encoding and decoding functions.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: unary.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <climits>

using namespace std;

#include "systemcfg.hpp"
#include "bitbuffer.hpp"
#include "math_utils.hpp"
#include "unary.hpp"


//  -----------------------------------------------------------------
//  Encoding functions
//  -----------------------------------------------------------------

/*!
     Encode a value in unary.  Assumes value > 0.

     \param bitbuffer BitBuffer object where the bits are sent
     \param value Number to encode
     \return The number of bits written
*/
unsigned int Unary_Encode (BitBuffer &bitbuffer, unsigned int value) {
  unsigned int x = value;
  unsigned int bits_written = 0;

  //  Repeatedly write out 1's until x is less than g_UINT_SIZE_BITS
  while (x > g_UINT_SIZE_BITS) {
    bitbuffer.WriteBits (UINT_MAX, g_UINT_SIZE_BITS);
    x -= g_UINT_SIZE_BITS;
    bits_written += g_UINT_SIZE_BITS;
  }

  bitbuffer.WriteBits ((ALL_1_EXCEPT_LAST), x);
  bits_written += (x);

  return (bits_written);
}


/*!
     Encode a value, which is at least 'low', in unary

     \param bitbuffer BitBuffer object where the bits are sent
     \param value Number to encode
     \param low Lower bound on x
     \return The number of bits written
*/
unsigned int UnaryLow_Encode (BitBuffer &bitbuffer, unsigned int value, unsigned int low) {
  unsigned int x = value - low;
  unsigned int bits_written = 0;

  bits_written = Unary_Encode (bitbuffer, x);

  return (bits_written);
}


//  -----------------------------------------------------------------
//  Decoding functions
//  -----------------------------------------------------------------


/*!
     Decode a value in unary

     \param bitbuffer BitBuffer object where the bits are from
     \return decoded value
*/
unsigned int Unary_Decode (BitBuffer &bitbuffer) {
  unsigned int x = 0;

  while (bitbuffer.ReadBits (1) == 1) {
    x++;
  }
  x++;
  
  return (x);
}


/*!
     Decode a value, which is at least 'low', in unary

     \param bitbuffer BitBuffer object where the bits are sent
     \param low_arg Lower bound on x
     \return Decoded value
*/
unsigned int UnaryLow_Decode (BitBuffer &bitbuffer, unsigned int low_arg) {
  unsigned int x = 0;

  x = Unary_Decode (bitbuffer);
  
  return (x + low_arg);
}


