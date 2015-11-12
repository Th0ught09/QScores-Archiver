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
          
    $Id: test_showlengths.cpp 224 2011-11-06 15:32:09Z rwan $

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
     Apply unary coding to show the bit lengths.

     \param[in] showlengths The maximum value to print up to
     \return The program exit condition
*/
int TestUnaryShowLengths (unsigned int showlengths) {
  string str = "tmp.data";  //  Input/output filename

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 1; i <= showlengths; i++) {
    unsigned int len = Unary_Encode (bitbuff_out, i);
    cout << i << "\t" << len << endl;
  }
  bitbuff_out.Finish ();

  unsigned int result = 0;
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (i = 1; i <= showlengths; i++) {
    result = Unary_Decode (bitbuff_in);
    if (i != result) {
      cerr << "EE\tError:  Mismatch in number (" << i << " : " << result << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tShow-lengths of unary coding successful!" << endl;
  return (true);
}


/*!
     Apply binary coding to show the bit lengths.

     \param[in] showlengths The maximum value to print up to
     \return The program exit condition
*/
int TestBinaryShowLengths (unsigned int showlengths) {
  string str = "tmp.data";  //  Input/output filename

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 1; i <= showlengths; i++) {
    unsigned int len = BinaryHigh_Encode (bitbuff_out, i, showlengths);
    cout << i << "\t" << len << endl;
  }
  bitbuff_out.Finish ();

  unsigned int result = 0;
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (i = 1; i <= showlengths; i++) {
    result = BinaryHigh_Decode (bitbuff_in, showlengths);
    if (i != result) {
      cerr << "EE\tError:  Mismatch in number (" << i << " : " << result << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tShow-lengths of binary coding successful!" << endl;
  return (true);
}


/*!
     Apply gamma coding to show the bit lengths.

     \param[in] showlengths The maximum value to print up to
     \return The program exit condition
*/
int TestGammaShowLengths (unsigned int showlengths) {
  string str = "tmp.data";  //  Input/output filename

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 1; i <= showlengths; i++) {
    unsigned int len = Gamma_Encode (bitbuff_out, i);
    cout << i << "\t" << len << endl;
  }
  bitbuff_out.Finish ();

  unsigned int result = 0;
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (i = 1; i <= showlengths; i++) {
    result = Gamma_Decode (bitbuff_in);
    if (i != result) {
      cerr << "EE\tError:  Mismatch in number (" << i << " : " << result << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tShow-lengths of gamma coding successful!" << endl;
  return (true);
}


/*!
     Apply delta coding to show the bit lengths.

     \param[in] showlengths The maximum value to print up to
     \return The program exit condition
*/
int TestDeltaShowLengths (unsigned int showlengths) {
  string str = "tmp.data";  //  Input/output filename

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 1; i <= showlengths; i++) {
    unsigned int len = Delta_Encode (bitbuff_out, i);
    cout << i << "\t" << len << endl;
  }
  bitbuff_out.Finish ();

  unsigned int result = 0;
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (i = 1; i <= showlengths; i++) {
    result = Delta_Decode (bitbuff_in);
    if (i != result) {
      cerr << "EE\tError:  Mismatch in number (" << i << " : " << result << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tShow-lengths of delta coding successful!" << endl;
  return (true);
}


/*!
     Apply Golomb coding to show the bit lengths.

     \param b Parameter for Golomb coding
     \param[in] showlengths The maximum value to print up to
     \return The program exit condition
*/
int TestGolombShowLengths (unsigned int b, unsigned int showlengths) {
  string str = "tmp.data";  //  Input/output filename

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 1; i <= showlengths; i++) {
    unsigned int len = Golomb_Encode (bitbuff_out, i, b);
    cout << i << "\t" << len << endl;
  }
  bitbuff_out.Finish ();

  unsigned int result = 0;
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (i = 1; i <= showlengths; i++) {
    result = Golomb_Decode (bitbuff_in, b);
    if (i != result) {
      cerr << "EE\tError:  Mismatch in number (" << i << " : " << result << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tShow-lengths of Golomb coding successful!" << endl;
  return (true);
}


/*!
     Apply Rice coding to show the bit lengths.

     \param k Parameter for Rice coding
     \param[in] showlengths The maximum value to print up to
     \return The program exit condition
*/
int TestRiceShowLengths (unsigned int k, unsigned int showlengths) {
  string str = "tmp.data";  //  Input/output filename

  unsigned int i = 0;
  BitBuffer bitbuff_out;
  bitbuff_out.Initialize (str, e_MODE_WRITE);
  for (i = 1; i <= showlengths; i++) {
    unsigned int len = Rice_Encode (bitbuff_out, i, k);
    cout << i << "\t" << len << endl;
  }
  bitbuff_out.Finish ();

  unsigned int result = 0;
  BitBuffer bitbuff_in;
  bitbuff_in.Initialize (str, e_MODE_READ);
  for (i = 1; i <= showlengths; i++) {
    result = Rice_Decode (bitbuff_in, k);
    if (i != result) {
      cerr << "EE\tError:  Mismatch in number (" << i << " : " << result << ")" << endl;
      return (false);
    }
  }
  bitbuff_in.Finish ();

  cerr << "II\tShow-lengths of Rice coding successful!" << endl;
  return (true);
}

