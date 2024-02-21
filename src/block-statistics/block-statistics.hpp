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
    \file block-statistics.hpp
    Header file for BlockStatistics class.
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


