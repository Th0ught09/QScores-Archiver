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
    \file mutators.cpp
    Mutating member functions for ExternalSoftware class definition .

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
    $Id: mutators.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <vector>
#include <string>
#include <cassert>  //  assert
#include <iostream>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

#include "external-software-local.hpp"
#include "external-software.hpp"


//  -----------------------------------------------------------------
//  General options
//  -----------------------------------------------------------------


/*!
     Indicate debugging output is required.

     \return Boolean value for success/failure.
*/
bool ExternalSoftware::SetDebug () {
  m_Debug = true;
  return true;
}


/*!
     Indicate that we have initialized already.

     \return Boolean value for success/failure.
*/
bool ExternalSoftware::SetInitializePaths () {
  m_InitializePaths = true;
  return true;
}


/*!
     Set the gzip command

     \return Always returns true.
*/
bool ExternalSoftware::SetGzipCommand (string cmd) {
  m_GzipCommand = cmd;
  return true;
}


/*!
     Set the gunzip command

     \return Always returns true.
*/
bool ExternalSoftware::SetGunzipCommand (string cmd) {
  m_GunzipCommand = cmd;
  return true;
}


/*!
     Set the bzip command

     \return Always returns true.
*/
bool ExternalSoftware::SetBzipCommand (string cmd) {
  m_BzipCommand = cmd;
  return true;
}


/*!
     Set the bunzip command

     \return Always returns true.
*/
bool ExternalSoftware::SetBunzipCommand (string cmd) {
  m_BunzipCommand = cmd;
  return true;
}


/*!
     Set the Re-Pair command

     \return Always returns true.
*/
bool ExternalSoftware::SetRepairCommand (string cmd) {
  m_RepairCommand = cmd;
  return true;
}


/*!
     Set the Des-Pair command

     \return Always returns true.
*/
bool ExternalSoftware::SetDespairCommand (string cmd) {
  m_DespairCommand = cmd;
  return true;
}


/*!
     Set the shuff command

     \return Always returns true.
*/
bool ExternalSoftware::SetShuffCommand (string cmd) {
  m_ShuffCommand = cmd;
  return true;
}


/*!
     Add a set of search paths to the list of paths
*/
void ExternalSoftware::AddSearchPath (string paths) {
  string result = "";
  vector<string>::const_iterator path_iter;

  char_separator<char> separators (",");
  tokenizer<boost::char_separator<char> > tokens (paths, separators);
  for (tokenizer<char_separator<char> >::iterator token_iter = tokens.begin (); token_iter != tokens.end (); token_iter++){
    bool found = false;
    for (path_iter = m_SearchPaths.begin (); path_iter != m_SearchPaths.end(); path_iter++) {
//       cerr << *token_iter << " | " << *path_iter << endl;
      //  Path already exists
      if (*path_iter == *token_iter) {
        found = true;
        break;
      }
    }
    if (!found) {
      m_SearchPaths.push_back (*token_iter);
    }
  }
  
  return;
}

