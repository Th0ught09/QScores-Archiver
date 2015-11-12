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
    \file accessors.cpp
    Accessing member functions for QScoresSettings class definition.

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: accessors.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <fstream>
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
     Get the debug setting.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetDebug () const {
  return (m_Debug);
}


/*!
     Get the input filename.
     
     \return String representing the setting.
*/
string QScoresSettings::GetInputFn () const {
  return (m_InputFn);
}


/*!
     Get the output filename.
     
     \return String representing the setting.
*/
string QScoresSettings::GetOutputFn () const {
  return (m_OutputFn);
}


/*!
     Get the mapping method as an enumerated type.

     \return Enumerated type representing the setting
*/
enum e_QSCORES_MAP QScoresSettings::GetQScoresMapping () const {
  return (m_Mapping);
}


/*!
     Get the mapping method as a string.

     \return String representing the setting.
*/
string QScoresSettings::GetQScoresMappingStr () const {
  string result;

  switch (m_Mapping) {
    case e_QSCORES_MAP_SANGER:  result = "Sanger";
                           break;
    case e_QSCORES_MAP_SOLEXA:  result = "Solexa";
                           break;
    case e_QSCORES_MAP_ILLUMINA:  result = "Illumina";
                             break;
    default:  result = "Unset";
              break;
  }

  return (result);
}


/*!
     Get the minimum quality score for the selected mapping method.

     \return The minimum.
*/
int QScoresSettings::GetQScoresMappingMin () const {
  int result = 0;
  
  switch (m_Mapping) {
    case e_QSCORES_MAP_SANGER:  result = g_SANGER_MIN;
                           break;
    case e_QSCORES_MAP_SOLEXA:  result = g_SOLEXA_MIN;
                           break;
    case e_QSCORES_MAP_ILLUMINA:  result = g_ILLUMINA_MIN;
                             break;
    default:  result = UINT_MAX;
              break;
  }

  return (result);
}


/*!
     Get the maximum quality score for the selected mapping method.

     \return The maximum.
*/
int QScoresSettings::GetQScoresMappingMax () const {
  int result = 0;
  
  switch (m_Mapping) {
    case e_QSCORES_MAP_SANGER:  result = g_SANGER_MAX;
                           break;
    case e_QSCORES_MAP_SOLEXA:  result = g_SOLEXA_MAX;
                           break;
    case e_QSCORES_MAP_ILLUMINA:  result = g_ILLUMINA_MAX;
                             break;
    default:  result = UINT_MAX;
              break;
  }

  return (result);
}


/*!
     Get the range of quality scores for the selected mapping method.

     \return The range.
*/
int QScoresSettings::GetQScoresMappingRange () const {
  int result = 0;
  
  switch (m_Mapping) {
    case e_QSCORES_MAP_SANGER:  result = g_SANGER_MAX - g_SANGER_MIN;
                           break;
    case e_QSCORES_MAP_SOLEXA:  result = g_SOLEXA_MAX - g_SOLEXA_MIN;
                           break;
    case e_QSCORES_MAP_ILLUMINA:  result = g_ILLUMINA_MAX - g_ILLUMINA_MIN;
                             break;
    default:  result = UINT_MAX;
              break;
  }

  return (result);
}


//  -----------------------------------------------------------------
//  Lossy transformation options
//  -----------------------------------------------------------------


/*!
     Get whether minimum value truncating is being done.

     \return Boolean indicating value.
*/
bool QScoresSettings::GetLossyMinTruncation () const {
  return (m_LossyMinTruncation);
}


/*!
     Get whether maximum value truncating is being done.

     \return Boolean indicating value.
*/
bool QScoresSettings::GetLossyMaxTruncation () const {
  return (m_LossyMaxTruncation);
}


/*!
     Get whether logarithmic binning is being done.

     \return Boolean indicating value.
*/
bool QScoresSettings::GetLossyLogBinning () const {
  return (m_LossyLogBinning);
}


/*!
     Get whether uniform binning is being done.

     \return Boolean indicating value.
*/
bool QScoresSettings::GetLossyUniBinning () const {
  return (m_LossyUniBinning);
}


/*!
     Get the minimum value we are truncating to.

     \return Value as an unsigned integer.
*/
int QScoresSettings::GetLossyMinTruncationParameter () const {
  return (m_LossyMinTruncationParameter);
}


/*!
     Get the maximum value we are truncating to.

     \return Value as an unsigned integer.
*/
int QScoresSettings::GetLossyMaxTruncationParameter () const {
  return (m_LossyMaxTruncationParameter);
}


/*!
     Get the gap size of the logarithmic binning.

     \return Value as an unsigned integer.
*/
int QScoresSettings::GetLossyLogBinningParameter () const {
  return (m_LossyLogBinningParameter);
}


/*!
     Get the gap size of the uniform binning.

     \return Value as an unsigned integer.
*/
int QScoresSettings::GetLossyUniBinningParameter () const {
  return (m_LossyUniBinningParameter);
}


//  -----------------------------------------------------------------
//  Lossless transformation options
//  -----------------------------------------------------------------


/*!
     Get the gap transform setting.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetTransformGapTrans () const {
  return (m_TransformGapTrans);
}


/*!
     Get the minimum shifting setting.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetTransformMinShift () const {
  return (m_TransformMinShift);
}


/*!
     Get the frequency-based reordering setting.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetTransformFreqOrder () const {
  return (m_TransformFreqOrder);
}


//  -----------------------------------------------------------------
//  Compression options
//  -----------------------------------------------------------------


/*!
     Get the Binary compression setting.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetCompressionBinary () const {
  return (m_CompressionBinary);
}


/*!
     Get the Gamma compression setting.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetCompressionGamma () const {
  return (m_CompressionGamma);
}


/*!
     Get the Delta compression setting.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetCompressionDelta () const {
  return (m_CompressionDelta);
}


/*!
     Get the parameter for Golomb coding.  A negative or zero value means Golomb coding is not used.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetCompressionGolomb () const {
  return (m_CompressionGolomb);
}


/*!
     Get the parameter for Rice coding.  A negative or zero value means Rice coding is not used.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetCompressionRice () const {
  return (m_CompressionRice);
}


/*!
     Get the interpolative coding compression setting.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetCompressionInterP () const {
  return (m_CompressionInterP);
}


/*!
     Get the Huffman coding compression setting.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetCompressionHuffman () const {
  return (m_CompressionHuffman);
}


/*!
     Get the arithmetic coding compression setting.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetCompressionArithmetic () const {
  return (m_CompressionArithmetic);
}


/*!
     Get the gzip compression setting.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetCompressionGzip () const {
  return (m_CompressionGzip);
}


/*!
     Get the bzip compression setting.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetCompressionBzip () const {
  return (m_CompressionBzip);
}


/*!
     Get the Re-Pair compression setting.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetCompressionRepair () const {
  return (m_CompressionRepair);
}


/*!
     Get the PPM compression setting.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetCompressionPPM () const {
  return (m_CompressionPPM);
}


/*!
     Get the no compression setting.

     \return Boolean value representing the setting.
*/
bool QScoresSettings::GetCompressionNone () const {
  return (m_CompressionNone);
}


/*!
     Get the global parameter for Golomb or Rice coding

     \return Parameter
*/
unsigned int QScoresSettings::GetCompressionGlobalParameter () const {
  return (m_CompressionGlobalParameter);
}


