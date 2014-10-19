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
    \file testing.cpp
    Testing functions for BitBuffer.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
    $Id: testing.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <cstdlib>  //  EXIT_SUCCESS, EXIT_FAILURE
#include <cmath>  //  log

using namespace std;

#include "systemcfg.hpp"
#include "BitBufferConfig.hpp"
#include "bitbuffer.hpp"
#include "testing.hpp"


/*!
     Calculate the number of bits required to binary encode a given numberfloor of the log (base 2) of a number.

     \param[in] num The number
     \return Length in bits
*/
int BitLength (int num) {
  return ((static_cast<int>(floor (log (num) / log (2)))) + 1);
}


/*!
     Show basic information about the program

     \return The program exit condition
*/
int ShowInfo () {
  cout << sizeof (unsigned int) << endl;
  cout << sizeof (unsigned long long int) << endl;
  cout << sizeof (double) << endl;

  cout << "Hello, world!" << endl;
  cout << "BitBuffer Version:  " << BitBuffer_VERSION_MAJOR << "." << BitBuffer_VERSION_MINOR << endl;

  cerr << "==\tShowInfo successful!" << endl;
  return (EXIT_SUCCESS);
}


/*!
     Produce, write out, and read in powers of 2 until the total number of bits is just over 32 in length

     \return The program exit condition
*/
int GeneratePowerTwoLimit () {
  string str = "tmp.data";  //  Input/output filename
  unsigned int bits_total = 0;
  vector<int> nums;
  vector<int>::iterator iter;

  //  Generate numbers to the power of 2
  int i = 0;
  while (1) {
    int num = 1 << (i + 1);
    bits_total = bits_total + (BitLength (num));
    if (bits_total > g_UINT_SIZE_BITS) {
      break;
    }
    nums.push_back (num);
    i++;    
  }

  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);

  for (iter = nums.begin(); iter != nums.end(); iter++) {
    int bits = BitLength (*iter);
    bitbuff_out.WriteBits (*iter, bits);
  }
  bitbuff_out.Finish ();

  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);

  for (iter = nums.begin(); iter != nums.end(); iter++) {
    int bits = BitLength (*iter);
    int num = bitbuff_in.ReadBits (bits);
    if (num != *iter) {
      cerr << "==\tError:  Mismatch in number (" << num << " : " << *iter << ")" << endl;
      return (EXIT_FAILURE);
    }
  }
  bitbuff_in.Finish ();

  cerr << "==\tGeneratePowerTwoLimit successful!" << endl;
  return (EXIT_SUCCESS);
}


/*!
     Produce, write out, and read in powers of 2

     \return The program exit condition
*/
int GeneratePowerTwo () {
  string str = "tmp.data";  //  Input/output filename
  vector<int> nums;
  vector<int>::iterator iter;

  //  Generate numbers to the power of 2
  int i = 0;
  for (i = 0; i < g_TEST_SIZE_POW2; i++) {
    int num = 1 << (i + 1);
    nums.push_back (num);
  }

  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);

  for (iter = nums.begin(); iter != nums.end(); iter++) {
    int bits = BitLength (*iter);
    bitbuff_out.WriteBits (*iter, bits);
  }
  bitbuff_out.Finish ();

  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);

  for (iter = nums.begin(); iter != nums.end(); iter++) {
    int bits = BitLength (*iter);
    int num = bitbuff_in.ReadBits (bits);
    if (num != *iter) {
      cerr << "==\tError:  Mismatch in number (" << num << " : " << *iter << ")" << endl;
      return (EXIT_FAILURE);
    }
  }
  bitbuff_in.Finish ();

  cerr << "==\tGeneratePowerTwo successful!" << endl;
  return (EXIT_SUCCESS);
}


/*!
     Produce, write out, and read in random values with a fixed width

     \return The program exit condition
*/
int GenerateFixed () {
  string str = "tmp.data";  //  Input/output filename
  vector<int> nums;
  vector<int>::iterator iter;

  //  Initialize the random seed
  srand (time (NULL));

  //  Generate random numbers in the range [1, g_TEST_RANGE].
  int i = 0;
  for (i = 0; i < g_TEST_SIZE; i++) {
    int num = (rand() % g_TEST_RANGE) + 1;
    nums.push_back (num);
  }

  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);

  int bits = BitLength (g_TEST_RANGE);

  for (i = 0; i < g_TEST_SIZE; i++) {
    bitbuff_out.WriteBits (nums[i], bits);
  }
  bitbuff_out.Finish ();

  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);

  for (iter = nums.begin(); iter != nums.end(); iter++) {
    int num = bitbuff_in.ReadBits (bits);
    if (num != *iter) {
      cerr << "==\tError:  Mismatch in number (" << num << " : " << *iter << ")" << endl;
      return (EXIT_FAILURE);
    }
  }
  bitbuff_in.Finish ();

  cerr << "==\tGenerateFixed successful!" << endl;
  return (EXIT_SUCCESS);
}


/*!
     Produce, write out, and read in random values with a variable width

     \return The program exit condition
*/
int GenerateVariable () {
  string str = "tmp.data";  //  Input/output filename
  vector<int> nums;
  vector<int>::iterator iter;

  //  Initialize the random seed
  srand (time (NULL));

  //  Generate random numbers
  int i = 0;
  for (i = 0; i < g_TEST_SIZE; i++) {
    int num = (rand() % g_TEST_RANGE) + 1;
    nums.push_back (num);
  }

  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);

  for (iter = nums.begin(); iter != nums.end(); iter++) {
    int bits = BitLength (*iter);
    bitbuff_out.WriteBits (*iter, bits);
  }
  bitbuff_out.Finish ();

  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);

  for (iter = nums.begin(); iter != nums.end(); iter++) {
    int bits = BitLength (*iter);
    int num = bitbuff_in.ReadBits (bits);
    if (num != *iter) {
      cerr << "==\tError:  Mismatch in number (" << num << " : " << *iter << ")" << endl;
      return (EXIT_FAILURE);
    }
  }
  bitbuff_in.Finish ();

  cerr << "==\tGenerateVariable successful!" << endl;
  return (EXIT_SUCCESS);
}


/*!
     Produce, write out, and read in random unsigned int values

     \return The program exit condition
*/
int TestUnsignedInts () {
  string str = "tmp.data";  //  Input/output filename
  unsigned int *nums_out = new unsigned int[g_TEST_SIZE];
  unsigned int *nums_in = new unsigned int[g_TEST_SIZE];
  unsigned int count_out = g_TEST_SIZE;
  unsigned int count_in = 0;
  
  //  Initialize the random seed
  srand (time (NULL));

  //  Generate random numbers
  for (unsigned int i = 0; i < count_out; i++) {
    nums_out[i] = (rand() % g_TEST_RANGE) + 1;
  }

  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  bitbuff_out.WriteUInts (&count_out, 1);
  bitbuff_out.WriteUInts (nums_out, count_out);
  bitbuff_out.Finish ();

  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  bitbuff_in.ReadUInts (&count_in, 1);
  bitbuff_in.ReadUInts (nums_in, count_in);
  bitbuff_in.Finish ();

  if (count_out != count_in) {
    cerr << "==\tError:  Mismatch in number (" << count_out << " : " << count_in << ")" << endl;
    return (EXIT_FAILURE);
  }
  
  for (int i = 0; i < g_TEST_SIZE; i++) {
    if (nums_out[i] != nums_in[i]) {
      cerr << "==\tError:  Mismatch in number (" << nums_out[i] << " : " << nums_in[i] << ")" << endl;
      return (EXIT_FAILURE);
    }
  }

  cerr << "==\tTestUnsignedInts successful!" << endl;
  return (EXIT_SUCCESS);
}


/*!
     Produce, write out, and read in random unsigned char values

     \return The program exit condition
*/
int TestUnsignedChars () {
  string str = "tmp.data";  //  Input/output filename
  char *nums_out = new char[g_TEST_SIZE];
  char *nums_in = new char[g_TEST_SIZE];
  unsigned int count_out = g_TEST_SIZE;
  unsigned int count_in = 0;

  //  Initialize the random seed
  srand (time (NULL));

  //  Generate random numbers; unsigned chars, so the range is 0 to 255
  for (unsigned int i = 0; i < count_out; i++) {
    nums_out[i] = rand() % 256;
  }

  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  bitbuff_out.WriteUInts (&count_out, 1);
  bitbuff_out.WriteChars (nums_out, count_out);
  bitbuff_out.Finish ();

  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  bitbuff_in.ReadUInts (&count_in, 1);
  bitbuff_in.ReadChars (nums_in, count_in);
  bitbuff_in.Finish ();

  if (count_out != count_in) {
    cerr << "==\tError:  Mismatch in number (" << count_out << " : " << count_in << ")" << endl;
    return (EXIT_FAILURE);
  }

  for (int i = 0; i < g_TEST_SIZE; i++) {
    if (nums_out[i] != nums_in[i]) {
      cerr << "==\tError:  Mismatch in number (" << nums_out[i] << " : " << nums_in[i] << ")" << endl;
      return (EXIT_FAILURE);
    }
  }

  cerr << "==\tTestUnsignedChars successful!" << endl;
  return (EXIT_SUCCESS);
}
