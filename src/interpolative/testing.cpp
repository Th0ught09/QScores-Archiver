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
    \file testing.cpp
    Testing functions of main-test.
*/
/*******************************************************************/

#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>  //  EXIT_SUCCESS, EXIT_FAILURE
#include <climits>  //  UINT_MAX

using namespace std;

//  Pull the configuration file in
#include "Interpolative_Config.hpp"

#include "bitbuffer.hpp"
#include "bitio-defn.hpp"
#include "math_utils.hpp"
#include "encode.hpp"
#include "decode.hpp"
#include "testing.hpp"


/*!
     Show basic information about the program

     \return Always returns true
*/
bool ShowInfo () {
  cout << "Interpolative coding version " << INTERPOLATIVE_PROGRAM_VERSION << " compiled on:  " << __DATE__ <<  " (" << __TIME__ << ")" << endl;
  cout << "Git hash:  " << INTERPOLATIVE_GIT_HASH << endl;

  cout << "II\tShowInfo successful!" << endl;

  return (true);
}


/*!
     Interpolative code the example from Compression and Coding Algorithms by Moffat and Turpin [2002], page 42.

     \return Always returns true
*/
bool InterpolativeCodeCACAExample () {
  string str = "tmp.data";  //  Input/output filename
  vector<unsigned int> out_nums;
  vector<unsigned int> in_nums;
  unsigned int size = 0;

  out_nums.push_back (1);
  out_nums.push_back (1);
  out_nums.push_back (1);
  out_nums.push_back (2);
  out_nums.push_back (2);
  out_nums.push_back (2);
  out_nums.push_back (2);
  out_nums.push_back (4);
  out_nums.push_back (3);
  out_nums.push_back (1);
  out_nums.push_back (1);
  out_nums.push_back (1);

  size = out_nums.size ();
  in_nums.resize (size);
  
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  Interpolative_Encode (bitbuff_out, out_nums);
  bitbuff_out.Finish ();
  cerr << "II\tFinished encoding..." << endl;
  
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  Interpolative_Decode (bitbuff_in, in_nums, size);
  bitbuff_in.Finish ();
  cerr << "II\tFinished decoding..." << endl;

  //  Verify numbers
  for (unsigned int i = 0; i < out_nums.size (); i++) {
    cerr << "II\t" << i << "\t" << out_nums[i] << "\t" << in_nums[i] << "\t" << endl;
    if (out_nums[i] != in_nums[i]) {
      cerr << "EE\tError:  Mismatch in number (" << out_nums[i] << " : " << in_nums[i] << ")" << endl;
      return (false);
    }
  }

  cerr << "II\tInterpolative coding of CACA example successful!" << endl;
  return (true);
}


/*!
     Interpolative code a set of random numbers

     \return Always returns true
*/
bool InterpolativeCodeRandom () {
  string str = "tmp.data";  //  Input/output filename
  vector<unsigned int> out_nums;
  vector<unsigned int> in_nums;
  unsigned int size = 0;

  //  Initialize the random seed
  srand (time (NULL));

  for (unsigned int i = 0; i < g_TEST_SIZE; i++) {
    unsigned int num = (rand() % g_TEST_RANGE) + 1;
    out_nums.push_back (num);
  }

  size = out_nums.size ();
  in_nums.resize (size);

  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  Interpolative_Encode (bitbuff_out, out_nums);
  bitbuff_out.Finish ();
  cerr << "II\tFinished encoding..." << endl;

  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  Interpolative_Decode (bitbuff_in, in_nums, size);
  bitbuff_in.Finish ();
  cerr << "II\tFinished decoding..." << endl;

  //  Verify numbers
  for (unsigned int i = 0; i < out_nums.size (); i++) {
    cerr << "II\t" << i << "\t" << out_nums[i] << "\t" << in_nums[i] << "\t" << endl;
    if (out_nums[i] != in_nums[i]) {
      cerr << "EE\tError:  Mismatch in number (" << out_nums[i] << " : " << in_nums[i] << ")" << endl;
      return (false);
    }
  }

  cerr << "II\tInterpolative coding of random numbers successful!" << endl;
  return (true);
}
