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
    \file bytecode.cpp
    File for encoding and decoding bytecodes.

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: bytecode.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/


#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>  //  exit
#include <climits>

using namespace std;

#include "systemcfg.hpp"
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

