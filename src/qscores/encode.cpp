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
    \file encode.cpp
    Encoding functions for a block.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: encode.cpp 224 2011-11-06 15:32:09Z rwan $
*/
/*******************************************************************/

#include "filesystem-v3.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <climits>  //  UINT_MAX
#include <cmath>

#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations

using namespace std;
namespace bfs = boost::filesystem;

#include "external-software-exception.hpp"
#include "external-software.hpp"
#include "block-statistics.hpp"
#include "bitbuffer.hpp"
#include "bitio-defn.hpp"
#include "huffman.hpp"
#include "interpolative.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "qscores-settings.hpp"
#include "qscores-local.hpp"
#include "qscores-defn.hpp"
#include "qscores.hpp"


/*!
     Encode a value to mark the end of the file.
*/
void QScores::EncodeEOF () {
  Delta_Encode (m_BitBuff_Out, g_EOF_REACHED);
  
  return;
}


/*!
     Encode the header of the current block.  For Golomb, Rice, and Binary coding, also 
     determine the compression parameter for this block.

     \param[in] current_blocksize The size of the current block
     \param[in] block_count Block ID (from 0); not encoded and just used for printing
                information to the screen
*/
void QScores::EncodeHeaderBlock (int current_blocksize, int block_count) {
  unsigned int lossless_remap_size = 0;
  vector<unsigned int> lossless_remap;

  //  Parameters that are global to the entire file (continuation of the global header)
  if (block_count == 0) {
    m_FileReadLength = m_BlockReadLength;
    Delta_Encode (m_BitBuff_Out, m_FileReadLength + 1);
    
    m_FileBlockSize = current_blocksize;
    Delta_Encode (m_BitBuff_Out, m_FileBlockSize + 1);

    //  Output the lossy transformation parameter
    if (m_QScoresSettings.GetLossyLogBinning ()) {
      Delta_Encode (m_BitBuff_Out, static_cast<unsigned int> (m_QScoresSettings.GetLossyLogBinningParameter () + 1));
    }
    else if (m_QScoresSettings.GetLossyUniBinning ()) {
      Delta_Encode (m_BitBuff_Out, static_cast<unsigned int> (m_QScoresSettings.GetLossyUniBinningParameter () + 1));
    }
  }
  
  //  Encode the number of reads in this block; add 1 so that 1 = use the file level value; otherwise, the block size as it appears (minus 1)
  if (current_blocksize == m_FileBlockSize) {
    Delta_Encode (m_BitBuff_Out, 1);
  }
  else {
    Delta_Encode (m_BitBuff_Out, current_blocksize + 1);
  }
  
  //  Length of the reads; add 2 so that 1 = use the file-level value; g_READ_LENGTH_VARIABLE (2) means not all the same
  if (m_BlockReadLength == m_FileReadLength) {
    Delta_Encode (m_BitBuff_Out, 1);
  }
  else if (m_BlockReadLength == g_READ_LENGTH_VARIABLE) {
    Delta_Encode (m_BitBuff_Out, 2);
  }
  else {
    Delta_Encode (m_BitBuff_Out, m_BlockReadLength + 2);
  }

  if (m_QScoresSettings.GetTransformMinShift ()) {
    //  Offset from the smallest value; add 1 in case it is 0
    Delta_Encode (m_BitBuff_Out, m_BlockMinimum + 1);
  }

  //  Sub-alphabet remapping
  if (m_QScoresSettings.GetTransformFreqOrder ()) {
    lossless_remap_size = m_BlockStatistics.GetIDsToQScoresSize ();
    lossless_remap = m_BlockStatistics.GetIDsToQScores ();
    lossless_remap.resize (lossless_remap_size);
    
    Delta_Encode (m_BitBuff_Out, lossless_remap_size);
    Interpolative_Encode (m_BitBuff_Out, lossless_remap);
  }

  //  Binary coding parameter
  if (m_QScoresSettings.GetCompressionBinary ()) {
    unsigned int current_max = 0;
    for (int i = 0; i < current_blocksize; i++) {
      m_Qscores[i].SetMinMax ();
      if (m_Qscores[i].GetMax () > current_max) {
        current_max = m_Qscores[i].GetMax ();
      }
    }
    
    m_CompressionParameter = current_max;
    
    //  Encode the parameter
    Delta_Encode (m_BitBuff_Out, m_CompressionParameter);
  }

  //  Golomb or Rice coding parameter
  if ((m_QScoresSettings.GetCompressionGolomb ()) ||
      (m_QScoresSettings.GetCompressionRice ())) {
    //  Check if we are using a global parameter or a local one
    if (m_QScoresSettings.GetCompressionGlobalParameter () != g_DEFAULT_GOLOMB_RICE_PARAM) {
      m_CompressionParameter = m_QScoresSettings.GetCompressionGlobalParameter ();
    }
    else {
      //  Tabulate the average across all reads
      unsigned int sum = 0;
      unsigned int count = 0;
      for (int i = 0; i < current_blocksize; i++) {
        sum += m_Qscores[i].GetIntSum ();
        count += m_Qscores[i].GetIntLength ();
      }

      double average = (static_cast<double> (sum)) / (static_cast<double> (count));
      m_CompressionParameter = static_cast<unsigned int> (ceil (g_GOLOMB_RICE_CONSTANT * (average)));
      
      //  Rice coding -- need to find the largest power of 2
      if (m_QScoresSettings.GetCompressionRice ()) {
        unsigned int tmp = 1;
        unsigned int shift = 0;
        while ((tmp << shift) < m_CompressionParameter) {
          shift++;
        }
        m_CompressionParameter = (shift - 1);
      }
      
      if ((m_CompressionParameter == UINT_MAX) || (m_CompressionParameter == 0)) {
        m_CompressionParameter = 1;
      }
    }
    
    //  Encode the parameter
    Delta_Encode (m_BitBuff_Out, m_CompressionParameter);
  }
  
  if (GetDebug ()) {
    if (block_count % g_BLOCK_STATUS_FREQUENCY == 0) {
      cerr << "II\tEncoding block " << block_count << "\t";
      cerr << current_blocksize << "\t";
      cerr << m_BlockMinimum << "\t";
      cerr << m_BlockReadLength << "\t";
      if (m_QScoresSettings.GetLossyLogBinning ()) {
        cerr << "[LB " << m_QScoresSettings.GetLossyLogBinningParameter () << "]\t";
      }
      else if (m_QScoresSettings.GetLossyUniBinning ()) {
        cerr << "[UB " << m_QScoresSettings.GetLossyUniBinningParameter () << "]\t";
      }
      if (m_QScoresSettings.GetCompressionBinary ()) {
        cerr << "[B " << m_CompressionParameter << "]\t";
      }
      else if (m_QScoresSettings.GetCompressionGolomb ()) {
        cerr << "[G " << m_CompressionParameter << "]\t";
      }
      else if (m_QScoresSettings.GetCompressionRice ()) {
        cerr << "[R " << m_CompressionParameter << "]\t";
      }
      if (m_QScoresSettings.GetTransformMinShift ()) {
        cerr << "[RS " << m_BlockMinimum << "]\t";
      }
      if (m_QScoresSettings.GetTransformFreqOrder ()) {
        cerr << "[LS " << lossless_remap_size << "]\t";
      }
      cerr << endl;
    }      
  }

  return;
}


/*!
     Encode the current block using static codes.

     \param[in] current_blocksize The size of the current block
*/
void QScores::EncodeStaticCodesBlock (int current_blocksize) {
  for (int i = 0; i < current_blocksize; i++) {
    if (m_QScoresSettings.GetCompressionBinary ()) {
      m_Qscores[i].ApplyCompressionBinary (m_BitBuff_Out, m_CompressionParameter, m_BlockReadLength);
    }
    else if (m_QScoresSettings.GetCompressionGamma ()) {
      m_Qscores[i].ApplyCompressionGamma (m_BitBuff_Out, m_BlockReadLength);
    }
    else if (m_QScoresSettings.GetCompressionDelta ()) {
      m_Qscores[i].ApplyCompressionDelta (m_BitBuff_Out, m_BlockReadLength);
    }
    else if (m_QScoresSettings.GetCompressionGolomb () > 0) {
      m_Qscores[i].ApplyCompressionGolomb (m_BitBuff_Out, m_CompressionParameter, m_BlockReadLength);
    }
    else if (m_QScoresSettings.GetCompressionRice () > 0) {
      m_Qscores[i].ApplyCompressionRice (m_BitBuff_Out, m_CompressionParameter, m_BlockReadLength);
    }
    else if (m_QScoresSettings.GetCompressionInterP ()) {
      m_Qscores[i].ApplyCompressionInterP (m_BitBuff_Out, m_BlockReadLength);
    }
  }
  
  return;
}


/*!
     Encode the current block using Huffman coding.

     \param[in] current_blocksize The size of the current block
*/
void QScores::EncodeHuffmanBlock (int current_blocksize) {
  Huffman hm_out;

  //  Update frequencies with the quality scores in this block
  for (int i = 0; i < current_blocksize; i++) {
    hm_out.UpdateFrequencies (m_Qscores[i].GetQScoreInt ());
  }
  
  //  Start encoding
  hm_out.EncodeBegin (m_BitBuff_Out);

  //  Encode each vector of quality score
  for (int i = 0; i < current_blocksize; i++) {
    hm_out.EncodeMessage (m_BitBuff_Out, m_Qscores[i].GetQScoreInt ());
  }
  
  //  Finish encoding
  hm_out.EncodeFinish (m_BitBuff_Out);

  return;
}


/*!
     Encode the current block using an external compression system.

     \param[in] current_blocksize The size of the current block
*/
void QScores::EncodeExternalBlock (int current_blocksize) {
  bool last = false;

  //  Add a read at a time
  unsigned int mini_buffer_size = 0;
  //  This is the longest that a read can be; if it is too short, the program will exit
  unsigned int max_buffer_size = 65536;  
  char* mini_buffer = (char*) calloc (max_buffer_size, sizeof (char));
  for (int i = 0; i < current_blocksize; i++) {
    if (i == (current_blocksize - 1)) {
      //  Last iteration, so notify m_ExternalSoftware
      last = true;
    }

    mini_buffer_size = m_Qscores[i].GetQScoreIntAsBinary (mini_buffer, max_buffer_size);
    m_ExternalSoftware.Process (mini_buffer, mini_buffer_size, last);
  }
  free (mini_buffer);

  unsigned int buffer_size = m_ExternalSoftware.GetOutBufferLength ();
  char *buffer = m_ExternalSoftware.RetrieveChar ();
  
  //  Append size to bitbuffer
  m_BitBuff_Out.WriteUInts (&buffer_size, 1);
    
  //  Append binary representation to bitbuffer
  m_BitBuff_Out.WriteChars (buffer, buffer_size);

  //  Free memory
  free (buffer);
  
  //  Reset for next block
  m_ExternalSoftware.UnInitialize ();
  
  return;
}


/*!
     Convert the integers to quality scores for printing (required for the --nocompress option).

     \param[in] current_blocksize The size of the current block
*/
void QScores::EncodeIntToQScore (int current_blocksize) {
  for (int i = 0; i < current_blocksize; i++) {
//     m_Qscores[i].UnapplyFinalize ();
    m_Qscores[i].IntToQScore ();
  }
  
  return;
}
