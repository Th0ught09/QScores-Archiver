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
    \file block-statistics.hpp
    Header file for BlockStatistics class.

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan

    $Id: block-statistics.hpp 224 2011-11-06 15:32:09Z rwan $
*/
/*******************************************************************/

#ifndef BLOCK_STATISTICS_HPP
#define BLOCK_STATISTICS_HPP


/*!
    \struct FrequencyEntry

    \details An entry of the frequency table which lists the quality score, its frequency in the
             block, and the ID it corresponds to.
*/
struct FrequencyEntry {
  //!  Quality score
  unsigned int qscore;
  //!  Frequency of that quality score
  unsigned int freq;
  //!  ID to map the quality score to (1-based)
  unsigned int id;

  FrequencyEntry (unsigned int qscore_pos)
    : qscore (qscore_pos),
      freq (0),
      id (UINT_MAX)
        {
        }
};


/*!
    \class BlockStatistics

    \details Class used to represent the statistics for a block.
*/
class BlockStatistics {
  //  Friend function to print out statistics for debugging  [block-statistics.cpp]
  friend ostream &operator<< (ostream &os, const BlockStatistics& qs);

  public:
    //  Constructors/destructors  [block-statistics.cpp]
    BlockStatistics (bool debug=false);
    ~BlockStatistics ();
    void Initialize ();

    //  Accessors  [accessors.cpp]
    bool GetDebug () const;
    unsigned int GetFrequencyTableQScore (unsigned int pos) const;
    unsigned int GetFrequencyTableFreq (unsigned int pos) const;
    unsigned int GetFrequencyTableID (unsigned int pos) const;
    unsigned int GetIDsToQScoresSize () const;
    vector<unsigned int> GetQScoresToIDs ();
    vector<unsigned int> GetIDsToQScores ();

    //  Main processing functions  [process.cpp]
    void UpdateFrequencyTable (vector<unsigned int> x);
    void CopyIDsToQScores (vector<unsigned int> x);
  private:
    //  Main processing functions  [process.cpp]
    void ApplySort ();  //  Called by GetRemapping () only

    //!  Debug mode?
    bool m_Debug;

    //!  Is it sorted yet?
    bool m_IsSorted;

    //!  Table of frequency (may or may not be sorted)
    vector<FrequencyEntry> m_FrequencyTable;

    //!  Lookup table that maps quality scores to the new IDs (needed for encoding)
    vector<unsigned int> m_QScoresToIDs;

    //!  Lookup table that maps the new IDs to quality scores (needed for decoding)
    vector<unsigned int> m_IDsToQScores;

    //!  Size of m_NewIDsToQScore
    unsigned int m_IDsToQScoresSize;
};

#endif


