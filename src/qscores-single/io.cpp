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
    \file io.cpp
    IO functions for QScoresSingle class.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: io.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <string>
#include <vector>
#include <ostream>
#include <iostream>
#include <fstream>
#include <cstdlib>  //  exit, EXIT_FAILURE
#include <cassert>
#include <sstream>  //  String stream

using namespace std;

#include "bitbuffer.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "debug.hpp"


//  -----------------------------------------------------------------
//  Friends
//  -----------------------------------------------------------------


/*!
     Overloaded << operator defined as a friend of QScoresSingle
     to print the quality scores as a string.

     \param[in] os Output stream
     \param[in] qs QScoresSingle passed as reference
     \return Output stream
*/
ostream &operator<< (ostream &os, const QScoresSingle& qs) {
  os << qs.GetQScoreStr ();

  return os;
}


//  -----------------------------------------------------------------
//  Private functions for printing
//  -----------------------------------------------------------------


/*!
     Convert an unsigned integer to a string

     \return String representation of unsigned integer
*/
string QScoresSingle::ConvertUInt (unsigned int num) {
   stringstream ss;  //  Create a stringstream
   ss << num;  //  Add number to the stream
   return ss.str();  //  Return a string with the contents of the stream
}


/*!
     Debugging function to print quality scores [strings]

     \return String representation of quality scores
*/
string QScoresSingle::GetQScoreStr () const {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_STR));

  return (m_QScoreStr);
}


/*!
     Debugging function to print quality scores [integers]

     \return String representation of integers
*/
string QScoresSingle::GetQScoreIntAsStr () {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  string result = "";
  unsigned int len = GetIntLength ();

  for (unsigned int i = 0; i < (len - 1); i++) {
    result = result + ConvertUInt (m_QScoreInt[i]) + " ";
  }
  result = result + ConvertUInt (m_QScoreInt[len - 1]);

  return result;
}


//  -----------------------------------------------------------------
//  Public functions for printing
//  -----------------------------------------------------------------


/*!
     Debugging function to print both representations of the quality scores out
*/
void QScoresSingle::PrintQScore () {
  if (GetStatus () == e_QSCORES_SINGLE_STATUS_UNSET) {
    cerr << "EE\tQScoresSingle status not set!" << endl;
  }
  else if (GetStatus () == e_QSCORES_SINGLE_STATUS_STR) {
    cerr << "[" << GetQScoreStr () << "]" << endl;
  }
  else if (GetStatus () == e_QSCORES_SINGLE_STATUS_INT) {
    cerr << "(" << GetQScoreIntAsStr () << ")" << endl;
  }
  else if (GetStatus () == e_QSCORES_SINGLE_STATUS_BOTH) {
    cerr << "[" << GetQScoreStr () << "]" << "\t";
    cerr << "(" << GetQScoreIntAsStr () << ")" << endl;
  }

  return;
}


/*!
     Print the integer representations of the quality scores out as-is

     \return Integer representation of quality scores
*/
vector<unsigned int> QScoresSingle::GetQScoreInt () const {
  return m_QScoreInt;
}


/*!
     Print the integer representations of the quality scores out in binary

     \return String representation of integers
*/
unsigned int QScoresSingle::GetQScoreIntAsBinary (char* buffer, unsigned int buffer_size) {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  unsigned int len = GetIntLength ();
  if (len > buffer_size) {
    cerr << "EE\tBuffer size too small in QScoresSingle::GetQScoreIntAsBinary ()." << endl;
    exit (EXIT_FAILURE);
  }

  for (unsigned int i = 0; i < len; i++) {
    buffer[i] = static_cast<char> (m_QScoreInt[i]);
  }

  return (len);
}


#if 0
/*!
     Print the integer representations of the quality scores out in binary

     \return String representation of integers
*/
unsigned char *QScoresSingle::GetQScoreIntBinary () {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  unsigned int len = GetIntLength ();
  unsigned char *result = (unsigned char*) malloc (sizeof (unsigned char) * len);

  for (unsigned int i = 0; i < len; i++) {
    result[i] = static_cast<unsigned char> (m_QScoreInt[i]);
  }

  return (result);
}


#endif
