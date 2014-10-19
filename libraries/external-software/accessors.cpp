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
    \file accessors.cpp
    Accessing member functions for ExternalSoftware class definition .

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: accessors.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <vector>
#include <string>
#include <iterator>

using namespace std;

#include "external-software-local.hpp"
#include "external-software.hpp"



//  -----------------------------------------------------------------
//  General options
//  -----------------------------------------------------------------

/*!
     Get the debug setting.

     \return Boolean value representing the setting.
*/
bool ExternalSoftware::GetDebug () const {
  return (m_Debug);
}


/*!
     Get the initialize setting.

     \return Boolean value representing the setting.
*/
bool ExternalSoftware::GetInitializePaths () const {
  return (m_InitializePaths);
}


/*!
     Test if the gzip command was found

     \return Boolean indicating if the path exists.
*/
bool ExternalSoftware::GetGzipCommand () const {
  if (m_GzipCommand.length () == 0) {
    return (false);
  }
  return (true);
}


/*!
     Test if the gunzip command was found

     \return Boolean indicating if the path exists.
*/
bool ExternalSoftware::GetGunzipCommand () const {
  if (m_GunzipCommand.length () == 0) {
    return (false);
  }
  return (true);
}


/*!
     Test if the bzip command was found

     \return Boolean indicating if the path exists.
*/
bool ExternalSoftware::GetBzipCommand () const {
  if (m_BzipCommand.length () == 0) {
    return (false);
  }
  return (true);
}


/*!
     Test if the bunzip command was found

     \return Boolean indicating if the path exists.
*/
bool ExternalSoftware::GetBunzipCommand () const {
  if (m_BunzipCommand.length () == 0) {
    return (false);
  }
  return (true);
}


/*!
     Test if the Re-Pair command was found

     \return Boolean indicating if the path exists.
*/
bool ExternalSoftware::GetRepairCommand () const {
  if (m_RepairCommand.length () == 0) {
    return (false);
  }
  return (true);
}


/*!
     Test if the Des-Pair command was found

     \return Boolean indicating if the path exists.
*/
bool ExternalSoftware::GetDespairCommand () const {
  if (m_DespairCommand.length () == 0) {
    return (false);
  }
  return (true);
}


/*!
     Test if the shuff command was found

     \return Boolean indicating if the path exists.
*/
bool ExternalSoftware::GetShuffCommand () const {
  if (m_ShuffCommand.length () == 0) {
    return (false);
  }
  return (true);
}


/*!
     Get the gzip command

     \return Command as a string.
*/
string ExternalSoftware::GetGzipCommandPath () const {
  if (m_GzipCommand.length () == 0) {
    return (g_COMMAND_NOT_FOUND_STR);
  }
  return (m_GzipCommand);
}


/*!
     Get the gunzip command

     \return Command as a string.
*/
string ExternalSoftware::GetGunzipCommandPath () const {
  if (m_GunzipCommand.length () == 0) {
    return (g_COMMAND_NOT_FOUND_STR);
  }
  return (m_GunzipCommand);
}


/*!
     Get the bzip command

     \return Command as a string.
*/
string ExternalSoftware::GetBzipCommandPath () const {
  if (m_BzipCommand.length () == 0) {
    return (g_COMMAND_NOT_FOUND_STR);
  }
  return (m_BzipCommand);
}


/*!
     Get the bunzip command

     \return Command as a string.
*/
string ExternalSoftware::GetBunzipCommandPath () const {
  if (m_BunzipCommand.length () == 0) {
    return (g_COMMAND_NOT_FOUND_STR);
  }
  return (m_BunzipCommand);
}


/*!
     Get the Re-Pair command

     \return Command as a string.
*/
string ExternalSoftware::GetRepairCommandPath () const {
  if (m_RepairCommand.length () == 0) {
    return (g_COMMAND_NOT_FOUND_STR);
  }
  return (m_RepairCommand);
}


/*!
     Get the Des-Pair command

     \return Command as a string.
*/
string ExternalSoftware::GetDespairCommandPath () const {
  if (m_DespairCommand.length () == 0) {
    return (g_COMMAND_NOT_FOUND_STR);
  }
  return (m_DespairCommand);
}


/*!
     Get the shuff command

     \return Command as a string.
*/
string ExternalSoftware::GetShuffCommandPath () const {
  if (m_ShuffCommand.length () == 0) {
    return (g_COMMAND_NOT_FOUND_STR);
  }
  return (m_ShuffCommand);
}


/*!
     Show the search paths

     \return Search paths as a string.
*/
string ExternalSoftware::ShowSearchPaths () const {
  string result = "";
  vector<string>::const_iterator iter;

  //  The search paths is empty
  if (m_SearchPaths.size () == 0) {
    return (g_COMMAND_NOT_FOUND_STR);
  }
  
  iter = m_SearchPaths.begin ();
  result = *iter;
  iter++;
  for (; iter != m_SearchPaths.end(); iter++) {
    result = result + ";" + *iter;
  }
  result = result + "\n";
  
  return (result);
}


