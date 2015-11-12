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
    \file transform.cpp
    Transformation functions for a block.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: transform.cpp 224 2011-11-06 15:32:09Z rwan $
*/
/*******************************************************************/

#include <string>
#include <vector>
#include <fstream>
#include <climits>  //  UINT_MAX
#include <iostream>  //  cerr
#include <cstdlib>  //  EXIT_SUCCESS, EXIT_FAILURE, exit ()

using namespace std;

#include "common.hpp"
#include "external-software.hpp"
#include "block-statistics.hpp"
#include "bitbuffer.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "qscores-settings.hpp"
#include "binning.hpp"
#include "qscores.hpp"


//  -----------------------------------------------------------------
//  Main public member functions
//  -----------------------------------------------------------------


/*!
     Perform a "check" of lossy binning; basically print out the bin assignments.     
*/
void QScores::PerformBinningCheck () {
  vector<unsigned int> lossy_mapping;
  e_QSCORES_MAP mode = m_QScoresSettings.GetQScoresMapping ();

  //  Valid mapping scheme required
  if (mode == e_QSCORES_MAP_UNSET) {
    cerr << "EE\tA valid mapping mode is required with the --mapping option." << endl;
    exit (EXIT_FAILURE);
  }
  
  //  Ensure that either --logbin or --unibin were specified
  if ((m_QScoresSettings.GetLossyLogBinning ()) && (m_QScoresSettings.GetLossyUniBinning ())) {
    cerr << "EE\tEither --logbin or --unibin is required with the --checkbin option." << endl;
    exit (EXIT_FAILURE);
  }
  
  //  Verify the lossy settings
  if (!m_QScoresSettings.CheckLossySettings ()) {
    exit (EXIT_FAILURE);
  }

  unsigned int min = m_QScoresSettings.GetQScoresMappingMin ();
  unsigned int max = m_QScoresSettings.GetQScoresMappingMax ();

  if (m_QScoresSettings.GetLossyLogBinningParameter () != -1) {
    lossy_mapping = GenerateLookup_LogBinning (mode, m_QScoresSettings.GetLossyLogBinningParameter ());
  }
  else if (m_QScoresSettings.GetLossyUniBinningParameter () != -1) {
    lossy_mapping = GenerateLookup_UniBinning (mode, m_QScoresSettings.GetLossyUniBinningParameter ());
  }
  else {
    cerr << "EE\tEither --logbin or --unibin is required with a non-negative parameter." << endl;
    exit (EXIT_FAILURE);
  }

  for (unsigned int i = min; i <= max; i++) {
    cout << i << "\t" << lossy_mapping[i] << endl;
  }

  return;
}


/*!
     Perform an "uncheck" of lossy binning; basically print out the bin assignments.     
*/
void QScores::PerformUnbinningCheck () {
  vector<unsigned int> lossy_mapping;
  e_QSCORES_MAP mode = m_QScoresSettings.GetQScoresMapping ();

  //  Valid mapping scheme required
  if (mode == e_QSCORES_MAP_UNSET) {
    cerr << "EE\tA valid mapping mode is required with the --mapping option." << endl;
    exit (EXIT_FAILURE);
  }
  
  //  Ensure that either --logbin or --unibin were specified
  if ((m_QScoresSettings.GetLossyLogBinning ()) && (m_QScoresSettings.GetLossyUniBinning ())) {
    cerr << "EE\tEither --logbin or --unibin is required with the --checkunbin option." << endl;
    exit (EXIT_FAILURE);
  }
  
  //  Verify the lossy settings
  if (!m_QScoresSettings.CheckLossySettings ()) {
    exit (EXIT_FAILURE);
  }

  unsigned int min = 1;
  unsigned int max = m_QScoresSettings.GetQScoresMappingMax ();

  if (m_QScoresSettings.GetLossyLogBinningParameter () != -1) {
    lossy_mapping = GenerateReverseLookup_LogBinning (mode, m_QScoresSettings.GetLossyLogBinningParameter ());
  }
  else if (m_QScoresSettings.GetLossyUniBinningParameter () != -1) {
    lossy_mapping = GenerateReverseLookup_UniBinning (mode, m_QScoresSettings.GetLossyUniBinningParameter ());
  }
  else {
    cerr << "EE\tEither --logbin or --unibin is required with a non-negative parameter." << endl;
    exit (EXIT_FAILURE);
  }

  //  max is less than g_MAX_ASCII since not all of the 255 positions are used
  for (unsigned int i = min; i < g_MAX_ASCII; i++) {
    if (lossy_mapping[i] > max) {
      break;
    }
    cout << i << "\t" << lossy_mapping[i] << endl;
  }

  return;
}


/*!
     Preprocess a block of quality scores.  Several passes over the block are made:

     1)  Map the quality scores to integers and apply lossy transformations
     2)  Rescaling requires a single pass to find the overall minimum in the block.
     3)  Perform the actual rescaling.
     4)  Collect statistics over the block if Remapping was selected.
     5)  Perform remapping and then finalize.

     Some of the loops can be merged -- for example, step 4 can be added to the end of step 3 to
     save one loop.  However, we've separated it into 5 loops to divide up the process and
     (hopefully) make this source code easier to read.

     \param[in] current_blocksize The size of the current block
*/
void QScores::PreprocessBlock (int current_blocksize) {
  vector<unsigned int> lossy_mapping;
  vector<unsigned int> lossless_mapping;

  //  Calculate the lookup table of lossy mappings, once per block
  if (m_QScoresSettings.GetLossyLogBinning ()) {
    lossy_mapping = GenerateLookup_LogBinning (m_QScoresSettings.GetQScoresMapping (), static_cast<unsigned int> (m_QScoresSettings.GetLossyLogBinningParameter ()));
  }
  else if (m_QScoresSettings.GetLossyUniBinning ()) {
    lossy_mapping = GenerateLookup_UniBinning (m_QScoresSettings.GetQScoresMapping (), static_cast<unsigned int> (m_QScoresSettings.GetLossyUniBinningParameter ()));
  }

  //  Map quality scores to integers and then perform lossy transformations
  for (int i = 0; i < current_blocksize; i++) {
    //  Map quality scores to ASCII
    m_Qscores[i].QScoreToInt ();

    //  Lossy transformations, order does not matter since they do not conflict
    if (m_QScoresSettings.GetLossyMinTruncation ()) {
      m_Qscores[i].ApplyLossyMinTruncation (static_cast<unsigned int> (m_QScoresSettings.GetLossyMinTruncationParameter ()));
    }
    else if (m_QScoresSettings.GetLossyMaxTruncation ()) {
      m_Qscores[i].ApplyLossyMaxTruncation (static_cast<unsigned int> (m_QScoresSettings.GetLossyMaxTruncationParameter ()));
    }
    else if (m_QScoresSettings.GetLossyLogBinning ()) {
      m_Qscores[i].ApplyLossyRemapping (lossy_mapping);
    }
    else if (m_QScoresSettings.GetLossyUniBinning ()) {
      m_Qscores[i].ApplyLossyRemapping (lossy_mapping);
    }
  }

  //  Lossless transformation -- apply difference coding
  if (m_QScoresSettings.GetTransformGapTrans ()) {
    unsigned int previous = UINT_MAX;
    for (int i = 0; i < current_blocksize; i++) {
      previous = m_Qscores[i].ApplyDifferenceCoding (previous);
    }
  }

  //  Lossless transformation -- need to find the minimum for rescaling
  m_BlockMinimum = UINT_MAX;
  if (m_QScoresSettings.GetTransformMinShift ()) {
    for (int i = 0; i < current_blocksize; i++) {
      m_Qscores[i].SetMinMax ();
      unsigned int tmp = m_Qscores[i].GetMin ();
      if (tmp < m_BlockMinimum) {
        m_BlockMinimum = tmp;
      }
    }
  }

  //  Lossless transformation -- perform rescaling
  if ((m_QScoresSettings.GetTransformMinShift ()) && (m_BlockMinimum != 0)) {
    for (int i = 0; i < current_blocksize; i++) {
      m_Qscores[i].ApplyRescaling (m_BlockMinimum);
    }
  }
  
  //  Lossless transformation -- need to prepare to collect the statistics for remapping
  if (m_QScoresSettings.GetTransformFreqOrder ()) {
    m_BlockStatistics.Initialize ();

    for (int i = 0; i < current_blocksize; i++) {
      m_BlockStatistics.UpdateFrequencyTable (m_Qscores[i].GetQScoreInt ());
    }
    lossless_mapping = m_BlockStatistics.GetQScoresToIDs ();
  }
  
  //  Lossless transformation -- Perform remapping (if requested) and then finalize
  for (int i = 0; i < current_blocksize; i++) {
    if (m_QScoresSettings.GetTransformFreqOrder ()) {
      m_Qscores[i].ApplyLosslessRemapping (lossless_mapping);
    }

    //  Finalize (required) -- shift by 1 to prevent a 0 from ever appearing beyond this point
//     m_Qscores[i].ApplyFinalize ();
  }
  
  return;
}


/*!
     Reverse the work done by PreprocessBlock ().

     \param[in] current_blocksize The size of the current block
*/
void QScores::UnPreprocessBlock (int current_blocksize) {
  vector<unsigned int> lossy_mapping;
  vector<unsigned int> lossless_remapping;
  unsigned int previous = UINT_MAX;
  
  //  Calculate the lookup table of lossy mappings, once per block
  if (m_QScoresSettings.GetLossyLogBinning ()) {
    lossy_mapping = GenerateReverseLookup_LogBinning (m_QScoresSettings.GetQScoresMapping (), static_cast<unsigned int> (m_QScoresSettings.GetLossyLogBinningParameter ()));
  }
  else if (m_QScoresSettings.GetLossyUniBinning ()) {
    lossy_mapping = GenerateReverseLookup_UniBinning (m_QScoresSettings.GetQScoresMapping (), static_cast<unsigned int> (m_QScoresSettings.GetLossyUniBinningParameter ()));
  }
  
  if (m_QScoresSettings.GetTransformFreqOrder ()) {
    lossless_remapping = m_BlockStatistics.GetIDsToQScores ();
  }
  //  Reverse everything done by PreprocessBlock (), except for the lossy transformations
  for (int i = 0; i < current_blocksize; i++) {
//     m_Qscores[i].UnapplyFinalize ();

    if (m_QScoresSettings.GetTransformFreqOrder ()) {
      m_Qscores[i].UnapplyLosslessRemapping (lossless_remapping);
    }
    
    if ((m_QScoresSettings.GetTransformMinShift ()) && (m_BlockMinimum != 0)) {
      m_Qscores[i].UnapplyRescaling (m_BlockMinimum);
    }

    if (m_QScoresSettings.GetTransformGapTrans ()) {
      previous = m_Qscores[i].UnapplyDifferenceCoding (previous);
    }

    //  Lossy transformations
    if (m_QScoresSettings.GetLossyLogBinning ()) {
      m_Qscores[i].UnapplyLossyRemapping (lossy_mapping);
    }
    else if (m_QScoresSettings.GetLossyUniBinning ()) {
      m_Qscores[i].UnapplyLossyRemapping (lossy_mapping);
    }

    //  Map quality scores to ASCII
    m_Qscores[i].IntToQScore ();
  }
  
  return;
}

