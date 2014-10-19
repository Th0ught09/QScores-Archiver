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
    Encoding functions for interpolative coding.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: encode.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <climits>

using namespace std;

#include "bitbuffer.hpp"
#include "math_utils.hpp"
#include "bitio-defn.hpp"
#include "encode.hpp"


//  -----------------------------------------------------------------
//  Functions for unsigned ints
//  -----------------------------------------------------------------


/*!
     Encode a vector of unsigned ints using interpolative coding

     \param bitbuffer BitBuffer object where the bits are sent
     \param v Vector of values to encode
*/
void Interpolative_Encode (BitBuffer &bitbuffer, const vector <unsigned int>& v) {
  unsigned int v_size = static_cast<unsigned int>(v.size ());
  vector <unsigned int> v_tmp;

  //  Calculate the cumulative sum
  v_tmp.resize (v_size);
  v_tmp[0] = v[0];
  for (unsigned int i = 1; i < v_size; i++) {
    v_tmp[i] = v_tmp[i - 1] + v[i];
  }

  //  Encode the lower and upper bound limits
  Gamma_Encode (bitbuffer, v_tmp[0]);
  Gamma_Encode (bitbuffer, v_tmp[v_size - 1]);

  //  Begin to recursively process list
  Recursive_Interpolative_Encode (bitbuffer, v_tmp, v_size, 0, v_size - 1, v_tmp[0], v_tmp[v_size - 1]);

  return;
}


/*!
     Recursively encode a vector of unsigned ints using interpolative coding

     \param bitbuffer BitBuffer object where the bits are sent
     \param list Vector of values to encode
     \param list_size Size of the list to be coded defined by the boundary (i.e., not the entire list)
     \param left Left (lower) boundary of vector
     \param right Right (upper) boundary of vector
     \param left_value Value for the left boundary of vector
     \param right_value Value for the right boundary of vector

     Note that left_value and right_value depends on the value that was just encoded and not the actual
     values in the list (since these values have not been sent to the encoder and are not yet known).

     Unlike the Compression and Coding Algorithms implementation [pg. 43], a normal binary code
     is used and not a centered (which would have improved compression effectiveness slightly).
*/
void Recursive_Interpolative_Encode (BitBuffer &bitbuffer, const vector <unsigned int>& list, unsigned int list_size, unsigned int left, unsigned int right, unsigned int left_value, unsigned int right_value) {
  unsigned int midpoint = 0;
  unsigned int first_list_size = 0;
  unsigned int second_list_size = 0;
  
  if (list_size == 0) {
    return;
  }

  //  0-based vector, so adjust these values accordingly
  midpoint = ((list_size + 1) >> 1) - 1 + left;
  first_list_size = midpoint - left;
  second_list_size = right - midpoint;

  //  Encode the middle value
  BinaryLowHigh_Encode (bitbuffer, list[midpoint], left_value + first_list_size, right_value - second_list_size);
  
  //  Recurse on both halves
  Recursive_Interpolative_Encode (bitbuffer, list, first_list_size, left, midpoint - 1, left_value, list[midpoint] - 1);
  Recursive_Interpolative_Encode (bitbuffer, list, second_list_size, midpoint + 1, right, list[midpoint] + 1, right_value);

  return;
}

