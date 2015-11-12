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
    \file lossless.cpp
    Lossless transformation functions.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: lossless.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <climits>

using namespace std;

#include "bitbuffer.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"


//  -----------------------------------------------------------------
//  Transformation functions
//  -----------------------------------------------------------------


/*!
     Apply difference coding to a vector of unsigned integers.  Since the difference will be both positive and negative, they are mapped back to positive numbers.
     
     \param[in] previous The last quality score value in the previous vector (UINT_MAX means "None")
     \return Returns the last value for processing the next vector
*/
unsigned int QScoresSingle::ApplyDifferenceCoding (unsigned int previous) {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));
  vector<int> tmp;
  unsigned int next = m_QScoreInt[m_QScoreInt.size () - 1];
  unsigned int i = 0;

  //  Add the first value on the list
  if (previous == UINT_MAX) {
    tmp.push_back (m_QScoreInt[0]);
  }
  else {
    tmp.push_back (m_QScoreInt[0] - previous);
  }
  
  for (unsigned int i = 1; i < m_QScoreInt.size (); i++) {
    tmp.push_back (m_QScoreInt[i] - m_QScoreInt[i - 1]);
  }
  
  m_QScoreInt.resize (0);  //  Clear m_QScoreInt

  if (previous == UINT_MAX) {
    m_QScoreInt.push_back (tmp[0]);
    i = 1;
  }
  else {
    i = 0;
  }
  
  for (; i < tmp.size (); i++) {
    int value = tmp[i];
    if (value == 0) {
      m_QScoreInt.push_back (0);
    }
    else if (value > 0) {
      //  positive values are in odd positions
      m_QScoreInt.push_back (2 * static_cast<unsigned int> (value) - 1);
    }
    else if (value < 0) {
      //  negative values are in even positions
      m_QScoreInt.push_back (2 * static_cast<unsigned int> (-1 * value));
    }
  }

  //  Add 1 to every value to prevent 0's
  for (i = 0; i < m_QScoreInt.size (); i++) {
    m_QScoreInt[i]++;
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return (next);
}


/*!
     Subtract a constant from the entire vector to make it 1-based.

     \param[in] k The value to subtract off
*/
void QScoresSingle::ApplyRescaling (unsigned int k) {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  for (unsigned int i = 0; i < m_QScoreInt.size (); i++) {
    if (m_QScoreInt[i] < k) {
      cerr << "EE\tThe value (" << k << ") being subtracted from (" << m_QScoreInt[i] << ") is too large." << endl;
      exit (EXIT_FAILURE);
    }
    m_QScoreInt[i] = m_QScoreInt[i] - k + 1;
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


#if 0
/*!
     Perform the final step of shifting every value by 1 so that no 0 appears (i.e., is 1-based).  
     This is the only function that performs this shifting; every other function should ensure 
     that values are 0-based (so that two functions don't end up doing the same job).
*/
void QScoresSingle::ApplyFinalize () {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  for (unsigned int i = 0; i < m_QScoreInt.size (); i++) {
    m_QScoreInt[i] += 1;
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}
#endif


/*!
     Remap quality scores using the given lookup table.  (Function is identical to ApplyLossyRemapping ().)

     \param[in] lookup The lookup table to remap with.  The size of the table should be equal to the size of the ASCII alphabet.
*/
void QScoresSingle::ApplyLosslessRemapping (vector <unsigned int> lookup) {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  for (unsigned int i = 0; i < m_QScoreInt.size (); i++) {
    m_QScoreInt[i] = lookup[m_QScoreInt[i]];
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


//  -----------------------------------------------------------------
//  Reverse transformation functions
//  -----------------------------------------------------------------


/*!
     Reverse the steps performed by ApplyDifferenceCoding ().

     \param[in] previous The last quality score value in the previous vector (UINT_MAX means "None")
     \return Returns the last value for processing the next vector
*/
unsigned int QScoresSingle::UnapplyDifferenceCoding (unsigned int previous) {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));
  vector<int> tmp;
  unsigned int next = 0;
  unsigned int i = 0;

  //  Subtract 1 from every value
  for (i = 0; i < m_QScoreInt.size (); i++) {
    m_QScoreInt[i]--;
  }

  if (previous == UINT_MAX) {
    tmp.push_back (m_QScoreInt[0]);
    i = 1;
  }
  else {
    i = 0;
  }
  
  for (; i < m_QScoreInt.size (); i++) {
    int value = m_QScoreInt[i];
    if (value % 2 != 0) {
      //  odd position means positive value
      tmp.push_back ((static_cast<int> (value) + 1) / 2);
    }
    else {
      //  even position
      tmp.push_back ((static_cast<int> (value) * -1) / 2);
    }      
  }      

  m_QScoreInt.resize (0);  //  Clear m_QScoreInt

  //  Add the first value on the list
  if (previous == UINT_MAX) {
    m_QScoreInt.push_back (tmp[0]);
  }
  else {
    m_QScoreInt.push_back (previous + tmp[0]);
  }
  
  for (unsigned int i = 1; i < tmp.size (); i++) {
    m_QScoreInt.push_back (static_cast<unsigned int> ((m_QScoreInt[i - 1]) + (tmp[i])));
  }
  next = m_QScoreInt[m_QScoreInt.size () - 1];

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return (next);
}


/*!
     Reverse the steps performed by ApplyRescaling ().

     \param[in] k The value to subtract off
*/
void QScoresSingle::UnapplyRescaling (unsigned int k) {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));
  
  for (unsigned int i = 0; i < m_QScoreInt.size (); i++) {
    if (m_QScoreInt[i] > (UINT_MAX - k)) {
      cerr << "EE\tThe value (" << k << ") being added back is too large." << endl;
      exit (EXIT_FAILURE);
    }
    m_QScoreInt[i] = m_QScoreInt[i] + k - 1;
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


#if 0
/*!
     Reverse the steps performed by ApplyFinalize ().
*/
void QScoresSingle::UnapplyFinalize () {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  //  Shift everything back; uncomment the following line to make all the values 1-based
  for (unsigned int i = 0; i < m_QScoreInt.size (); i++) {
    m_QScoreInt[i] -= 1;
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}
#endif


/*!
     Reverse the steps performed by ApplyLosslessRemapping ().

     \param[in] lookup The lookup table to remap with.  The size of the table should be equal to the size of the ASCII alphabet.
*/
void QScoresSingle::UnapplyLosslessRemapping (vector <unsigned int> lookup) {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  for (unsigned int i = 0; i < m_QScoreInt.size (); i++) {
    //  Since the prelude was encoded with 1 added to each value, we need to subtract 1 here
    m_QScoreInt[i] = lookup[m_QScoreInt[i] - 1];
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


