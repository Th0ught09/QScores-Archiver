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
    \file qscores-set.hpp
    Header file for functions for binning which are not part of the QScores class.
*/
/*******************************************************************/

#ifndef BINNING_HPP
#define BINNING_HPP

vector<unsigned int> GenerateLookup_LogBinning (e_QSCORES_MAP mode, unsigned int step_size);
vector<unsigned int> GenerateReverseLookup_LogBinning (e_QSCORES_MAP mode, unsigned int step_size);

vector<unsigned int> GenerateLookup_UniBinning (e_QSCORES_MAP mode, unsigned int step_size);
vector<unsigned int> GenerateReverseLookup_UniBinning (e_QSCORES_MAP mode, unsigned int step_size);

#endif

