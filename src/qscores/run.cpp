//  ###########################################################################
//  Copyright 2011-2015, 2024 by Raymond Wan (rwan.work@gmail.com)
//    https://github.com/rwanwork/QScores-Archiver
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
//  ###########################################################################


/*******************************************************************/
/*!
    \file run.cpp
    Primary function for class QScores .
*/
/*******************************************************************/

#include <string>
#include <vector>
#include <climits>  //  UINT_MAX
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>  //  setw

using namespace std;

#include "external-software.hpp"
#include "block-statistics.hpp"
#include "bitbuffer.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "qscores-settings.hpp"
#include "qscores-defn.hpp"
#include "qscores.hpp"


/*!
     Execute the main part of the program by iterating over each quality score

     \return Returns true on success, false on failure.
*/
bool QScores::Run() {
  int block_count = 0;

  //  Open the files for I/O
  if (!OpenFiles ()) {
    return (false);
  }

  //  Initialize things in QScores that the constructor did not know about
  Initialize ();

  //  Check the settings after reading in the files (since some settings are in the file's header during decoding)
  if (!m_QScoresSettings.CheckSettings ()) {
    return (false);
  }
    
  //  Output the settings to the screen
  if (GetVerbose ()) {
    cerr << left << m_ExternalSoftware << endl;
    cerr << m_QScoresSettings << endl;
  }

  if (GetEncode ()) {
    if (GetVerbose ()) {
      cerr << "II\tEncoding data file..." << endl;
    }

    while (true) {
      //  Read in the block
      int current_blocksize = ReadInFileBlock (m_Blocksize);      
      if (current_blocksize == g_EOF_REACHED) {
        if (!m_QScoresSettings.GetCompressionNone ()) {
          EncodeEOF ();
        }
        
        if (GetVerbose ()) {
          cerr << "II\t" << block_count << " blocks created of at most " << m_Blocksize << " reads each." << endl;
        }
        break;
      }
      PreprocessBlock (current_blocksize);

      if (m_QScoresSettings.GetCompressionNone ()) {
        EncodeIntToQScore (current_blocksize);
      }
      else {
        EncodeHeaderBlock (current_blocksize, block_count);
        if ((m_QScoresSettings.GetCompressionBinary ()) ||
            (m_QScoresSettings.GetCompressionGamma ()) ||
            (m_QScoresSettings.GetCompressionDelta ()) ||
            (m_QScoresSettings.GetCompressionGolomb () > 0) ||
            (m_QScoresSettings.GetCompressionRice () > 0) ||
            (m_QScoresSettings.GetCompressionInterP ())) {
          EncodeStaticCodesBlock (current_blocksize);
        }
        else if (m_QScoresSettings.GetCompressionHuffman ()) {
          EncodeHuffmanBlock (current_blocksize);
        }
        else if ((m_QScoresSettings.GetCompressionGzip ()) ||
                 (m_QScoresSettings.GetCompressionBzip ()) ||
                 (m_QScoresSettings.GetCompressionRepair ()) ||
                 (m_QScoresSettings.GetCompressionPPM ())) {
          EncodeExternalBlock (current_blocksize);
        }
      }

      if (m_QScoresSettings.GetCompressionNone ()) {
        WriteOutFileBlock ();
      }
      block_count++;
    }
  }
  else {
    //  Decoding mode
    if (GetVerbose ()) {
      cerr << "II\tDecoding data file..." << endl;
    }
    
    int current_blocksize = g_EOF_REACHED;
    while (1) {
      //  Clear the vector of quality scores
      m_Qscores.clear ();

      current_blocksize = DecodeHeaderBlock (block_count);
      if (current_blocksize == g_EOF_REACHED) {
        if (GetVerbose ()) {
          cerr << "II\t" << block_count << " blocks decoded." << endl;
        }
        break;
      }

      if ((m_QScoresSettings.GetCompressionBinary ()) ||
          (m_QScoresSettings.GetCompressionGamma ()) ||
          (m_QScoresSettings.GetCompressionDelta ()) ||
          (m_QScoresSettings.GetCompressionGolomb () > 0) ||
          (m_QScoresSettings.GetCompressionRice () > 0) ||
          (m_QScoresSettings.GetCompressionInterP ())) {          
        DecodeStaticCodesBlock (current_blocksize);
      }
      else if (m_QScoresSettings.GetCompressionHuffman ()) {
        DecodeHuffmanBlock (current_blocksize);
      }
      else if ((m_QScoresSettings.GetCompressionGzip ()) ||
               (m_QScoresSettings.GetCompressionBzip ()) ||
               (m_QScoresSettings.GetCompressionRepair ()) ||
               (m_QScoresSettings.GetCompressionPPM ())) {
        DecodeExternalBlock (current_blocksize);
      }
      UnPreprocessBlock (current_blocksize);
      WriteOutFileBlock ();
      block_count++;
    }
  }

  if (!CloseFiles ()) {
    return (false);
  }

  return (true);
}

