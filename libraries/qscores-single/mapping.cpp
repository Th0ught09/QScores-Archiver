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
    \file mapping.cpp
    Mapping functions for QScoresSingle class.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: mapping.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <string>
#include <vector>
#include <ostream>
#include <iostream>
#include <fstream>
#include <cstdlib>  //  exit, EXIT_FAILURE
#include <cassert>

using namespace std;

#include "bitbuffer.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"


/*!
     Convert quality scores string to a vector of integers in the printable ASCII range.
*/
void QScoresSingle::QScoreToInt () {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_STR));
  
  m_QScoreInt.resize (0);  //  Empty the vector first
  for (unsigned int i = 0; i < m_QScoreStr.length (); i++) {
    //  - offset + g_GLOBAL_OFFSET
    unsigned int value = static_cast<unsigned int>(m_QScoreStr[i]);
    m_QScoreInt.push_back (value);
  }

  m_Status = e_QSCORES_SINGLE_STATUS_BOTH;

  return;
}


/*!
     Reverse the changes done by QScoreToInt ().
*/
void QScoresSingle::IntToQScore () {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  m_QScoreStr = "";  //  Empty the string first

  for (unsigned int i = 0; i < m_QScoreInt.size () ; i++) {
    m_QScoreStr += (static_cast<char> (m_QScoreInt[i]));
  }

  m_Status = e_QSCORES_SINGLE_STATUS_BOTH;

  return;
}


/*!
     Convert string of non-printable characters to vector of integers.
*/
void QScoresSingle::StrToInt () {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_STR));

  m_QScoreInt.resize (0);  //  Empty the vector first
  for (unsigned int i = 0; i < m_QScoreStr.length (); i++) {
    unsigned int value = static_cast<unsigned int> (m_QScoreStr[i]);
    m_QScoreInt.push_back (value);
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


/*!
     Check that the vector of integers is valid in terms of range; skips the final termination symbol.

     \param[in] mode Method used for mapping
*/
void QScoresSingle::Validate (e_QSCORES_MAP mode) {
  unsigned int min = 0;
  unsigned int max = 0;

  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  switch (mode) {
    case e_QSCORES_MAP_SANGER:
      min = g_SANGER_MIN;
      max = g_SANGER_MAX;
      break;
    case e_QSCORES_MAP_SOLEXA:
      min = g_SOLEXA_MIN;
      max = g_SOLEXA_MAX;
      break;
    case e_QSCORES_MAP_ILLUMINA:
      min = g_ILLUMINA_MIN;
      max = g_ILLUMINA_MAX;
      break;
    default:
      break;
  }

  //  Skip the final termination symbol
  for (unsigned int i = 0; i < m_QScoreInt.size () - 1; i++) {
    if (((m_QScoreInt[i] - 1) < min) || ((m_QScoreInt[i] - 1) > max)) {
      cerr << "EE\tInvalid conversion of quality score:  [" << m_QScoreStr << "]" << endl;
    }
  }

  return;
}


