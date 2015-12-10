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
    \file decode.cpp
    Decoding functions for a block.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: decode.cpp 224 2011-11-06 15:32:09Z rwan $
*/
/*******************************************************************/

#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <climits>  //  UINT_MAX

#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations

using namespace std;
namespace bfs = boost::filesystem;
// namespace btk = boost::tokenizer;

#include "external-software.hpp"
#include "block-statistics.hpp"
#include "bitbuffer.hpp"
#include "bitbuffer_exception.hpp"
#include "bitio-defn.hpp"
#include "interpolative.hpp"
#include "huffman.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "qscores-settings.hpp"
#include "qscores-local.hpp"
#include "qscores-defn.hpp"
#include "qscores.hpp"


//!  The number of symbols to decode with Huffman coding with each call of DecodeMessage ()
const unsigned int g_HUFFMAN_DECODE_SYMBOLS = 1024;


/*!
     Decode the header of the current block.

     \param[in] block_count Block ID (from 0); not encoded and just used for printing
                information to the screen
*/
int QScores::DecodeHeaderBlock (int block_count) {
  unsigned int lossless_remap_size = 0;
  vector<unsigned int> lossless_remap;
  int current_blocksize = g_NO_BLOCKSIZE;

  //  Length of the reads for the file
  if (block_count == 0) {
    m_FileReadLength = Delta_Decode (m_BitBuff_In);
    m_FileReadLength--;
    
    m_FileBlockSize = Delta_Decode (m_BitBuff_In);
    m_FileBlockSize--;
    
    //  Output the lossy transformation parameter
    if (m_QScoresSettings.GetLossyLogBinning ()) {
      unsigned int tmp = Delta_Decode (m_BitBuff_In);
      tmp--;
      m_QScoresSettings.SetLossyLogBinningParameter (static_cast<int> (tmp));
    }
    else if (m_QScoresSettings.GetLossyUniBinning ()) {
      unsigned int tmp = Delta_Decode (m_BitBuff_In);
      tmp--;
      m_QScoresSettings.SetLossyUniBinningParameter (static_cast<int> (tmp));
    }
  }

  //  Decode the number of reads in this block
  current_blocksize = Delta_Decode (m_BitBuff_In);
  if (current_blocksize == g_EOF_REACHED) {
    return (g_EOF_REACHED);
  }
  
  //  Block size; see EncodeHeaderBlock ()
  if (current_blocksize == 1) {
    current_blocksize = m_FileBlockSize;
  }
  else {
    current_blocksize--;
  }

  //  Length of the reads; see EncodeHeaderBlock ()
  m_BlockReadLength = Delta_Decode (m_BitBuff_In);
  if (m_BlockReadLength == 1) {
    m_BlockReadLength = m_FileReadLength;
  }
  else if (m_BlockReadLength == 2) {
    m_BlockReadLength = g_READ_LENGTH_VARIABLE;
  }
  else {
    m_BlockReadLength -= 2;
  }

  m_BlockMinimum = 0;
  if (m_QScoresSettings.GetTransformMinShift ()) {
    //  Offset from the smallest value; subtract 1 from it
    m_BlockMinimum = Delta_Decode (m_BitBuff_In);
    m_BlockMinimum--;
  }

  //  Sub-alphabet remapping
  if (m_QScoresSettings.GetTransformFreqOrder ()) {
    lossless_remap_size = Delta_Decode (m_BitBuff_In);
    Interpolative_Decode (m_BitBuff_In, lossless_remap, lossless_remap_size);

    m_BlockStatistics.Initialize ();
    m_BlockStatistics.CopyIDsToQScores (lossless_remap);
  }

  //  Binary coding parameter
  if (m_QScoresSettings.GetCompressionBinary ()) {
    m_CompressionParameter = Delta_Decode (m_BitBuff_In);
  }

  //  Golomb or Rice coding parameter
  if ((m_QScoresSettings.GetCompressionGolomb ()) ||
      (m_QScoresSettings.GetCompressionRice ())) {
    m_CompressionParameter = Delta_Decode (m_BitBuff_In);
  }  
  
  if (GetDebug ()) {
    if (block_count % g_BLOCK_STATUS_FREQUENCY == 0) {
      cerr << "II\tDecoding block " << block_count << "\t";
      cerr << current_blocksize << "\t";
      cerr << m_BlockMinimum << "\t";
      cerr << m_BlockReadLength << "\t";
      if (m_QScoresSettings.GetLossyLogBinning ()) {
        cerr << "[QB " << m_QScoresSettings.GetLossyLogBinningParameter () << "]\t";
      }
      else if (m_QScoresSettings.GetLossyUniBinning ()) {
        cerr << "[EB " << m_QScoresSettings.GetLossyUniBinningParameter () << "]\t";
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

  return current_blocksize;
}


/*!
     Decode the current block using static codes.

     \param[in] blocksize Number of reads in this block
*/
void QScores::DecodeStaticCodesBlock (int blocksize) {
  for (int i = 0; i < blocksize; i++) {
    QScoresSingle tmp = QScoresSingle ();

    if (m_QScoresSettings.GetCompressionBinary ()) {
      tmp.UnapplyCompressionBinary (m_BitBuff_In, m_CompressionParameter, m_BlockReadLength);
    }
    else if (m_QScoresSettings.GetCompressionGamma ()) {
      tmp.UnapplyCompressionGamma (m_BitBuff_In, m_BlockReadLength);
    }
    else if (m_QScoresSettings.GetCompressionDelta ()) {
      tmp.UnapplyCompressionDelta (m_BitBuff_In, m_BlockReadLength);
    }
    else if (m_QScoresSettings.GetCompressionGolomb () > 0) {
      tmp.UnapplyCompressionGolomb (m_BitBuff_In, m_CompressionParameter, m_BlockReadLength);
    }
    else if (m_QScoresSettings.GetCompressionRice () > 0) {
      tmp.UnapplyCompressionRice (m_BitBuff_In, m_CompressionParameter, m_BlockReadLength);
    }
    else if (m_QScoresSettings.GetCompressionInterP ()) {
      tmp.UnapplyCompressionInterP (m_BitBuff_In, m_BlockReadLength);
    }
    
    m_Qscores.push_back (tmp);
  }

  return;
}


/*!
     Decode the current block of quality scores using Huffman coding.

     \param[in] blocksize Number of reads in this block
*/
void QScores::DecodeHuffmanBlock (int blocksize) {
  vector<unsigned int> buffer;  //  Temporary buffer of quality scores (not yet split)
  vector<unsigned int> tmp;  //  Temporary quality scores' read
  unsigned int block_length = 0;  //  Length of the block in # of symbols
  unsigned int curr_read_length = 0;
  
  Huffman hm_in;

  //  Start decoding
  hm_in.DecodeBegin (m_BitBuff_In);
  block_length = hm_in.GetMessageLength ();

  //  Continue while there are still symbols left to decode
  while (block_length != 0) {
    if (block_length > g_HUFFMAN_DECODE_SYMBOLS) {
      buffer = hm_in.DecodeMessage (m_BitBuff_In, g_HUFFMAN_DECODE_SYMBOLS);
      block_length -= g_HUFFMAN_DECODE_SYMBOLS;
    }
    else {
      buffer = hm_in.DecodeMessage (m_BitBuff_In, block_length);
      block_length = 0;
    }
    
    //  Process the decoded symbols; if the current length is equal to m_BlockReadLength, then we completed a read
    for (unsigned int i = 0; i < buffer.size (); i++) {
        tmp.push_back (buffer[i]);
        curr_read_length++;
        if (curr_read_length == m_BlockReadLength) {
          QScoresSingle qscores_tmp (tmp);
          m_Qscores.push_back (qscores_tmp);
          tmp.clear ();
          curr_read_length = 0;
        }
    }
  }

  //  Finish decoding
  hm_in.DecodeFinish (m_BitBuff_In);

  return;
}


/*!
     Decode the current block using an external compression system.

     \param[in] blocksize Number of reads in this block
*/
void QScores::DecodeExternalBlock (int blocksize) {
  char *buffer = NULL;
  
  //  Read in the size of the binary representation from the BitBuffer
  unsigned int compressed_filesize = 0;
  m_BitBuff_In.ReadUInts (&compressed_filesize, 1);

  //  Read in the bytes of the binary representation from the BitBuffer
  buffer = new char [compressed_filesize];
  m_BitBuff_In.ReadChars (buffer, compressed_filesize);

  //  Decompress the buffer using an external program/library
  m_ExternalSoftware.UnProcess (buffer, compressed_filesize, true);
  delete [] buffer;
  
  unsigned int uncompressed_filesize = m_ExternalSoftware.GetOutBufferLength ();
  buffer = m_ExternalSoftware.RetrieveChar ();
  buffer[uncompressed_filesize] = '\0';
  
  //  Convert the buffer to a string
  string buffer_string (buffer);
  for (unsigned int i = 0; i < buffer_string.length (); i += m_BlockReadLength) {
    string tmp_str = buffer_string.substr (i, m_BlockReadLength);
    QScoresSingle tmp (tmp_str);
    tmp.StrToInt ();
    m_Qscores.push_back (tmp);
  }
  
  //  Free memory
  free (buffer);

  //  Reset for next block
  m_ExternalSoftware.UnInitialize ();
  
  return;
}


