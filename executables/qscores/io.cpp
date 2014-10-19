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
    \file io.cpp
    Input/output functions for class QScores .

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: io.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <climits>  //  UINT_MAX

using namespace std;

#include "external-software.hpp"
#include "block-statistics.hpp"
#include "bitbuffer.hpp"
#include "qscores-local.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "qscores-settings.hpp"
#include "qscores-defn.hpp"
#include "qscores.hpp"


/*!
     Open files for input/output for both encoding and decoding

     \return Returns true on success, false on failure.
*/
bool QScores::OpenFiles () {
  //  Check settings pertaining to file I/O
  if (!m_QScoresSettings.CheckFileSettings ()) {
    return false;
  }
  
  //  Open files for input/output
  if (GetEncode ()) {
    //  Open text input and check if it succeeded
    m_Text_In.open (m_QScoresSettings.GetInputFn ().c_str (), ios::in);
    if (!m_Text_In) {
      cerr << "EE\tError opening file for input:  " << m_QScoresSettings.GetInputFn () << endl;
      return false;
    }
    
//     m_OldStream = cin.rdbuf (m_Text_In.rdbuf ());
//     if (GetVerbose ()) {
//       cerr << "II\tOpening file for input:  " << m_QScoresSettings.GetInputFn () << endl;
//     }

    //  Binary output if compressing; text output if no compression (used for testing only)
    if (!m_QScoresSettings.GetCompressionNone ()) {
      m_BitBuff_Out.Initialize (m_QScoresSettings.GetOutputFn(), e_MODE_WRITE);
      m_QScoresSettings.WriteBinarySettings (m_BitBuff_Out);
    }
    else {
      //  Open text output and check if it succeeded
      m_Text_Out.open (m_QScoresSettings.GetOutputFn ().c_str (), ios::out);
      if (!m_Text_Out) {
        cerr << "EE\tError opening file for output:  " << m_QScoresSettings.GetOutputFn () << endl;
        return false;
      }
    }
  }
  else {
    //  Binary input
    m_BitBuff_In.Initialize (m_QScoresSettings.GetInputFn(), e_MODE_READ);
    m_QScoresSettings.ReadBinarySettings (m_BitBuff_In);

    //  Open text output and check if it succeeded
    m_Text_Out.open (m_QScoresSettings.GetOutputFn ().c_str (), ios::out);
    if (!m_Text_Out) {
      cerr << "EE\tError opening file for output:  " << m_QScoresSettings.GetOutputFn () << endl;
      return false;
    }
    
//     m_OldStream = cout.rdbuf (m_Text_Out.rdbuf ());
//     if (GetVerbose ()) {
//       cerr << "II\tOpening file for output:  " << m_QScoresSettings.GetOutputFn () << endl;
//     }
  }
  
  return true;
}


/*!
     Closes files for input/output.

     \return Returns true on success, false on failure.
*/
bool QScores::CloseFiles () {
  if (GetEncode ()) {
    m_Text_In.close ();
    m_BitBuff_Out.Finish ();
  }
  else {
    m_BitBuff_In.Finish ();
    m_Text_Out.close ();
  }

  return true;
}


/*!
     Read in a block of quality scores from file.

     \param[in] blocksize The number of quality scores to read in
     \return Returns the number of quality score strings read in
*/
int QScores::ReadInFileBlock (int blocksize) {
  int i = 0;
  int num_qscores = 0;
  string tmp;
  unsigned int read_length = 0;
  bool lengths_same = true;  //  true/false whether all lengths the same in this block

  //  Clear the vector of quality scores and assume the lengths all differ
  m_Qscores.clear ();
  m_BlockReadLength = g_READ_LENGTH_VARIABLE;

  //  Check if EOF has already been reached
  if (m_Text_In.eof ()) {
    return (g_EOF_REACHED);
  }
  
  //  Read in the quality scores, a line at a time
  for (i = 0; i < blocksize; i++) {
    if (!getline (m_Text_In, tmp)) {
      break;
    }
    
    if (i == 0) {
      read_length = tmp.length ();
    }
    else {
      if (tmp.length () != read_length) {
        lengths_same = false;
      }
    }

    //  Change the second argument to "true" to set to debug mode
    m_Qscores.push_back (QScoresSingle (tmp, false));
    num_qscores++;
  }

  //  If the number of quality scores read is 0, then we indicate EOF was reached.  This is needed when the number 
  //  of reads is evenly divisible by the blocksize since we need to do a "getline ()" to realize the EOF was reached.
  if (num_qscores == 0) {
    return (g_EOF_REACHED);
  }

  //  Resize the array if the number of quality scores read is less than the blocksize
  if (num_qscores != blocksize) {
    m_Qscores.resize (num_qscores);
  }

  //  All reads are the same length, so set the block's length
  if (lengths_same) {
    m_BlockReadLength = read_length;
  }

  return (num_qscores);
}


/*!
     Write out a block of quality scores to file, which are each terminated by a newline.  Note that the quality 
     scores could include the newline character, which would make the location of the newline character ambigious.
     
     Values are 0-based.  To make them 1-based, change UnapplyFinalize () in QScoresSingle.
*/
void QScores::WriteOutFileBlock () {
  unsigned int num_qscores = 0;
  string tmp;
  
  for (num_qscores = 0; num_qscores < m_Qscores.size (); num_qscores++) {
//     cerr << num_qscores << "\t" << m_Qscores[num_qscores] << endl;
    m_Text_Out << m_Qscores[num_qscores] << endl;
  }

  return;
}

