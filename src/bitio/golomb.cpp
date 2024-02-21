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
    \file golomb.cpp
    File for encoding and decoding Golomb codes.
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
#include "golomb.hpp"


//  -----------------------------------------------------------------
//  Encoding functions
//  -----------------------------------------------------------------

/*!
     Encode a value using Golomb coding

     \param bitbuffer BitBuffer object where the bits are sent
     \param value Number to encode
     \param b Parameter to Golomb coding
     \return The number of bits written
*/
unsigned int Golomb_Encode (BitBuffer &bitbuffer, unsigned int value, unsigned int b) {
  unsigned int x = value - 1;
  unsigned int bits_written = 0;
  unsigned int remainder = 0;
  unsigned int quotient = 0;

  while (x >= b) {
    x -= b;
    quotient++;
  }
  remainder = x + 1;

  bits_written = Unary_Encode (bitbuffer, quotient + 1);
  bits_written += BinaryHigh_Encode (bitbuffer, remainder, b);
  
  return (bits_written);
}


//  -----------------------------------------------------------------
//  Decoding functions
//  -----------------------------------------------------------------


/*!
     Decode a value using Golomb coding

     \param bitbuffer BitBuffer object where the bits are from
     \param b Parameter to Golomb coding
     \return decoded value
*/
unsigned int Golomb_Decode (BitBuffer &bitbuffer, unsigned int b) {
  unsigned int quotient = 0;
  unsigned int remainder = 0;
  unsigned int value = 0;

  quotient = Unary_Decode (bitbuffer);
  quotient--;

  remainder = BinaryHigh_Decode (bitbuffer, b);
  
  value = (quotient * b) + remainder;
  return (value);
}

