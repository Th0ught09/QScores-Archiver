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
    \file mutators.cpp
    Mutating member functions for QScoresSettings class definition .

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: mutators.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <fstream>
#include <iostream>  //  cerr
#include <ostream>
#include <climits>

using namespace std;

#include "bitbuffer.hpp"
#include "bitio-defn.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-settings.hpp"


//  -----------------------------------------------------------------
//  General options
//  -----------------------------------------------------------------

/*!
     Indicate debugging output is required.
*/
void QScoresSettings::SetDebug () {
  m_Debug = true;
  return;
}


/*!
     Set the input filename.
     
     \param[in] x Input string value
*/
void QScoresSettings::SetInputFn (string x) {
  m_InputFn = x;
  return;
}


/*!
     Set the output filename.
     
     \param[in] x Output string value
*/
void QScoresSettings::SetOutputFn (string x) {
  m_OutputFn = x;
  return;
}


/*!
     Set the quality scores mapping method to use.

     \param[in] x Output string value
     \return Boolean value for success/failure.
*/
bool QScoresSettings::SetQScoresMapping (string x) {
  if (x == "sanger") {
    m_Mapping = e_QSCORES_MAP_SANGER;
  }
  else if (x == "solexa") {
    m_Mapping = e_QSCORES_MAP_SOLEXA;
  }
  else if (x == "illumina") {
    m_Mapping = e_QSCORES_MAP_ILLUMINA;
  }
  else {
    cerr << "EE\tThe mapping method " << x << " with the --mapping option is invalid." << endl;
    return false;
  }

  return true;
}

//  -----------------------------------------------------------------
//  Lossy transformation options
//  -----------------------------------------------------------------

/*!
     Indicate that range truncation on the lower bound is required.
*/
void QScoresSettings::SetLossyMinTruncation () {
  m_LossyMinTruncation = true;
  return;
}


/*!
     Indicate that range truncation on the upper bound is required.
*/
void QScoresSettings::SetLossyMaxTruncation () {
  m_LossyMaxTruncation = true;
  return;
}


/*!
     Indicate that logarithmic binning is needed.
*/
void QScoresSettings::SetLossyLogBinning () {
  m_LossyLogBinning = true;
  return;
}


/*!
     Indicate that uniform binning is needed.
*/
void QScoresSettings::SetLossyUniBinning () {
  m_LossyUniBinning = true;
  return;
}


/*!
     Indicate that range truncation on the lower bound is required.

     \param[in] param Minimum value to truncate to.
*/
void QScoresSettings::SetLossyMinTruncationParameter (int param) {
  m_LossyMinTruncationParameter = param;
  return;
}


/*!
     Indicate that range truncation on the upper bound is required.

     \param[in] param Maximum value to truncate to.
*/
void QScoresSettings::SetLossyMaxTruncationParameter (int param) {
  m_LossyMaxTruncationParameter = param;
  return;
}


/*!
     Indicate the gaps for logarithmic binning.

     \param[in] param Gap size.
*/
void QScoresSettings::SetLossyLogBinningParameter (int param) {
  m_LossyLogBinningParameter = param;
  return;
}


/*!
     Indicate the gaps for uniform binning.

     \param[in] param Gap size.
*/
void QScoresSettings::SetLossyUniBinningParameter (int param) {
  m_LossyUniBinningParameter = param;
  return;
}

//  -----------------------------------------------------------------
//  Lossless transformation options
//  -----------------------------------------------------------------


/*!
     Indicate gap transform is required.
*/
void QScoresSettings::SetTransformGapTrans () {
  m_TransformGapTrans = true;
  return;
}


/*!
     Indicate minimum shifting is required.
*/
void QScoresSettings::SetTransformMinShift () {
  m_TransformMinShift = true;
  return;
}


/*!
     Indicate frequency-based reordering is required.
*/
void QScoresSettings::SetTransformFreqOrder () {
  m_TransformFreqOrder = true;
  return;
}


//  -----------------------------------------------------------------
//  Compression options
//  -----------------------------------------------------------------

/*!
     Indicate that Binary coding is used.
*/
void QScoresSettings::SetCompressionBinary () {
  m_CompressionBinary = true;
  return;
}


/*!
     Indicate that Gamma coding is used.
*/
void QScoresSettings::SetCompressionGamma () {
  m_CompressionGamma = true;
  return;
}


/*!
     Indicate that Delta coding is used.
*/
void QScoresSettings::SetCompressionDelta () {
  m_CompressionDelta = true;
  return;
}


/*!
     Indicate that Golomb coding is used.
*/
void QScoresSettings::SetCompressionGolomb () {
  m_CompressionGolomb = true;
  return;
}


/*!
     Indicate that Rice coding is used.
*/
void QScoresSettings::SetCompressionRice () {
  m_CompressionRice = true;
  return;
}


/*!
     Indicate that interpolative coding is used.
*/
void QScoresSettings::SetCompressionInterP () {
  m_CompressionInterP = true;
  return;
}


/*!
     Indicate that Huffman coding is used.
*/
void QScoresSettings::SetCompressionHuffman () {
  m_CompressionHuffman = true;
  return;
}


/*!
     Indicate that arithmetic coding is used.
*/
void QScoresSettings::SetCompressionArithmetic () {
  m_CompressionArithmetic = true;
  return;
}


/*!
     Indicate that gzip is used.
*/
void QScoresSettings::SetCompressionGzip () {
  m_CompressionGzip = true;
  return;
}


/*!
     Indicate that bzip is used.
*/
void QScoresSettings::SetCompressionBzip () {
  m_CompressionBzip = true;
  return;
}


/*!
     Indicate that Re-Pair is used.
*/
void QScoresSettings::SetCompressionRepair () {
  m_CompressionRepair = true;
  return;
}


/*!
     Indicate that PPM is used.
*/
void QScoresSettings::SetCompressionPPM () {
  m_CompressionPPM = true;
  return;
}


/*!
     Indicate that no compression is used.
*/
void QScoresSettings::SetCompressionNone () {
  m_CompressionNone = true;
  return;
}


/*!
     Set the global parameter for Golomb or Rice coding

     \param[in] x Parameter
*/
void QScoresSettings::SetCompressionGlobalParameter (unsigned int x) {
  m_CompressionGlobalParameter = x;
  return;
}


