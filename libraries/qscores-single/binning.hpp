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
    \file qscores-binning.hpp
    Copied from QScores class for testing.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: binning.hpp 224 2011-11-06 15:32:09Z rwan $
*/
/*******************************************************************/

#ifndef BINNING_HPP
#define BINNING_HPP

vector<unsigned int> GenerateLookup_QScoreBinning (e_QSCORES_MAP mode, unsigned int step_size);
vector<unsigned int> GenerateReverseLookup_QScoreBinning (e_QSCORES_MAP mode, unsigned int step_size);

vector<unsigned int> GenerateLookup_ErrProbBinning (e_QSCORES_MAP mode, unsigned int step_size);
vector<unsigned int> GenerateReverseLookup_ErrProbBinning (e_QSCORES_MAP mode, unsigned int step_size);

#endif

