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

#include <fstream>  //  ostream
#include <vector>
#include <iostream>
#include <cstdlib>  //  EXIT_SUCCESS, EXIT_FAILURE, exit ()

using namespace std;

#include "Huffman_Config.hpp"
#include "common.hpp"
#include "bitbuffer.hpp"
#include "huffman.hpp"
#include "testing.hpp"


/*!
     Show basic information about the program

     \return true if the vectors are the same; false otherwise
*/
bool VectorSame (vector<unsigned int> x, vector<unsigned int> y) {
  if (x.size () != y.size ()) {
    return false;
  }
  
  for (unsigned int i = 0; i < x.size (); i++) {
    if (x[i] != y[i]) {
      cerr << "EE\tMismatch at position " << i << endl;
      return false;
    }
  }
  
  return true;
}


/*!
     Show basic information about the program

     \return Always returns true
*/
bool ShowInfo () {
  cout << "Huffman version " << PROGRAM_VERSION << " compiled on:  " << __DATE__ <<  " (" << __TIME__ << ")" << endl;
  cout << "Git hash:  " << GIT_HASH << endl;

  cout << "II\tShowInfo successful!" << endl;

  return (true);
}


/*!
     Simple example with only one symbol in the alphabet.

     \return Always returns true
*/
bool HuffmanSimple1Example () {
  string str = "tmp.data";  //  Input/output filename
  vector<unsigned int> tmp;
  vector<unsigned int> tmp2;

  //  Generate test data
  for (unsigned int i = 0; i < 11; i++) {
    tmp.push_back (10);
  }

  //  Test encoding
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  Huffman hm_out;
  hm_out.SetDebug ();
  hm_out.UpdateFrequencies (tmp);
  hm_out.EncodeBegin (bitbuff_out);

  cerr << "II\tEncodeMessage ()" << endl;
  hm_out.EncodeMessage (bitbuff_out, tmp);
  hm_out.EncodeFinish (bitbuff_out);
  cout << hm_out << endl;

  bitbuff_out.Finish ();
  cerr << "II\tFinished encoding..." << endl;
  
  //  Test decoding
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  Huffman hm_in;
  hm_in.SetDebug ();
  hm_in.DecodeBegin (bitbuff_in);

  cerr << "II\tDecodeMessage () of length " << hm_in.GetMessageLength() << endl;;
  tmp2 = hm_in.DecodeMessage (bitbuff_in, hm_in.GetMessageLength ());
  hm_in.DecodeFinish (bitbuff_in);
  cout << hm_in << endl;

  bitbuff_in.Finish ();
  cerr << "II\tFinished decoding... " << tmp2.size () << " symbols." << endl;

  //  Compare hm_in with hm_out
  if (!VectorSame (tmp, tmp2)) {
    cerr << "EE\tHuffman coding of Simple1 example unsuccessful!" << endl;
    return (false);
  }

  cerr << "II\tHuffman coding of Simple1 example successful!" << endl;
  return (true);
}


/*!
     Simple example with only two symbols in the alphabet.

     \return Always returns true
*/
bool HuffmanSimple2Example () {
  string str = "tmp.data";  //  Input/output filename
  vector<unsigned int> tmp;
  vector<unsigned int> tmp2;

  //  Generate test data
  for (unsigned int i = 0; i < 11; i++) {
    tmp.push_back (10);
  }
  for (unsigned int i = 0; i < 5; i++) {
    tmp.push_back (90);
  }

  //  Test encoding
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  Huffman hm_out;
  hm_out.SetDebug ();
  hm_out.UpdateFrequencies (tmp);
  hm_out.EncodeBegin (bitbuff_out);

  cerr << "II\tEncodeMessage ()" << endl;
  hm_out.EncodeMessage (bitbuff_out, tmp);
  hm_out.EncodeFinish (bitbuff_out);
  cout << hm_out << endl;

  bitbuff_out.Finish ();
  cerr << "II\tFinished encoding..." << endl;
  
  //  Test decoding
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  Huffman hm_in;
  hm_in.SetDebug ();
  hm_in.DecodeBegin (bitbuff_in);

  cerr << "II\tDecodeMessage () of length " << hm_in.GetMessageLength() << endl;;
  tmp2 = hm_in.DecodeMessage (bitbuff_in, hm_in.GetMessageLength ());
  hm_in.DecodeFinish (bitbuff_in);
  cout << hm_in << endl;

  bitbuff_in.Finish ();
  cerr << "II\tFinished decoding... " << tmp2.size () << " symbols." << endl;

  //  Compare hm_in with hm_out
  if (!VectorSame (tmp, tmp2)) {
    cerr << "EE\tHuffman coding of Simple2 example unsuccessful!" << endl;
    return (false);
  }

  cerr << "II\tHuffman coding of Simple2 example successful!" << endl;
  return (true);
}


/*!
     Simple example with only three symbols in the alphabet.

     \return Always returns true
*/
bool HuffmanSimple3Example () {
  string str = "tmp.data";  //  Input/output filename
  vector<unsigned int> tmp;
  vector<unsigned int> tmp2;

  //  Generate test data
  for (unsigned int i = 0; i < 11; i++) {
    tmp.push_back (10);
  }
  for (unsigned int i = 0; i < 5; i++) {
    tmp.push_back (90);
  }
  for (unsigned int i = 0; i < 7; i++) {
    tmp.push_back (70);
  }

  //  Test encoding
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  Huffman hm_out;
  hm_out.SetDebug ();
  hm_out.UpdateFrequencies (tmp);
  hm_out.EncodeBegin (bitbuff_out);

  cerr << "II\tEncodeMessage ()" << endl;
  hm_out.EncodeMessage (bitbuff_out, tmp);
  hm_out.EncodeFinish (bitbuff_out);
  cout << hm_out << endl;

  bitbuff_out.Finish ();
  cerr << "II\tFinished encoding..." << endl;
  
  //  Test decoding
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  Huffman hm_in;
  hm_in.SetDebug ();
  hm_in.DecodeBegin (bitbuff_in);

  cerr << "II\tDecodeMessage () of length " << hm_in.GetMessageLength() << endl;;
  tmp2 = hm_in.DecodeMessage (bitbuff_in, hm_in.GetMessageLength ());
  hm_in.DecodeFinish (bitbuff_in);
  cout << hm_in << endl;

  bitbuff_in.Finish ();
  cerr << "II\tFinished decoding... " << tmp2.size () << " symbols." << endl;

  //  Compare hm_in with hm_out
  if (!VectorSame (tmp, tmp2)) {
    cerr << "EE\tHuffman coding of Simple3 example unsuccessful!" << endl;
    return (false);
  }

  cerr << "II\tHuffman coding of Simple3 example successful!" << endl;
  return (true);
}


/*!
     Huffman code the example from Compression and Coding Algorithms by Moffat and Turpin [2002], page 68.

     \return Always returns true
*/
bool HuffmanCACAExample () {
  string str = "tmp.data";  //  Input/output filename
  vector<unsigned int> tmp;
  vector<unsigned int> tmp2;

  //  Generate test data
  for (unsigned int i = 0; i < 11; i++) {
    tmp.push_back (10);
  }
  for (unsigned int i = 0; i < 5; i++) {
    tmp.push_back (90);
  }
  for (unsigned int i = 0; i < 7; i++) {
    tmp.push_back (70);
  }
  for (unsigned int i = 0; i < 67; i++) {
    tmp.push_back (50);
  }
  for (unsigned int i = 0; i < 6; i++) {
    tmp.push_back (40);
  }
  for (unsigned int i = 0; i < 4; i++) {
    tmp.push_back (20);
  }

  //  Test encoding
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  Huffman hm_out;
  hm_out.SetDebug ();
  hm_out.UpdateFrequencies (tmp);
  hm_out.EncodeBegin (bitbuff_out);

  cerr << "II\tEncodeMessage ()" << endl;
  hm_out.EncodeMessage (bitbuff_out, tmp);
  hm_out.EncodeFinish (bitbuff_out);
  cout << hm_out << endl;

  bitbuff_out.Finish ();
  cerr << "II\tFinished encoding..." << endl;
  
  //  Test decoding
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  Huffman hm_in;
  hm_in.SetDebug ();
  hm_in.DecodeBegin (bitbuff_in);

  cerr << "II\tDecodeMessage () of length " << hm_in.GetMessageLength() << endl;;
  tmp2 = hm_in.DecodeMessage (bitbuff_in, hm_in.GetMessageLength ());
  hm_in.DecodeFinish (bitbuff_in);
  cout << hm_in << endl;

  bitbuff_in.Finish ();
  cerr << "II\tFinished decoding... " << tmp2.size () << " symbols." << endl;

  //  Compare hm_in with hm_out
  if (!VectorSame (tmp, tmp2)) {
    cerr << "EE\tHuffman coding of CACA example unsuccessful!" << endl;
    return (false);
  }

  cerr << "II\tHuffman coding of CACA example successful!" << endl;
  return (true);
}


/*!
     Huffman code a set of random numbers

     \return Always returns true
*/
bool HuffmanRandom () {
  string str = "tmp.data";  //  Input/output filename
  vector<unsigned int> tmp;
  vector<unsigned int> tmp2;
  
  unsigned long long int seed = time (NULL);
  srand (seed);
  cerr << "II\tSeed:  " << seed << endl;

  //  Generate test data
  for (unsigned int i = 0; i < g_TEST_SIZE; i++) {
    unsigned int pos = (rand () % g_MAX_ASCII) + 1;  //  Avoid a 0 from appearing
    tmp.push_back (pos);
  }

  //  Test encoding
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  Huffman hm_out;
  hm_out.SetDebug ();
  hm_out.UpdateFrequencies (tmp);
  hm_out.DebugCumulativeSum ();
  hm_out.EncodeBegin (bitbuff_out);

  cerr << "II\tEncodeMessage ()" << endl;
  hm_out.EncodeMessage (bitbuff_out, tmp);
  hm_out.EncodeFinish (bitbuff_out);
  cout << hm_out << endl;

  bitbuff_out.Finish ();
  cerr << "II\tFinished encoding..." << endl;
  
  //  Test decoding
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  Huffman hm_in;
  hm_in.SetDebug ();
  hm_in.DecodeBegin (bitbuff_in);

  cerr << "II\tDecodeMessage () of length " << hm_in.GetMessageLength() << endl;;
  tmp2 = hm_in.DecodeMessage (bitbuff_in, hm_in.GetMessageLength ());
  hm_in.DecodeFinish (bitbuff_in);
  cout << hm_in << endl;
    
  bitbuff_in.Finish ();
  cerr << "II\tFinished decoding... " << tmp2.size () << " symbols." << endl;

  //  Compare hm_in with hm_out
  if (!VectorSame (tmp, tmp2)) {
    cerr << "EE\tHuffman coding of random numbers unsuccessful!" << endl;
    return (false);
  }

  cerr << "II\tHuffman coding of random numbers successful!" << endl;
  return (true);
}
  
