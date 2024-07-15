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
    \file testing.cpp
    Test functions QScoresSingle class.
*/
/*******************************************************************/


#include <cstdlib>  //  EXIT_SUCCESS, EXIT_FAILURE, exit ()
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
#include <climits>  //  UINT_MAX

using namespace std;

#include "bitbuffer.hpp"
#include "block-statistics.hpp"
#include "QScoresSingle_Config.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "binning.hpp"
#include "testing.hpp"



/*!
     Test rescaling (minshifting)

     \param[in] str Input quality score
*/
void TestRescaling (string str) {
  QScoresSingle tmp (str, true);
  tmp.PrintQScore ();

  tmp.QScoreToInt ();
  tmp.PrintQScore ();

  tmp.SetMinMax ();
  tmp.ApplyRescaling (tmp.GetMin ());
  tmp.PrintQScore ();

  cerr << "===================================" << endl;

  tmp.UnapplyRescaling (tmp.GetMin ());
  tmp.PrintQScore ();

  tmp.IntToQScore ();
  tmp.PrintQScore ();

  return;
}


/*!
     Test remapping (freqordering)

     \param[in] str Input quality score
*/
void TestRemapping (string str) {
  BlockStatistics blockstats;
  vector<unsigned int> lookup (0xFF);
  vector<unsigned int> lookup_rev (0xFF);

  QScoresSingle tmp (str, true);
  tmp.PrintQScore ();

  tmp.QScoreToInt ();
  tmp.PrintQScore ();

  blockstats.Initialize ();
  blockstats.UpdateFrequencyTable (tmp.GetQScoreInt ());
  lookup = blockstats.GetQScoresToIDs ();
  tmp.ApplyLosslessRemapping (lookup);
  tmp.PrintQScore ();

  cerr << "===================================" << endl;

  lookup_rev = blockstats.GetIDsToQScores ();
  tmp.UnapplyLosslessRemapping (lookup_rev);
  tmp.PrintQScore ();

  tmp.IntToQScore ();
  tmp.PrintQScore ();

  return;
}


/*!
     Test difference coding (gaptrans)

     \param[in] str Input quality score
*/
void TestDifferenceCoding (string str) {
  QScoresSingle tmp (str, true);
  tmp.PrintQScore ();

  tmp.QScoreToInt ();
  tmp.PrintQScore ();

  tmp.SetMinMax ();
  tmp.ApplyDifferenceCoding (UINT_MAX);
  tmp.PrintQScore ();

  cerr << "===================================" << endl;

  tmp.UnapplyDifferenceCoding (UINT_MAX);
  tmp.PrintQScore ();

  tmp.IntToQScore ();
  tmp.PrintQScore ();
  
  return;
}


/*!
     Test QSBinning (logbin)

     \param[in] str Input quality score
     \param[in] mode The quality scores mapping mode
     \param[in] bin Parameter for qsbinning
*/
void TestQSBinning (string str, enum e_QSCORES_MAP mode, unsigned int bin) {
  vector<unsigned int> lookup (0xFF);
  vector<unsigned int> lookup_rev (0xFF);

  lookup = GenerateLookup_QScoreBinning (mode, bin);
  lookup_rev = GenerateReverseLookup_QScoreBinning (mode, bin);
  
  QScoresSingle tmp (str, true);
  tmp.PrintQScore ();

  tmp.QScoreToInt ();
  tmp.PrintQScore ();

  tmp.ApplyLossyRemapping (lookup);
  tmp.PrintQScore ();

//   tmp.SetMinMax ();
//   tmp.ApplyDifferenceCoding (UINT_MAX);
  tmp.PrintQScore ();

  cerr << "===================================" << endl;

  tmp.UnapplyLossyRemapping (lookup_rev);
  tmp.PrintQScore ();

  tmp.IntToQScore ();
  tmp.PrintQScore ();
  
  return;
}


/*!
     Test EPBinning (unibin)

     \param[in] str Input quality score
     \param[in] mode The quality scores mapping mode
     \param[in] bin Parameter for qsbinning
*/
void TestEPBinning (string str, enum e_QSCORES_MAP mode, unsigned int bin) {
  vector<unsigned int> lookup (0xFF);
  vector<unsigned int> lookup_rev (0xFF);

  lookup = GenerateLookup_ErrProbBinning (mode, bin);
  lookup_rev = GenerateReverseLookup_ErrProbBinning (mode, bin);

  QScoresSingle tmp (str, true);
  tmp.PrintQScore ();

  tmp.QScoreToInt ();
  tmp.PrintQScore ();

  tmp.ApplyLossyRemapping (lookup);
  tmp.PrintQScore ();

//   tmp.SetMinMax ();
//   tmp.ApplyDifferenceCoding (UINT_MAX);
  tmp.PrintQScore ();

  cerr << "===================================" << endl;

  tmp.UnapplyLossyRemapping (lookup_rev);
  tmp.PrintQScore ();

  tmp.IntToQScore ();
  tmp.PrintQScore ();
  
  return;
}


/*!
     Test QSBinning (logbin) with difference coding (gaptrans)

     \param[in] str Input quality score
     \param[in] mode The quality scores mapping mode
     \param[in] bin Parameter for qsbinning
*/
void TestQSBinningDifferenceCoding (string str, enum e_QSCORES_MAP mode, unsigned int bin) {
  vector<unsigned int> lookup (0xFF);
  vector<unsigned int> lookup_rev (0xFF);

  lookup = GenerateLookup_QScoreBinning (mode, bin);
  lookup_rev = GenerateReverseLookup_QScoreBinning (mode, bin);
  
  QScoresSingle tmp (str, true);
  tmp.PrintQScore ();

  tmp.QScoreToInt ();
  tmp.PrintQScore ();

  tmp.ApplyLossyRemapping (lookup);
  tmp.PrintQScore ();

  tmp.SetMinMax ();
  tmp.ApplyDifferenceCoding (UINT_MAX);
  tmp.PrintQScore ();

  cerr << "===================================" << endl;

  tmp.UnapplyDifferenceCoding (UINT_MAX);
  tmp.PrintQScore ();
  
  tmp.UnapplyLossyRemapping (lookup_rev);
  tmp.PrintQScore ();

  tmp.IntToQScore ();
  tmp.PrintQScore ();
  
  return;
}
