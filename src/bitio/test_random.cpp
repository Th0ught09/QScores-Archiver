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
    \file test_random.cpp
    Test random sequence of numbers for BitIO.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: test_random.cpp 224 2011-11-06 15:32:09Z rwan $

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
#include "test_random.hpp"


/*!
     Apply unary coding to a random list of g_TEST_SIZE numbers.

     \return The program exit condition
*/
int TestUnaryRandom () {
  string str = "tmp.data";  //  Input/output filename
  vector<unsigned int> nums;
  vector<unsigned int>::iterator iter;

  //  Initialize the random seed
  srand (time (NULL));

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 0; i < g_TEST_SIZE; i++) {
    int num = (rand() % g_TEST_RANGE) + 1;
    nums.push_back (num);
    Unary_Encode (bitbuff_out, num);
  }
  bitbuff_out.Finish ();

  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (iter = nums.begin(); iter != nums.end(); iter++) {
    unsigned int num = Unary_Decode (bitbuff_in);
    if (num != *iter) {
      cerr << "EE\tError:  Mismatch in number (" << num << " : " << *iter << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();
  
  cerr << "II\tRandom unary coding successful!" << endl;
  return (true);
}


/*!
     Apply binary coding to a random list of g_TEST_SIZE numbers.

     \return The program exit condition
*/
int TestBinaryRandom () {
  string str = "tmp.data";  //  Input/output filename
  vector<unsigned int> nums;
  vector<unsigned int>::iterator iter;

  //  Initialize the random seed
  srand (time (NULL));

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 0; i < g_TEST_SIZE; i++) {
    int num = (rand() % g_TEST_RANGE) + 1;
    nums.push_back (num);
    BinaryHigh_Encode (bitbuff_out, num, g_TEST_RANGE);
  }
  bitbuff_out.Finish ();

  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (iter = nums.begin(); iter != nums.end(); iter++) {
    unsigned int num = BinaryHigh_Decode (bitbuff_in, g_TEST_RANGE);
    if (num != *iter) {
      cerr << "EE\tError:  Mismatch in number (" << num << " : " << *iter << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tRandom binary coding successful!" << endl;
  return (true);
}


/*!
     Apply gamma coding to a random list of g_TEST_SIZE numbers.

     \return The program exit condition
*/
int TestGammaRandom () {
  string str = "tmp.data";  //  Input/output filename
  vector<unsigned int> nums;
  vector<unsigned int>::iterator iter;

  //  Initialize the random seed
  srand (time (NULL));

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 0; i < g_TEST_SIZE; i++) {
    int num = (rand() % g_TEST_RANGE) + 1;
    nums.push_back (num);
    Gamma_Encode (bitbuff_out, num);
  }
  bitbuff_out.Finish ();

  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (iter = nums.begin(); iter != nums.end(); iter++) {
    unsigned int num = Gamma_Decode (bitbuff_in);
    if (num != *iter) {
      cerr << "EE\tError:  Mismatch in number (" << num << " : " << *iter << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tRandom gamma coding successful!" << endl;
  return (true);
}


/*!
     Apply delta coding to a random list of g_TEST_SIZE numbers.

     \return The program exit condition
*/
int TestDeltaRandom () {
  string str = "tmp.data";  //  Input/output filename
  vector<unsigned int> nums;
  vector<unsigned int>::iterator iter;

  //  Initialize the random seed
  srand (time (NULL));

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 0; i < g_TEST_SIZE; i++) {
    int num = (rand() % g_TEST_RANGE) + 1;
    nums.push_back (num);
    Delta_Encode (bitbuff_out, num);
  }
  bitbuff_out.Finish ();

  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (iter = nums.begin(); iter != nums.end(); iter++) {
    unsigned int num = Delta_Decode (bitbuff_in);
    if (num != *iter) {
      cerr << "EE\tError:  Mismatch in number (" << num << " : " << *iter << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tRandom delta coding successful!" << endl;
  return (true);
}


/*!
     Apply Golomb coding to a random list of g_TEST_SIZE numbers.

     \param b Parameter to Golomb coding
     \return The program exit condition
*/
int TestGolombRandom (unsigned int b) {
  string str = "tmp.data";  //  Input/output filename
  vector<unsigned int> nums;
  vector<unsigned int>::iterator iter;

  //  Initialize the random seed
  srand (time (NULL));

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 0; i < g_TEST_SIZE; i++) {
    int num = (rand() % g_TEST_RANGE) + 1;
    nums.push_back (num);
    Golomb_Encode (bitbuff_out, num, b);
  }
  bitbuff_out.Finish ();

  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (iter = nums.begin(); iter != nums.end(); iter++) {
    unsigned int num = Golomb_Decode (bitbuff_in, b);
    if (num != *iter) {
      cerr << "EE\tError:  Mismatch in number (" << num << " : " << *iter << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tRandom Golomb coding with parameter " << b << " successful!" << endl;
  return (true);
}


/*!
     Apply Rice coding to a random list of g_TEST_SIZE numbers.

     \param k Parameter to Rice coding
     \return The program exit condition
*/
int TestRiceRandom (unsigned int k) {
  string str = "tmp.data";  //  Input/output filename
  vector<unsigned int> nums;
  vector<unsigned int>::iterator iter;

  //  Initialize the random seed
  srand (time (NULL));

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 0; i < g_TEST_SIZE; i++) {
    int num = (rand() % g_TEST_RANGE) + 1;
    nums.push_back (num);
    Rice_Encode (bitbuff_out, num, k);
  }
  bitbuff_out.Finish ();

  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (iter = nums.begin(); iter != nums.end(); iter++) {
    unsigned int num = Rice_Decode (bitbuff_in, k);
    if (num != *iter) {
      cerr << "EE\tError:  Mismatch in number (" << num << " : " << *iter << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tRandom Rice coding with parameter " << k << " successful!" << endl;
  return (true);
}
