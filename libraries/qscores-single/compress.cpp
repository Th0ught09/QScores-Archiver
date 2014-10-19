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
    \file compress.cpp
    Compression functions.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: compress.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include <iostream>
#include <climits>

using namespace std;

#include "bitbuffer.hpp"
#include "bitio-defn.hpp"
#include "interpolative.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "compress.hpp"


//  -----------------------------------------------------------------
//  Compression functions
//  -----------------------------------------------------------------

/*!
     Apply binary coding and output to the binary file

     \param[in] bitbuffer BitBuffer object to output to
     \param[in] param Parameter to Binary coding (largest possible value)
     \param[in] len Length of this read (if 0, then explicitly encode it)
*/
void QScoresSingle::ApplyCompressionBinary (BitBuffer &bitbuffer, unsigned int param, unsigned int len) {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  //  Delta encode the length of the vector if explicitly asked to
  if (len == 0) {
    Delta_Encode (bitbuffer, m_QScoreInt.size ());
  }

  //  Binary encode each element
  for (unsigned int i = 0; i < m_QScoreInt.size (); i++) {
    BinaryHigh_Encode (bitbuffer, m_QScoreInt[i], param);
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


/*!
     Apply gamma coding and output to the binary file

     \param[in] bitbuffer BitBuffer object to output to
     \param[in] len Length of this read (if 0, then explicitly encode it)
*/
void QScoresSingle::ApplyCompressionGamma (BitBuffer &bitbuffer, unsigned int len) {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  //  Delta encode the length of the vector if explicitly asked to
  if (len == 0) {
    Delta_Encode (bitbuffer, m_QScoreInt.size ());
  }

  //  Gamma encode each element
  for (unsigned int i = 0; i < m_QScoreInt.size (); i++) {
    Gamma_Encode (bitbuffer, m_QScoreInt[i]);
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


/*!
     Apply delta coding and output to the binary file

     \param[in] bitbuffer BitBuffer object to output to
     \param[in] len Length of this read (if 0, then explicitly encode it)
*/
void QScoresSingle::ApplyCompressionDelta (BitBuffer &bitbuffer, unsigned int len) {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  //  Delta encode the length of the vector if explicitly asked to
  if (len == 0) {
    Delta_Encode (bitbuffer, m_QScoreInt.size ());
  }
  
  //  Delta encode each element
  for (unsigned int i = 0; i < m_QScoreInt.size (); i++) {
    Delta_Encode (bitbuffer, m_QScoreInt[i]);
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


/*!
     Apply Golomb coding and output to the binary file

     \param[in] bitbuffer BitBuffer object to output to
     \param[in] param Parameter to Golomb coding
     \param[in] len Length of this read (if 0, then explicitly encode it)
*/
void QScoresSingle::ApplyCompressionGolomb (BitBuffer &bitbuffer, unsigned int param, unsigned int len) {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  //  Delta encode the length of the vector if explicitly asked to
  if (len == 0) {
    Delta_Encode (bitbuffer, m_QScoreInt.size ());
  }

  //  Golomb encode each element
  for (unsigned int i = 0; i < m_QScoreInt.size (); i++) {
    Golomb_Encode (bitbuffer, m_QScoreInt[i], param);
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


/*!
     Apply Rice coding and output to the binary file

     \param[in] bitbuffer BitBuffer object to output to
     \param[in] param Parameter to Rice coding
     \param[in] len Length of this read (if 0, then explicitly encode it)
*/
void QScoresSingle::ApplyCompressionRice (BitBuffer &bitbuffer, unsigned int param, unsigned int len) {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  //  Delta encode the length of the vector if explicitly asked to
  if (len == 0) {
    Delta_Encode (bitbuffer, m_QScoreInt.size ());
  }
  
  //  Rice encode each element
  for (unsigned int i = 0; i < m_QScoreInt.size (); i++) {
    Rice_Encode (bitbuffer, m_QScoreInt[i], param);
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


/*!
     Apply interpolative coding and output to the binary file

     \param[in] bitbuffer BitBuffer object to output to
     \param[in] len Length of this read (if 0, then explicitly encode it)
*/
void QScoresSingle::ApplyCompressionInterP (BitBuffer &bitbuffer, unsigned int len) {
  assert ((m_Status == e_QSCORES_SINGLE_STATUS_BOTH) || (m_Status == e_QSCORES_SINGLE_STATUS_INT));

  //  Delta encode the length of the vector if explicitly asked to
  if (len == 0) {
    Delta_Encode (bitbuffer, m_QScoreInt.size ());
  }

  //  Interpolative encode each element
  Interpolative_Encode (bitbuffer, m_QScoreInt);

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


//  -----------------------------------------------------------------
//  Uncompression functions
//  -----------------------------------------------------------------

/*!
     Unapply binary coding from a binary file

     \param[in] bitbuffer BitBuffer object to output to
     \param[in] len Length of this read (if 0, then explicitly decode it)
*/
void QScoresSingle::UnapplyCompressionBinary (BitBuffer &bitbuffer, unsigned int param, unsigned int len) {
  assert (m_Status == e_QSCORES_SINGLE_STATUS_UNSET);
  unsigned int num = 0;

  //  Delta decode the length of the vector if explicitly asked to
  if (len == 0) {
    len = Delta_Decode (bitbuffer);
  }
  
  for (unsigned int i = 0; i < len; i++) {
    num = BinaryHigh_Decode (bitbuffer, param);
    m_QScoreInt.push_back (num);
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


/*!
     Unapply gamma coding from a binary file

     \param[in] bitbuffer BitBuffer object to output to
     \param[in] len Length of this read (if 0, then explicitly decode it)
*/
void QScoresSingle::UnapplyCompressionGamma (BitBuffer &bitbuffer, unsigned int len) {
  assert (m_Status == e_QSCORES_SINGLE_STATUS_UNSET);
  unsigned int num = 0;

  //  Delta decode the length of the vector if explicitly asked to
  if (len == 0) {
    len = Delta_Decode (bitbuffer);
  }
  
  for (unsigned int i = 0; i < len; i++) {
    num = Gamma_Decode (bitbuffer);
    m_QScoreInt.push_back (num);
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


/*!
     Unapply delta coding from a binary file

     \param[in] bitbuffer BitBuffer object to output to
     \param[in] len Length of this read (if 0, then explicitly decode it)
*/
void QScoresSingle::UnapplyCompressionDelta (BitBuffer &bitbuffer, unsigned int len) {
  assert (m_Status == e_QSCORES_SINGLE_STATUS_UNSET);
  unsigned int num = 0;

  //  Delta decode the length of the vector if explicitly asked to
  if (len == 0) {
    len = Delta_Decode (bitbuffer);
  }
  
  for (unsigned int i = 0; i < len; i++) {
    num = Delta_Decode (bitbuffer);

    m_QScoreInt.push_back (num);
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


/*!
     Unapply Golomb coding from a binary file

     \param[in] bitbuffer BitBuffer object to output to
     \param[in] param Golomb coding parameter
     \param[in] len Length of this read (if 0, then explicitly decode it)
*/
void QScoresSingle::UnapplyCompressionGolomb (BitBuffer &bitbuffer, unsigned int param, unsigned int len) {
  assert (m_Status == e_QSCORES_SINGLE_STATUS_UNSET);
  unsigned int num = 0;

  //  Delta decode the length of the vector if explicitly asked to
  if (len == 0) {
    len = Delta_Decode (bitbuffer);
  }
  
  for (unsigned int i = 0; i < len; i++) {
    num = Golomb_Decode (bitbuffer, param);
    m_QScoreInt.push_back (num);
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


/*!
     Unapply Rice coding from a binary file

     \param[in] bitbuffer BitBuffer object to output to
     \param[in] param Rice coding parameter
     \param[in] len Length of this read (if 0, then explicitly decode it)
*/
void QScoresSingle::UnapplyCompressionRice (BitBuffer &bitbuffer, unsigned int param, unsigned int len) {
  assert (m_Status == e_QSCORES_SINGLE_STATUS_UNSET);
  unsigned int num = 0;

  //  Delta decode the length of the vector if explicitly asked to
  if (len == 0) {
    len = Delta_Decode (bitbuffer);
  }
  
  for (unsigned int i = 0; i < len; i++) {
    num = Rice_Decode (bitbuffer, param);
    m_QScoreInt.push_back (num);
  }

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


/*!
     Unapply interpolative coding from a binary file

     \param[in] bitbuffer BitBuffer object to output to
     \param[in] len Length of this read (if 0, then explicitly decode it)
*/
void QScoresSingle::UnapplyCompressionInterP (BitBuffer &bitbuffer, unsigned int len) {
  assert (m_Status == e_QSCORES_SINGLE_STATUS_UNSET);

  unsigned int size = len;
  vector <unsigned int> tmp;

  //  Delta decode the length of the vector if explicitly asked to
  if (len == 0) {
    size = Delta_Decode (bitbuffer);
  }

  //  Interpolative decode each element
  Interpolative_Decode (bitbuffer, tmp, size);

  m_QScoreInt = tmp;

  m_Status = e_QSCORES_SINGLE_STATUS_INT;

  return;
}


