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
    \file gzip-zlib.cpp
    Main processing functions for gzip/zlib.

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: setup.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <vector>
#include <string>
#include <iterator>
#include <iostream>
#include <string.h>
#include <cstdlib>
#include <cstdio>  //  fopen
#include <climits>
#include <fstream>

#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations

using namespace std;
namespace bfs = boost::filesystem;

#include "external-software-local.hpp"
#include "external-software-exception.hpp"
#include "external-software.hpp"


//  -----------------------------------------------------------------
//  Public functions
//  -----------------------------------------------------------------

void ExternalSoftware::Initialize (enum e_EXTERNAL_METHOD method, bool compress) {
  m_Method = method;
  m_Compress = compress;
  
  return;
}


void ExternalSoftware::UnInitialize () {
  m_DictionaryBufferPtr = 0;
  m_InBufferPtr = 0;
  m_InBufferRetrieval = 0;
  m_OutBufferPtr = 0;
  m_OutBufferRetrieval = 0;
  
  return;
}


//  -----------------------------------------------------------------
//  Private functions
//  -----------------------------------------------------------------

/*!
     Perform initialization of paths; if already done, does not do it again.
*/
void ExternalSoftware::InitializePaths () {
  vector<string>::const_iterator iter;

  if (GetInitializePaths ()) {
    return;
  }
  
  //  Search for each command.  We only set the path of commands that have not 
  //  yet been found.  This ensures that the order of the paths also defines
  //  the search priority.
  for (iter = m_SearchPaths.begin (); iter != m_SearchPaths.end(); iter++) {
    string tmp = *iter + "/" + g_COMMAND_GZIP;
    if (!GetGzipCommand ()) {
      bfs::path tmppath (tmp);
      if (bfs::exists (tmppath)) {
        SetGzipCommand (tmp);
      }
    }

    tmp = *iter + "/" + g_COMMAND_GUNZIP;
    if (!GetGunzipCommand ()) {
      bfs::path tmppath (tmp);
      if (bfs::exists (tmppath)) {
        SetGunzipCommand (tmp);
      }
    }

    tmp = *iter + "/" + g_COMMAND_BZIP;
    if (!GetBzipCommand ()) {
      bfs::path tmppath (tmp);
      if (bfs::exists (tmppath)) {
        SetBzipCommand (tmp);
      }
    }

    tmp = *iter + "/" + g_COMMAND_BUNZIP;
    if (!GetBunzipCommand ()) {
      bfs::path tmppath (tmp);
      if (bfs::exists (tmppath)) {
        SetBunzipCommand (tmp);
      }
    }

    tmp = *iter + "/" + g_COMMAND_REPAIR;
    if (!GetRepairCommand ()) {
      bfs::path tmppath (tmp);
      if (bfs::exists (tmppath)) {
        SetRepairCommand (tmp);
      }
    }

    tmp = *iter + "/" + g_COMMAND_DESPAIR;
    if (!GetDespairCommand ()) {
      bfs::path tmppath (tmp);
      if (bfs::exists (tmppath)) {
        SetDespairCommand (tmp);
      }
    }

    tmp = *iter + "/" + g_COMMAND_SHUFF;
    if (!GetShuffCommand ()) {
      bfs::path tmppath (tmp);
      if (bfs::exists (tmppath)) {
        SetShuffCommand (tmp);
      }
    }
  }

  SetInitializePaths ();
  return;
}


