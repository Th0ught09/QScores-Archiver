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
    \file qscores-settings.cpp
    Constructor and destructor for QScoresSettings class definition .
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: qscores-settings.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <vector>
#include <string>
#include <iostream>
#include <ostream>
#include <iomanip>  //  setw
#include <fstream>
#include <climits>
#include <cstdlib>

using namespace std;

#include "common.hpp"
#include "bitbuffer.hpp"
#include "bitio-defn.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-settings-local.hpp"
#include "qscores-settings.hpp"


/*!
     Constructor that takes one optional argument.
     
     \param[in] debug Set to true if in debug mode; false by default
*/
QScoresSettings::QScoresSettings ()
  : m_Debug (false),
    m_InputFn (""),
    m_OutputFn (""),
    m_Mapping (e_QSCORES_MAP_UNSET),
    m_LossyMinTruncation (false),
    m_LossyMaxTruncation (false),
    m_LossyLogBinning (false),
    m_LossyUniBinning (false),
    m_LossyMinTruncationParameter (-1),
    m_LossyMaxTruncationParameter (-1),
    m_LossyLogBinningParameter (-1),
    m_LossyUniBinningParameter (-1),
    m_TransformGapTrans (false),
    m_TransformMinShift (false),
    m_TransformFreqOrder (false),
    m_CompressionBinary (false),
    m_CompressionGamma (false),
    m_CompressionDelta (false),
    m_CompressionGolomb (false),
    m_CompressionRice (false),
    m_CompressionInterP (false),
    m_CompressionGlobalParameter (g_DEFAULT_GOLOMB_RICE_PARAM),
    m_CompressionHuffman (false),
    m_CompressionArithmetic (false),
    m_CompressionGzip (false),
    m_CompressionBzip (false),
    m_CompressionRepair (false),
    m_CompressionPPM (false),
    m_CompressionNone (false)
{
}


/*!
     Destructor that takes no arguments
*/
QScoresSettings::~QScoresSettings() {
}


/*!
     Overloaded << operator defined as a friend of QScoresSettings.

     \param[in] os Output stream
     \param[in] qs QScoresSettings passed as reference
     \return Output stream
*/
ostream &operator<< (ostream &os, const QScoresSettings& qs) {
  os << left << "II\tGeneral options" << endl;
  cerr << left << setw (g_VERBOSE_WIDTH) << "II\t  Input filename:" << qs.GetInputFn () << endl;
  cerr << left << setw (g_VERBOSE_WIDTH) << "II\t  Output filename:" << qs.GetOutputFn () << endl;
  
  os << left << "II\tMapping" << endl;
  os << left << setw (g_VERBOSE_WIDTH) << "II\t  Scheme:" << qs.GetQScoresMappingStr () << endl;

  os << left << "II\tLossy transformations" << endl;
  if (qs.GetLossyMinTruncation () > 0) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Range truncation [minimum]:";
    os << qs.GetLossyMinTruncationParameter () << endl;
  }
  if (qs.GetLossyMaxTruncation () > 0) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Range truncation [maximum]:";
    os << qs.GetLossyMaxTruncationParameter () << endl;
  }
  if (qs.GetLossyLogBinning ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Logarithmic binning:";
    if (qs.GetLossyLogBinningParameter () == -1) {
      os << "Not yet known..." << endl;
    }
    else {
      os << qs.GetLossyLogBinningParameter () << endl;
    }
  }
  if (qs.GetLossyUniBinning ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Uniform binning:";
    if (qs.GetLossyUniBinningParameter () == -1) {
      os << "Not yet known..." << endl;
    }
    else {
      os << qs.GetLossyUniBinningParameter () << endl;
    }
  }

  os << left << "II\tLossless transformations" << endl;
  if (qs.GetTransformGapTrans ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Gap Transforming:" << (qs.GetTransformGapTrans () == true ? "Yes" : "No") << endl;
  }
  if (qs.GetTransformMinShift ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Minimum shifting:" << (qs.GetTransformMinShift () == true ? "Yes" : "No") << endl;
  }
  if (qs.GetTransformFreqOrder ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Frequency-based reordering:" << (qs.GetTransformFreqOrder () == true ? "Yes" : "No") << endl;
  }

  os << left << "II\tCompression" << endl;
  if (qs.GetCompressionBinary ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Binary coding:" << (qs.GetCompressionBinary () == true ? "Yes" : "No") << endl;
  }
  if (qs.GetCompressionGamma ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Gamma coding:" << (qs.GetCompressionGamma () == true ? "Yes" : "No") << endl;
  }
  if (qs.GetCompressionDelta ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Delta coding:" << (qs.GetCompressionDelta () == true ? "Yes" : "No") << endl;
  }
  if (qs.GetCompressionGolomb ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Golomb coding:" << (qs.GetCompressionGolomb () == true ? "Yes" : "No") << endl;
  }
  if (qs.GetCompressionRice ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Rice coding:" << (qs.GetCompressionRice () == true ? "Yes" : "No") << endl;
  }
  if (qs.GetCompressionInterP ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Interpolative coding:" << (qs.GetCompressionInterP () == true ? "Yes" : "No") << endl;
  }
  if (qs.GetCompressionHuffman ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Huffman coding:" << (qs.GetCompressionHuffman () == true ? "Yes" : "No") << endl;
  }
  if (qs.GetCompressionArithmetic ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Arithmetic coding:" << (qs.GetCompressionArithmetic () == true ? "Yes" : "No") << endl;
  }
  if (qs.GetCompressionGzip ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Gzip:" << (qs.GetCompressionGzip () == true ? "Yes" : "No") << endl;
  }
  if (qs.GetCompressionBzip ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Bzip:" << (qs.GetCompressionBzip () == true ? "Yes" : "No") << endl;
  }
  if (qs.GetCompressionRepair ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Re-Pair:" << (qs.GetCompressionRepair () == true ? "Yes" : "No") << endl;
  }
  if (qs.GetCompressionPPM ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  PPM:" << (qs.GetCompressionPPM () == true ? "Yes" : "No") << endl;
  }
  if (qs.GetCompressionNone ()) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  None:" << (qs.GetCompressionNone () == true ? "Yes" : "No") << endl;
  }

  if (qs.GetCompressionGlobalParameter () != g_DEFAULT_GOLOMB_RICE_PARAM) {
    os << left << setw (g_VERBOSE_WIDTH) << "II\t  Global parameter:" << (qs.GetCompressionGlobalParameter ()) << endl;
  }
  
  return os;
}


/*!
     Check the settings related to file I/O to ensure they are valid.

     \return FALSE if there were problems, TRUE otherwise
*/
bool QScoresSettings::CheckFileSettings () const {
  //  Check the filenames.
  if (GetInputFn ().length () == 0) {
    cerr << "EE\tInput filename required with the --input option." << endl;
    return false;
  }

  if (GetOutputFn ().length () == 0) {
    cerr << "EE\tOutput filename required with the --output option." << endl;
    return false;
  }

  return true;
}


/*!
     Check the lossy settings to ensure they are valid.

     \return FALSE if there were problems, TRUE otherwise
*/
bool QScoresSettings::CheckLossySettings () const {
  int lossy_count = 0;

  //  Check that no more than one lossy method is employed
  if (GetLossyMinTruncation ()) {
    lossy_count++;
  }
  if (GetLossyMaxTruncation ()) {
    lossy_count++;
  }
  if (GetLossyLogBinning ()) {
    lossy_count++;
  }
  if (GetLossyUniBinning ()) {
    lossy_count++;
  }
  if (lossy_count > 1) {
    cerr << "EE\tNo more than one lossy transformation can be selected." << endl;
    return false;
  }

  //  Check that the parameter values are valid
  if (GetLossyMinTruncation ()) {
    if ((GetLossyMinTruncationParameter () < GetQScoresMappingMin ()) || (GetLossyMinTruncationParameter () > GetQScoresMappingMax ())) {
      cerr << "EE\tThe parameter to --mintrunc is outside the allowable range of [" << GetQScoresMappingMin () << ", " << GetQScoresMappingMax () << "]." << endl;
      return false;      
    }
  }
  if (GetLossyMaxTruncation ()) {
    if ((GetLossyMaxTruncationParameter () < GetQScoresMappingMin ()) || (GetLossyMaxTruncationParameter () > GetQScoresMappingMax ())) {
      cerr << "EE\tThe parameter to --trunc is outside the allowable range of [" << GetQScoresMappingMin () << ", " << GetQScoresMappingMax () << "]." << endl;
      return false;      
    }
  }
  if (GetLossyLogBinning ()) {
    if ((GetLossyLogBinningParameter () == 0) || (GetLossyLogBinningParameter () > (GetQScoresMappingRange () + 1))) {
      cerr << "EE\tThe parameter to --logbin cannot be 0 or is outside the actual quality scores range of " << GetQScoresMappingRange () << "." << endl; 
      return false;
    }
  }
  if (GetLossyUniBinning ()) {
    if ((GetLossyUniBinningParameter () == 0) || (GetLossyUniBinningParameter () > g_ERRPROB_BINNING_MAX)) {
      cerr << "EE\tThe parameter to --unibin cannot be 0 and should not be larger than " << g_ERRPROB_BINNING_MAX << "." << endl;
      cerr << "EE\tIn theory, any value should be possible, but larger values are probably meaningless and will make calculation of the look up table take too long." << endl;
      return false;      
    }
  }

  return true;
}
  
  
/*!
     Check the lossless settings to ensure they are valid.

     \return FALSE if there were problems, TRUE otherwise
*/
bool QScoresSettings::CheckLosslessSettings () const {
  int lossless_count = 0;

  //  Check that no more than one lossless method is employed
  if (GetTransformGapTrans ()) {
    lossless_count++;
  }
  if (GetTransformMinShift ()) {
    lossless_count++;
  }
  if (GetTransformFreqOrder ()) {
    lossless_count++;
  }
  if (lossless_count > 1) {
    cerr << "EE\tNo more than one lossless transformation can be selected." << endl;
    return false;
  }

  return true;
}


/*!
     Check the compression settings to ensure they are valid.

     \return FALSE if there were problems, TRUE otherwise
*/
bool QScoresSettings::CheckCompressionSettings () const {
  int compression_count = 0;

  //  Check that either none or only one compression
  if (GetCompressionBinary ()) {
    compression_count++;
  }
  if (GetCompressionGamma ()) {
    compression_count++;
  }
  if (GetCompressionDelta ()) {
    compression_count++;
  }
  if (GetCompressionGolomb ()) {
    compression_count++;
  }
  if (GetCompressionRice ()) {
    compression_count++;
  }
  if (GetCompressionInterP ()) {
    compression_count++;
  }
  if (GetCompressionHuffman ()) {
    compression_count++;
  }
  if (GetCompressionArithmetic ()) {
    compression_count++;
  }
  if (GetCompressionGzip ()) {
    compression_count++;
  }
  if (GetCompressionBzip ()) {
    compression_count++;
  }
  if (GetCompressionRepair ()) {
    compression_count++;
  }
  if (GetCompressionPPM ()) {
    compression_count++;
  }
  if (GetCompressionNone ()) {
    compression_count++;
  }
  if (compression_count != 1) {
    cerr << "EE\tOne and only one compression option can be selected." << endl;
    return false;
  }

  if ((GetCompressionRice ()) && (GetCompressionGlobalParameter () != g_DEFAULT_GOLOMB_RICE_PARAM)) {
    if (GetCompressionGlobalParameter () >= g_UINT_SIZE_BITS) {
      cerr << "EE\tThe parameter for Rice coding cannot be greater than or equal to " << g_UINT_SIZE_BITS << "." << endl;
      return false;
    }
  }

  return true;
}


/*!
     Check the settings to ensure they are valid.

     \return FALSE if there were problems, TRUE otherwise
*/
bool QScoresSettings::CheckSettings () const {

  if (!CheckLossySettings ()) {
    return false;
  }
  
  if (!CheckLosslessSettings ()) {
    return false;
  }

  if (!CheckCompressionSettings ()) {
    return false;
  }

  return true;
}


/*!
     Read settings in binary format.

     \param[in] bitbuffer BitBuffer to read data from
     \return Always returns TRUE
*/
bool QScoresSettings::ReadBinarySettings (BitBuffer &bitbuffer) {
  unsigned int setting = 0;

  setting = BinaryHigh_Decode (bitbuffer, e_QSCORES_BINARY_SETTINGS_LAST);
  if (GetDebug ()) {
    cerr << "DD\t[QScoresSettings::ReadBinarySettings ()] Read in setting:  " << setting << endl;
  }
  
  if ((setting & g_MAPPING_BITMASK) == e_QSCORES_BINARY_SETTINGS_MAP_SANGER) {
    SetQScoresMapping ("sanger");
  }
  else if ((setting & g_MAPPING_BITMASK) == e_QSCORES_BINARY_SETTINGS_MAP_SOLEXA) {
    SetQScoresMapping ("solexa");
  }
  else if ((setting & g_MAPPING_BITMASK) == e_QSCORES_BINARY_SETTINGS_MAP_ILLUMINA) {
    SetQScoresMapping ("illumina");
  }

  if ((setting & g_LOSSY_TRANSFORM_BITMASK) == e_QSCORES_BINARY_SETTINGS_LOSSY_MAXTRUNC) {
    //  Since we are decoding, we do not need to know that lossy max truncation was previously performed
//     SetLossyMaxTruncation ();
  }
  else if ((setting & g_LOSSY_TRANSFORM_BITMASK) == e_QSCORES_BINARY_SETTINGS_LOSSY_QSBIN) {
    SetLossyLogBinning ();
  }
  else if ((setting & g_LOSSY_TRANSFORM_BITMASK) == e_QSCORES_BINARY_SETTINGS_LOSSY_EPBIN) {
    SetLossyUniBinning ();
  }

  if ((setting & e_QSCORES_BINARY_SETTINGS_LOSSLESS_DIFF) != 0) {
    SetTransformGapTrans ();
  }
  if ((setting & e_QSCORES_BINARY_SETTINGS_LOSSLESS_RESCALING) != 0) {
    SetTransformMinShift ();
  }
  if ((setting & e_QSCORES_BINARY_SETTINGS_LOSSLESS_REMAPPING) != 0) {
    SetTransformFreqOrder ();
  }
  
  if ((setting & g_COMPRESSION_METHOD_BITMASK) == e_QSCORES_BINARY_SETTINGS_COMP_BINARY) {
    SetCompressionBinary ();
  }
  if ((setting & g_COMPRESSION_METHOD_BITMASK) == e_QSCORES_BINARY_SETTINGS_COMP_GAMMA) {
    SetCompressionGamma ();
  }
  if ((setting & g_COMPRESSION_METHOD_BITMASK) == e_QSCORES_BINARY_SETTINGS_COMP_DELTA) {
    SetCompressionDelta ();
  }
  if ((setting & g_COMPRESSION_METHOD_BITMASK) == e_QSCORES_BINARY_SETTINGS_COMP_GOLOMB) {
    SetCompressionGolomb ();
  }
  if ((setting & g_COMPRESSION_METHOD_BITMASK) == e_QSCORES_BINARY_SETTINGS_COMP_RICE) {
    SetCompressionRice ();
  }
  if ((setting & g_COMPRESSION_METHOD_BITMASK) == e_QSCORES_BINARY_SETTINGS_COMP_INTERP) {
    SetCompressionInterP ();
  }
  if ((setting & g_COMPRESSION_METHOD_BITMASK) == e_QSCORES_BINARY_SETTINGS_COMP_HUFFMAN) {
    SetCompressionHuffman ();
  }
  if ((setting & g_COMPRESSION_METHOD_BITMASK) == e_QSCORES_BINARY_SETTINGS_COMP_ARITHMETIC) {
    SetCompressionArithmetic ();
  }
  if ((setting & g_COMPRESSION_METHOD_BITMASK) == e_QSCORES_BINARY_SETTINGS_COMP_GZIP) {
    SetCompressionGzip ();
  }
  if ((setting & g_COMPRESSION_METHOD_BITMASK) == e_QSCORES_BINARY_SETTINGS_COMP_BZIP) {
    SetCompressionBzip ();
  }
  if ((setting & g_COMPRESSION_METHOD_BITMASK) == e_QSCORES_BINARY_SETTINGS_COMP_REPAIR) {
    SetCompressionRepair ();
  }
//   if ((setting & g_COMPRESSION_METHOD_BITMASK) == e_QSCORES_BINARY_SETTINGS_COMP_PPM) {
//     SetCompressionPPM ();
//   }
  if ((setting & g_COMPRESSION_METHOD_BITMASK) == e_QSCORES_BINARY_SETTINGS_COMP_NONE) {
    SetCompressionNone ();
  }
  
  return (true);
}


/*!
     Write settings in binary format.

     \param[in] bitbuffer BitBuffer to write data to
     \return Always returns TRUE
*/
bool QScoresSettings::WriteBinarySettings (BitBuffer &bitbuffer) {
  unsigned int setting = 0;

  switch (GetQScoresMapping ()) {
    case e_QSCORES_BINARY_SETTINGS_MAP_SANGER :
      setting = setting | (e_QSCORES_BINARY_SETTINGS_MAP_SANGER & g_MAPPING_BITMASK);
      break;
    case e_QSCORES_BINARY_SETTINGS_MAP_SOLEXA :
      setting = setting | (e_QSCORES_BINARY_SETTINGS_MAP_SOLEXA & g_MAPPING_BITMASK);
      break;
    case e_QSCORES_BINARY_SETTINGS_MAP_ILLUMINA :
      setting = setting | (e_QSCORES_BINARY_SETTINGS_MAP_ILLUMINA & g_MAPPING_BITMASK);
      break;
    default :
      break;
  }

  if (GetTransformGapTrans ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_LOSSLESS_DIFF & g_LOSSLESS_TRANSFORM_BITMASK);
  }
  if (GetTransformMinShift ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_LOSSLESS_RESCALING & g_LOSSLESS_TRANSFORM_BITMASK);
  }
  if (GetTransformFreqOrder ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_LOSSLESS_REMAPPING & g_LOSSLESS_TRANSFORM_BITMASK);
  }

  if (GetLossyMaxTruncation ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_LOSSY_MAXTRUNC & g_LOSSY_TRANSFORM_BITMASK);
  }
  else if (GetLossyLogBinning ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_LOSSY_QSBIN & g_LOSSY_TRANSFORM_BITMASK);
  }
  else if (GetLossyUniBinning ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_LOSSY_EPBIN & g_LOSSY_TRANSFORM_BITMASK);
  }
      
  if (GetCompressionBinary ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_COMP_BINARY & g_COMPRESSION_METHOD_BITMASK);
  }
  else if (GetCompressionGamma ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_COMP_GAMMA & g_COMPRESSION_METHOD_BITMASK);
  }
  else if (GetCompressionDelta ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_COMP_DELTA & g_COMPRESSION_METHOD_BITMASK);
  }
  else if (GetCompressionGolomb ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_COMP_GOLOMB & g_COMPRESSION_METHOD_BITMASK);
  }
  else if (GetCompressionRice ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_COMP_RICE & g_COMPRESSION_METHOD_BITMASK);
  }
  else if (GetCompressionInterP ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_COMP_INTERP & g_COMPRESSION_METHOD_BITMASK);
  }
  else if (GetCompressionHuffman ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_COMP_HUFFMAN & g_COMPRESSION_METHOD_BITMASK);
  }
  else if (GetCompressionArithmetic ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_COMP_ARITHMETIC & g_COMPRESSION_METHOD_BITMASK);
  }
  else if (GetCompressionGzip ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_COMP_GZIP & g_COMPRESSION_METHOD_BITMASK);
  }
  else if (GetCompressionBzip ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_COMP_BZIP & g_COMPRESSION_METHOD_BITMASK);
  }
  else if (GetCompressionRepair ()) {
    setting = setting | (e_QSCORES_BINARY_SETTINGS_COMP_REPAIR & g_COMPRESSION_METHOD_BITMASK);
  }
//   if (GetCompressionPPM ()) {
//     setting = setting | (e_QSCORES_BINARY_SETTINGS_COMP_PPM & g_COMPRESSION_METHOD_BITMASK);
//   }

  BinaryHigh_Encode (bitbuffer, setting, e_QSCORES_BINARY_SETTINGS_LAST);
  if (GetDebug ()) {
    cerr << "DD\t[QScoresSettings::WriteBinarySettings ()] Write out setting:  " << setting << endl;
  }
  
  return (true);
}

