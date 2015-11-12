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
    \file accessors.cpp
    Accessing member functions for QScores class definition .
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: accessors.cpp 224 2011-11-06 15:32:09Z rwan $
*/
/*******************************************************************/

#include <fstream>
#include <string>
#include <climits>  //  UINT_MAX
#include <vector>
#include <iostream>

using namespace std;

#include "external-software.hpp"
#include "block-statistics.hpp"
#include "bitbuffer.hpp"
#include "qscores-single-defn.hpp"
#include "qscores-single.hpp"
#include "qscores-settings.hpp"
#include "qscores-defn.hpp"
#include "qscores.hpp"


//  -----------------------------------------------------------------
//  General options
//  -----------------------------------------------------------------

/*!
     Get the debug setting.
     
     \return Boolean value representing the setting.
*/
bool QScores::GetDebug () const {
  return (m_Debug);
}


/*!
     Get the verbose setting.
     
     \return Boolean value representing the setting.
*/
bool QScores::GetVerbose () const {
  return (m_Verbose);
}


/*!
     Get the encode setting.
     
     \return Boolean value representing the setting.
*/
bool QScores::GetEncode () const {
  return (m_Encode);
}


/*!
     Get the decode setting.
     
     \return Boolean value representing the setting.
*/
bool QScores::GetDecode () const {
  return (m_Decode);
}


/*!
     Get the external software check setting.
     
     \return Boolean value representing the setting.
*/
bool QScores::GetExternalSoftwareCheck () const {
  return (m_ExternalSoftwareCheck);
}


/*!
     Get the binning check setting.
     
     \return Boolean value representing the setting.
*/
bool QScores::GetBinningCheck () const {
  return (m_BinningCheck);
}


/*!
     Get the unbinning check setting.
     
     \return Boolean value representing the setting.
*/
bool QScores::GetUnbinningCheck () const {
  return (m_UnbinningCheck);
}


/*!
     Get the blocksize.

     \return Integer representing the setting.
*/
int QScores::GetBlocksize () const {
  return (m_Blocksize);
}
