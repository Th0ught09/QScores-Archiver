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
    \file qscores-single.cpp
    Constructor and destructor for QScoresSingle class definition .
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: qscores-single.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <vector>
#include <string>
#include <fstream>
#include <climits>
#include <cassert>

#include <iostream>

using namespace std;

#include "bitbuffer.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"


//  -----------------------------------------------------------------
//  Constructors and destructors
//  -----------------------------------------------------------------

/*!
     Default constructor.

*/
QScoresSingle::QScoresSingle ()
  : m_Debug (false),
    m_Status (e_QSCORES_SINGLE_STATUS_UNSET),
    m_Min (UINT_MAX),
    m_Max (0),
    m_QScoreStr (""),
    m_QScoreInt (0)
{
}


/*!
     Constructor that takes one required and one optional argument.
     
     \param[in] x Initial quality scores string
     \param[in] debug Set to true if in debug mode; false by default
*/
QScoresSingle::QScoresSingle (string x, bool debug)
  : m_Debug (debug),
    m_Status (e_QSCORES_SINGLE_STATUS_STR),
    m_Min (UINT_MAX),
    m_Max (0),
    m_QScoreStr (x),
    m_QScoreInt (0)
{
  //  Since the lengths of the string and integer representations are about equal (could be +/-1 due to difference coding and end
  //  terminator), we reserve the space now to minimize reallocation costs
  m_QScoreInt.reserve (x.length () + 1);
}


/*!
     Constructor that takes one required and one optional argument.
     
     \param[in] x Initial quality scores as integers
     \param[in] debug Set to true if in debug mode; false by default
*/
QScoresSingle::QScoresSingle (vector<unsigned int> x, bool debug)
  : m_Debug (debug),
    m_Status (e_QSCORES_SINGLE_STATUS_INT),
    m_Min (UINT_MAX),
    m_Max (0),
    m_QScoreStr (""),
    m_QScoreInt (x)
{
  //  Since the lengths of the string and integer representations are about equal (could be +/-1 due to difference coding and end
  //  terminator), we reserve the space now to minimize reallocation costs
  m_QScoreStr.reserve (x.size () + 1);
}


/*!
     Destructor that takes no arguments
*/
QScoresSingle::~QScoresSingle () {
//   m_QScoreInt.clear ();
//   m_QScoreStr = "";
}


//  -----------------------------------------------------------------
//  Accessors
//  -----------------------------------------------------------------

/*!
     Return m_Debug

     \return Debug mode or not
*/
bool QScoresSingle::GetDebug () const {
  return m_Debug;
}


/*!
     Return m_Status

     \return Status of QScoresSingle (which component is up-to-date)
*/
e_QSCORES_SINGLE_STATUS QScoresSingle::GetStatus () const {
  return m_Status;
}


/*!
     Return m_Min

     \return Minimum value
*/
unsigned int QScoresSingle::GetMin () const {
  return m_Min;
}


/*!
     Return m_Max

     \return Maximum value
*/
unsigned int QScoresSingle::GetMax () const {
  return m_Max;
}


/*!
     Return the length of the string representation

     \return Integral length
*/
unsigned int QScoresSingle::GetQScoreLength () const {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_STR));

  return m_QScoreStr.length ();
}


/*!
     Return the length of the integer representation

     \return Integral length
*/
unsigned int QScoresSingle::GetIntLength () const {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  return m_QScoreInt.size ();
}


/*!
     Get the sum of the integer vector

     \return Sum as an integer
*/
unsigned int QScoresSingle::GetIntSum () {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));
  unsigned int sum = 0;
  
  for (unsigned int i = 0; i < GetIntLength (); i++) {
    sum += m_QScoreInt[i];
  }
  
  return (sum);
}


/*!
     Get the average of the integer vector

     \return Average as a double
*/
double QScoresSingle::GetIntAverage () {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));
  unsigned int sum = GetIntSum ();
  
  return (static_cast<double> (sum) / static_cast<double> (m_QScoreInt.size ()));
}


/*!
     Set m_Min and m_Max
*/
void QScoresSingle::SetMinMax () {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));
  
  m_Min = UINT_MAX;
  m_Max = 0;
  
  for (unsigned int i = 0; i < GetIntLength (); i++) {
    if (m_QScoreInt[i] < m_Min) {
      m_Min = m_QScoreInt[i];
    }
    if (m_QScoreInt[i] > m_Max) {
      m_Max = m_QScoreInt[i];
    }
  }
  
  return;
}

