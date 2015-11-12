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
    \file test_sequential.cpp
    Test sequential sequence of numbers for BitIO.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: test_sequential.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <climits>

#include <cstdlib>  //  EXIT_SUCCESS, EXIT_FAILURE
#include <cmath>  //  log

using namespace std;

#include "BitIOConfig.hpp"
#include "bitbuffer.hpp"
#include "bitio-defn.hpp"
#include "math_utils.hpp"
#include "test_sequential.hpp"
#include "test_random.hpp"


/*!
     Apply unary coding to a sequential list of numbers, from 1 to g_TEST_SIZE_SEQUENTIAL.

     \return The program exit condition
*/
int TestUnarySequential () {
  string str = "tmp.data";  //  Input/output filename

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 1; i <= g_TEST_SIZE_SEQUENTIAL; i++) {
    Unary_Encode (bitbuff_out, i);
  }
  bitbuff_out.Finish ();

  unsigned int result = 0;
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (i = 1; i <= g_TEST_SIZE_SEQUENTIAL; i++) {
    result = Unary_Decode (bitbuff_in);
    if (i != result) {
      cerr << "EE\tError:  Mismatch in number (" << i << " : " << result << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tSequential unary coding successful!" << endl;
  return (true);
}


/*!
     Apply binary coding to a sequential list of numbers, from 1 to g_TEST_SIZE_SEQUENTIAL.

     \return The program exit condition
*/
int TestBinarySequential () {
  string str = "tmp.data";  //  Input/output filename

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 1; i <= g_TEST_SIZE_SEQUENTIAL; i++) {
    BinaryHigh_Encode (bitbuff_out, i, g_TEST_SIZE_SEQUENTIAL);
  }
  bitbuff_out.Finish ();

  unsigned int result = 0;
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (i = 1; i <= g_TEST_SIZE_SEQUENTIAL; i++) {
    result = BinaryHigh_Decode (bitbuff_in, g_TEST_SIZE_SEQUENTIAL);
    if (i != result) {
      cerr << "EE\tError:  Mismatch in number (" << i << " : " << result << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tSequential binary coding successful!" << endl;
  return (true);
}


/*!
     Apply gamma coding to a sequential list of numbers, from 1 to g_TEST_SIZE_SEQUENTIAL.

     \return The program exit condition
*/
int TestGammaSequential () {
  string str = "tmp.data";  //  Input/output filename

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 1; i <= g_TEST_SIZE_SEQUENTIAL; i++) {
    Gamma_Encode (bitbuff_out, i);
  }
  bitbuff_out.Finish ();

  unsigned int result = 0;
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (i = 1; i <= g_TEST_SIZE_SEQUENTIAL; i++) {
    result = Gamma_Decode (bitbuff_in);
    if (i != result) {
      cerr << "EE\tError:  Mismatch in number (" << i << " : " << result << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tSequential gamma coding successful!" << endl;
  return (true);
}


/*!
     Apply delta coding to a sequential list of numbers, from 1 to g_TEST_SIZE_SEQUENTIAL.

     \return The program exit condition
*/
int TestDeltaSequential () {
  string str = "tmp.data";  //  Input/output filename

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 1; i <= g_TEST_SIZE_SEQUENTIAL; i++) {
    Delta_Encode (bitbuff_out, i);
  }
  bitbuff_out.Finish ();

  unsigned int result = 0;
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (i = 1; i <= g_TEST_SIZE_SEQUENTIAL; i++) {
    result = Delta_Decode (bitbuff_in);
    if (i != result) {
      cerr << "EE\tError:  Mismatch in number (" << i << " : " << result << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tSequential delta coding successful!" << endl;
  return (true);
}


/*!
     Apply Golomb coding to a sequential list of numbers, from 1 to g_TEST_SIZE_SEQUENTIAL.

     \param b Parameter to Golomb coding
     \return The program exit condition
*/
int TestGolombSequential (unsigned int b) {
  string str = "tmp.data";  //  Input/output filename

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 1; i <= g_TEST_SIZE_SEQUENTIAL; i++) {
    Golomb_Encode (bitbuff_out, i, b);
  }
  bitbuff_out.Finish ();

  unsigned int result = 0;
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (i = 1; i <= g_TEST_SIZE_SEQUENTIAL; i++) {
    result = Golomb_Decode (bitbuff_in, b);
    if (i != result) {
      cerr << "EE\tError:  Mismatch in number (" << i << " : " << result << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tSequential Golomb coding with parameter " << b << " successful!" << endl;
  return (true);
}


/*!
     Apply Rice coding to a sequential list of numbers, from 1 to g_TEST_SIZE_SEQUENTIAL.

     \param k Parameter to Rice coding
     \return The program exit condition
*/
int TestRiceSequential (unsigned int k) {
  string str = "tmp.data";  //  Input/output filename

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 1; i <= g_TEST_SIZE_SEQUENTIAL; i++) {
    Rice_Encode (bitbuff_out, i, k);
  }
  bitbuff_out.Finish ();

  unsigned int result = 0;
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (i = 1; i <= g_TEST_SIZE_SEQUENTIAL; i++) {
    result = Rice_Decode (bitbuff_in, k);
    if (i != result) {
      cerr << "EE\tError:  Mismatch in number (" << i << " : " << result << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tSequential Rice coding with parameter " << k << " successful!" << endl;
  return (true);
}

