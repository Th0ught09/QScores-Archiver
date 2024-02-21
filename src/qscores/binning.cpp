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
    \file binning.cpp
    Functions for binning which are not part of the QScores class.
*/
/*******************************************************************/

#include <vector>
#include <fstream>
#include <climits>  //  UINT_MAX
#include <iostream>  //  cerr
#include <cstdlib>  //  EXIT_SUCCESS, EXIT_FAILURE, exit ()
#include <math.h>  //  pow

using namespace std;

#include "common.hpp"
#include "bitbuffer.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "binning.hpp"

//  -----------------------------------------------------------------
//  QS-binning functions
//  -----------------------------------------------------------------

/*!
     Generate a lookup table based on equally-sized binning of the quality scores.

     \param[in] step_size The size of each bin
     \return The lookup table as a vector of unsigned integers.  Note that positions in this array before the offset is uninitialized!
*/
vector<unsigned int> GenerateLookup_LogBinning (e_QSCORES_MAP mode, unsigned int step_size) {
  vector<unsigned int> lookup (g_MAX_ASCII);
  unsigned int offset = 0;
  unsigned int max = 0;

  //  Initialize the lookup table so that invalid values will cause a segmentation fault
  for (unsigned int i = 0; i < g_MAX_ASCII; i++) {
    lookup[i] = UINT_MAX;
  }
  
  switch (mode) {
    case e_QSCORES_MAP_SANGER : 
      offset = g_SANGER_OFFSET;
      max = g_SANGER_MAX;
      break;
    case e_QSCORES_MAP_SOLEXA :
      offset = g_SOLEXA_OFFSET;
      max = g_SOLEXA_MAX;
      break;
    case e_QSCORES_MAP_ILLUMINA :
      offset = g_ILLUMINA_OFFSET;
      max = g_ILLUMINA_MAX;
      break;
    default :
      cerr << "EE\tA valid mapping mode is required with the --mapping option." << endl;
      exit (EXIT_FAILURE);
  }

  //  This function will work if i started from 0; the difference of (i - g_SANGER_OFFSET)
  //  would be negative, but since negative numbers can be %'ed, there are no problems.
  //  Otherwise, everything before offset has *not* been initialized.
  unsigned int k = 0;  //  First valid value is 1 so the k++ is done in the beginning
  for (unsigned int i = offset; i < g_MAX_ASCII; i++) {
    if (((i - offset) % step_size) == 0) {
      k++;
    }
    lookup[i] = k;
    if (i > max) {
      break;
    }
  }

  return lookup;
}


/*!
     Generate the reverse lookup table based on equally-sized binning of the quality scores.

     \param[in] step_size The size of each bin
     \return The lookup table as a vector of unsigned integers.
*/
vector<unsigned int> GenerateReverseLookup_LogBinning (e_QSCORES_MAP mode, unsigned int step_size) {
  vector<unsigned int> lookup (g_MAX_ASCII);
  unsigned int offset = 0;
  unsigned int max = 0;

  //  Initialize the lookup table so that invalid values will cause a segmentation fault
  for (unsigned int i = 0; i < g_MAX_ASCII; i++) {
    lookup[i] = UINT_MAX;
  }
  
  switch (mode) {
    case e_QSCORES_MAP_SANGER : 
      offset = g_SANGER_OFFSET;
      max = g_SANGER_MAX;
      break;
    case e_QSCORES_MAP_SOLEXA :
      offset = g_SOLEXA_OFFSET;
      max = g_SOLEXA_MAX;
      break;
    case e_QSCORES_MAP_ILLUMINA :
      offset = g_ILLUMINA_OFFSET;
      max = g_ILLUMINA_MAX;
      break;
    default :
      cerr << "EE\tA valid mapping mode is required with the --mapping option." << endl;
      exit (EXIT_FAILURE);
  }

  unsigned int k = offset;
  for (unsigned int i = 1; i < g_MAX_ASCII; i++, k += step_size) {
    if (k > max) {
      break;
    }
    lookup[i] = k;
  }

  return lookup;
}


//  -----------------------------------------------------------------
//  EP-binning functions
//  -----------------------------------------------------------------

/*!
     Generate a lookup table based on equally-sized binning of the probabilities.  Ranges are
     allocated as [x, y) -- that is, equivalence is checked for the lower bound and not the upper
     bound.  The error probability 1.000 will be currently placed in the first bin, though, since
     that is our first bin.

     \param[in] step_size The size of each bin
     \return The lookup table as a vector of unsigned integers.  Note that positions in this array before the offset is uninitialized!
*/
vector<unsigned int> GenerateLookup_UniBinning (e_QSCORES_MAP mode, unsigned int step_size) {
  vector<unsigned int> lookup (g_MAX_ASCII);
  unsigned int offset = 0;

  //  Initialize the lookup table so that invalid values will cause a segmentation fault
  for (unsigned int i = 0; i < g_MAX_ASCII; i++) {
    lookup[i] = UINT_MAX;
  }

  switch (mode) {
    case e_QSCORES_MAP_SANGER : 
      offset = g_SANGER_OFFSET;
      break;
    case e_QSCORES_MAP_SOLEXA :
      offset = g_SOLEXA_OFFSET;
      break;
    case e_QSCORES_MAP_ILLUMINA :
      offset = g_ILLUMINA_OFFSET;
      break;
    default :
      cerr << "EE\tA valid mapping mode is required with the --mapping option." << endl;
      exit (EXIT_FAILURE);
  }

  vector<double> regions (step_size);
  double region_size = static_cast<double> (1) / static_cast<double> (step_size);
  for (unsigned int i = 0; i < step_size; i++) {
    regions[i] = static_cast<double> (i) * region_size;
  }

  unsigned int k = 1;  //  First bin is #1
  unsigned int pos = step_size - 1;  //  Start off at the end of the array (i.e., closer to 1.000)
  for (unsigned int i = offset; i < g_MAX_ASCII; i++) {
    double prob = pow (static_cast<double> (10), (static_cast<double> ((i - offset)) / static_cast<double> (-10)));

    //  If the probability is less than the current region, we need to move to the next bin
    if (prob < regions[pos]) {
      while (prob < regions[pos]) {
        pos--;
      }
      k++;  //  Move to the next bin
    }
    lookup[i] = k;
  }

  return (lookup);
}


/*!
     Generate the reverse lookup table based on equally-sized binning of the probabilities.

     \param[in] step_size The size of each bin
     \return The lookup table as a vector of unsigned integers.
*/
vector<unsigned int> GenerateReverseLookup_UniBinning (e_QSCORES_MAP mode, unsigned int step_size) {
  vector<unsigned int> lookup (g_MAX_ASCII);
  unsigned int offset = 0;
  unsigned int max = 0;

  //  Initialize the lookup table so that invalid values will cause a segmentation fault
  for (unsigned int i = 0; i < g_MAX_ASCII; i++) {
    lookup[i] = UINT_MAX;
  }

  switch (mode) {
    case e_QSCORES_MAP_SANGER : 
      offset = g_SANGER_OFFSET;
      max = g_SANGER_MAX;
      break;
    case e_QSCORES_MAP_SOLEXA :
      offset = g_SOLEXA_OFFSET;
      max = g_SOLEXA_MAX;
      break;
    case e_QSCORES_MAP_ILLUMINA :
      offset = g_ILLUMINA_OFFSET;
      max = g_ILLUMINA_MAX;
      break;
    default :
      cerr << "EE\tA valid mapping mode is required with the --mapping option." << endl;
      exit (EXIT_FAILURE);
  }

  vector<double> regions (step_size);
  double region_size = static_cast<double> (1) / static_cast<double> (step_size);
  for (unsigned int i = 0; i < step_size; i++) {
    regions[i] = static_cast<double> (i) * region_size;
  }

  unsigned int k = 1;
  unsigned int pos = step_size - 1;
  for (unsigned int i = offset; i < g_MAX_ASCII; i++) {
    double prob = pow (static_cast<double> (10), (static_cast<double> ((i - offset)) / static_cast<double> (-10)));
    if (prob < regions[pos]) {
      while (prob < regions[pos]) {
        pos--;
      }
      k++;
    }
    if (lookup[k] == UINT_MAX) {
      lookup[k] = i;
    }
    if (i > max) {
      break;
    }
  }

  return (lookup);
}
