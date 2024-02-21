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
    \file bytecode.cpp
    File for encoding and decoding bytecodes.
*/
/*******************************************************************/


#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>  //  exit
#include <climits>

using namespace std;

#include "common.hpp"
#include "bitbuffer.hpp"
#include "math_utils.hpp"
#include "unary.hpp"
#include "binary.hpp"


//  -----------------------------------------------------------------
//  Encoding functions
//  -----------------------------------------------------------------

/*!
     Encode a value using bytecodes.  It is possible to store a 0.

     \param bitbuffer BitBuffer object where the bits are sent
     \param value Number to encode
     \return The number of bits written
*/
unsigned int Bytecode_Encode (BitBuffer &bitbuffer, unsigned int value) {
  unsigned int bits_written = 0;
  
  if (value > ((1 << 14) - 1)) {
    cerr << "EE\tValue out of range for two-byte bytecodes:  " << value << endl;
    exit (EXIT_FAILURE);
  }

  //  Encode values less than or equal to 127 using 8 bits:  0xxxxxxx
  //  Other values less than 32767 are encoded using 16 bits:  1xxxxxxx xxxxxxxx
  if (value <= 127) {
    bitbuffer.WriteBits (0, 1);
    bitbuffer.WriteBits (value, 7);
    bits_written = 8;
  }
  else {
    bitbuffer.WriteBits (1, 1);
    bitbuffer.WriteBits (value, 15);
    bits_written = 16;
  }

  return (bits_written);
}


//  -----------------------------------------------------------------
//  Decoding functions
//  -----------------------------------------------------------------


/*!
     Decode a value using bytecodes

     \param bitbuffer BitBuffer object where the bits are from
     \return decoded value
*/
unsigned int Bytecode_Decode (BitBuffer &bitbuffer) {
  unsigned int value = 0;
  unsigned int flag = bitbuffer.ReadBits (1);
  
  if (flag == 0) {
    value = bitbuffer.ReadBits (7);
  }
  else {
    value = bitbuffer.ReadBits (15);
  }
  
  return (value);
}

