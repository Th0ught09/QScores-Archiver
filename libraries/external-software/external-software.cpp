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
    \file external-software.cpp
    Use of external compression programs.

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan

    $Id: external-software.cpp 224 2011-11-06 15:32:09Z rwan $
*/
/*******************************************************************/

#include "filesystem-v3.hpp"
#include <string>
#include <vector>
#include <iomanip>  //  setw
#include <iostream>
#include <cstdlib>

using namespace std;

#include "systemcfg.hpp"
#include "external-software-local.hpp"
#include "external-software.hpp"


/*!
     Constructor that takes one optional argument.

     \param[in] debug Set to true if in debug mode; false by default
*/
ExternalSoftware::ExternalSoftware (bool debug)
  : m_Debug (debug),
    m_Compress (true),
    m_Method (e_EXTERNAL_METHOD_UNSET),
    m_InitializePaths (false),
    m_SearchPaths (0),
    m_GzipCommand (""),
    m_GunzipCommand (""),
    m_BzipCommand (""),
    m_BunzipCommand (""),
    m_RepairCommand (""),
    m_DespairCommand (""),
    m_ShuffCommand (""),
#if ZLIB_FOUND
    m_ZStream (NULL),
#endif
#if BZIP2_FOUND
    m_BZStream (NULL),
#endif
    m_InBuffer (),
    m_InBufferPtr (0),
    m_InBufferRetrieval (0),
    m_InBufferSize (0),
    m_OutBuffer (),
    m_OutBufferPtr (0),
    m_OutBufferRetrieval (0),
    m_OutBufferSize (0)
{
#if ZLIB_FOUND
  m_ZStream = (z_stream*) (malloc (sizeof (z_stream)));
#endif
#if BZIP2_FOUND
  m_BZStream = (bz_stream*) (malloc (sizeof (bz_stream)));
#endif

  m_SearchPaths.push_back (g_PATH_BIN);
  
  //  Allocate the input buffer
  m_DictionaryBufferPtr = 0;
  m_DictionaryBufferSize = g_INIT_BUFFER_SIZE;
  m_DictionaryBuffer = (char*) calloc (m_DictionaryBufferSize,  sizeof (char));

  //  Allocate the input buffer
  m_InBufferPtr = 0;
  m_InBufferRetrieval = 0;
  m_InBufferSize = g_INIT_BUFFER_SIZE;
  m_InBuffer = (char*) calloc (m_InBufferSize,  sizeof (char));

  //  Allocate the output buffer
  m_OutBufferPtr = 0;
  m_OutBufferRetrieval = 0;
  m_OutBufferSize = g_INIT_BUFFER_SIZE;
  m_OutBuffer = (char*) calloc (m_OutBufferSize,  sizeof (char));
}



/*!
     Destructor that takes no arguments
*/
ExternalSoftware::~ExternalSoftware () {
#if ZLIB_FOUND
  free (m_ZStream);
#endif
#if BZIP2_FOUND
  free (m_BZStream);
#endif

  if (m_DictionaryBuffer != NULL) {
    free (m_DictionaryBuffer);
  }
  m_DictionaryBuffer = NULL;

  if (m_InBuffer != NULL) {
    free (m_InBuffer);
  }
  m_InBuffer = NULL;
  
  if (m_OutBuffer != NULL) {
    free (m_OutBuffer);
  }
  m_OutBuffer = NULL;
}



/*!
     Overloaded << operator defined as a friend of ExternalSoftware.

     \param[in] os Output stream
     \param[in] es ExternalSoftware passed as reference
     \return Output stream
*/
ostream &operator<< (ostream &os, const ExternalSoftware& es) {
  os << left << setw (g_VERBOSE_WIDTH) << "II\tExternal libraries:" << endl;

#if ZLIB_FOUND
  os << left << setw (g_VERBOSE_WIDTH) << "II\t  zlib:" << "Available" << endl;
#else
  os << left << setw (g_VERBOSE_WIDTH) << "II\t  zlib:" << "Unavailable" << endl;
#endif

#if BZIP2_FOUND
  os << left << setw (g_VERBOSE_WIDTH) << "II\t  bzlib2:" << "Available" << endl;
#else
  os << left << setw (g_VERBOSE_WIDTH) << "II\t  bzlib2:" << "Unavailable" << endl;
#endif

  os << left << setw (g_VERBOSE_WIDTH) << "II\tExternal software:" << endl;  
  os << left << setw (g_VERBOSE_WIDTH) << "II\t  gzip:" << es.GetGzipCommandPath () << endl;
  os << left << setw (g_VERBOSE_WIDTH) << "II\t  gunzip:" << es.GetGunzipCommandPath () << endl;
  os << left << setw (g_VERBOSE_WIDTH) << "II\t  bzip2:" << es.GetBzipCommandPath () << endl;
  os << left << setw (g_VERBOSE_WIDTH) << "II\t  bunzip2:" << es.GetBunzipCommandPath () << endl;
  os << left << setw (g_VERBOSE_WIDTH) << "II\t  Re-Pair:" << es.GetRepairCommandPath () << endl;
  os << left << setw (g_VERBOSE_WIDTH) << "II\t  Des-Pair:" << es.GetDespairCommandPath () << endl;
  os << left << setw (g_VERBOSE_WIDTH) << "II\t  shuff:" << es.GetShuffCommandPath () << endl;
  os << left << setw (g_VERBOSE_WIDTH) << "II\tSearch paths:" << es.ShowSearchPaths ();

  return os;
}


