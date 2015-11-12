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
    \file bzip-bzlib.cpp
    Main processing functions for bzip/bzlib.

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: bzip-bzlib.cpp 224 2011-11-06 15:32:09Z rwan $

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
//  Private functions
//  -----------------------------------------------------------------

#if BZIP2_FOUND
/*!
     Process the buffer of input using the bzlib library.  In the end, the compressed data is in 
     m_OutBuffer, occupying m_OutBufferPtr bytes.

     \throw External_Software_Exception
*/
void ExternalSoftware::ProcessBZlib () {
  unsigned int compressed_size = 0;

  //  Use the standard malloc/free routines
  m_BZStream -> bzalloc = NULL;
  m_BZStream -> bzfree = NULL;
  m_BZStream -> opaque = NULL;

  //  Initialize the bzip2 library with maximum compression (9),
  //  no verbose output (0), and the default workFactor (0)
  int return_value = BZ2_bzCompressInit (m_BZStream, 9, 0, 0);
  if (return_value != BZ_OK) {
    cerr << "EE\tError in initializing bzlib for compression." << endl;
    exit (EXIT_FAILURE);
  }
  
  return_value = 0;

  m_BZStream -> avail_in = m_InBufferPtr;
  m_BZStream -> next_in = m_InBuffer;

  do {
    m_BZStream -> avail_out = (m_OutBufferSize - m_OutBufferPtr);
    m_BZStream -> next_out = &m_OutBuffer[m_OutBufferPtr];
    
    return_value = BZ2_bzCompress (m_BZStream, BZ_FINISH);
    compressed_size = (m_OutBufferSize - m_OutBufferPtr) - (m_BZStream -> avail_out);

    if (m_BZStream -> avail_out == 0) {
      m_OutBufferSize = m_OutBufferSize + g_INIT_BUFFER_SIZE;
      if (m_OutBufferSize == UINT_MAX) {
        cerr << "EE\tOutBuffer size exhausted while executing ExternalSoftware::ProcessBZlib ()!" << endl;
        exit (EXIT_FAILURE);
      }
      m_OutBuffer = (char*) realloc (m_OutBuffer, sizeof (char) * m_OutBufferSize);
    }
    m_OutBufferPtr += compressed_size;
  } while (m_BZStream -> avail_out == 0);
  
  if (BZ2_bzCompressEnd (m_BZStream) != BZ_OK) {
    cerr << "EE\tError while finishing up compression using the bzip2 library." << endl;
    exit (EXIT_FAILURE);
  }
  
  return;
}
  

/*!
     Unprocess a buffer of input using the zlib library.  In the end, the uncompressed data is in 
     m_Buffer, occupying m_BufferPtr bytes.

     \throw External_Software_Exception
*/
void ExternalSoftware::UnProcessBZlib () {
  unsigned int decompressed_size = 0;

  //  Use the standard malloc/free routines
  m_BZStream -> bzalloc = NULL;
  m_BZStream -> bzfree = NULL;
  m_BZStream -> opaque = NULL;

  //  Initialize the bzip2 library with no verbose output (0),
  //  and the default value for small which uses the default
  //  amount of memory (0)
  int return_value = BZ2_bzDecompressInit (m_BZStream, 0, 0);
  if (return_value != BZ_OK) {
    cerr << "EE\tError in initializing bzlib for decompression." << endl;
    exit (EXIT_FAILURE);
  }
  
  return_value = 0;
  
  m_BZStream -> avail_in = m_InBufferPtr;
  m_BZStream -> next_in = m_InBuffer;

  do {
    m_BZStream -> avail_out = (m_OutBufferSize - m_OutBufferPtr);
    m_BZStream -> next_out = &m_OutBuffer[m_OutBufferPtr];

    return_value = BZ2_bzDecompress (m_BZStream);
    if ((return_value != BZ_OK) && (return_value != BZ_STREAM_END)) {
      cerr << "EE\tBZlib decompressor error:  " << return_value << endl;
      exit (EXIT_FAILURE);
    }
    decompressed_size = (m_OutBufferSize - m_OutBufferPtr) - (m_BZStream -> avail_out);

    if (m_BZStream -> avail_out == 0) {
      m_OutBufferSize = m_OutBufferSize + g_INIT_BUFFER_SIZE;
      if (m_OutBufferSize == UINT_MAX) {
        cerr << "EE\tOutBuffer size exhausted while executing ExternalSoftware::UnProcessBZlib ()!" << endl;
        exit (EXIT_FAILURE);
      }
      m_OutBuffer = (char*) realloc (m_OutBuffer, sizeof (char) * m_OutBufferSize);
    }
    m_OutBufferPtr += decompressed_size;
  } while (return_value != BZ_STREAM_END);
    
  if (BZ2_bzDecompressEnd (m_BZStream) != BZ_OK) {
    cerr << "EE\tError while finishing up decompression using the bzip2 library." << endl;
    exit (EXIT_FAILURE);
  }
  
  return;
}
#endif


/*!
     Process a buffer of input using an external call to bzip2.  In the end, the compressed data is in 
     m_OutBuffer, occupying m_OutBufferPtr bytes.

     \throw External_Software_Exception
*/
void ExternalSoftware::ProcessBzip () {
  unsigned int return_value = 0;
  
  if (!GetInitializePaths ()) {
    InitializePaths ();
  }
  
  if (!GetBzipCommand ()) {
    throw External_Software_Exception ();
  }
  
  FILE* fp = fopen (g_TEMP_IN_FILENAME.c_str (), "wb");
  if (!fp) {
    cerr << "EE\tError in initializing bzip2 for compression." << endl;
    exit (EXIT_FAILURE);
  }
  
  return_value = fwrite (m_InBuffer, sizeof (char), m_InBufferPtr, fp);
  if (return_value != m_InBufferPtr) {
    cerr << "EE\tProblem writing to temporary file " << g_TEMP_IN_FILENAME << " while executing bzip2!" << endl;
    exit (EXIT_FAILURE);
  }
  fclose (fp);
  
  string cmd = GetBzipCommandPath () + " -9 --stdout " + g_TEMP_IN_FILENAME + " >" + g_TEMP_OUT_FILENAME;

  return_value = system (cmd.c_str ());
  if (return_value != g_COMMAND_SUCCESS) {
    throw External_Software_Exception ();
  }
    
  //  Determine the size of the binary representation
  bfs::path output_path (g_TEMP_OUT_FILENAME);
  m_OutBufferSize = file_size (output_path);

  //  Read in binary representation
  m_OutBuffer = (char*) realloc (m_OutBuffer, sizeof (char) * m_OutBufferSize);
  ifstream fp_in (g_TEMP_OUT_FILENAME.c_str (), ios::in|ios::binary);
  fp_in.read ((char*)&(m_OutBuffer[0]), sizeof (char) * m_OutBufferSize);
  fp_in.close ();
  m_OutBufferPtr = m_OutBufferSize;
  
  //  Remove temporary files
  bfs::path input_path (g_TEMP_IN_FILENAME);
  remove (input_path);
  remove (output_path);
  
  return;
}


/*!
     Unprocess a buffer of input using an external call to bzip2.  In the end, the uncompressed data is in 
     m_OutBuffer, occupying m_OutBufferPtr bytes.

     \throw External_Software_Exception
*/
void ExternalSoftware::UnProcessBzip () {
  unsigned int return_value = 0;

  if (!GetInitializePaths ()) {
    InitializePaths ();
  }
  
  if (!GetBunzipCommand ()) {
    throw External_Software_Exception ();
  }
  
  FILE* fp = fopen (g_TEMP_IN_FILENAME.c_str (), "wb");
  if (!fp) {
    cerr << "EE\tError in initializing gzip for compression." << endl;
    exit (EXIT_FAILURE);
  }
  
  return_value = fwrite (m_InBuffer, sizeof (char), m_InBufferPtr, fp);
  if (return_value != m_InBufferPtr) {
    cerr << "EE\tProblem writing to temporary file " << g_TEMP_IN_FILENAME << " while executing gzip!" << endl;
    exit (EXIT_FAILURE);
  }
  fclose (fp);

  string cmd = GetBunzipCommandPath () + " --stdout " + g_TEMP_IN_FILENAME + " >" + g_TEMP_OUT_FILENAME;

  return_value = system (cmd.c_str ());
  if (return_value != g_COMMAND_SUCCESS) {
    throw External_Software_Exception ();
  }
    
  //  Determine the size of the binary representation
  bfs::path output_path (g_TEMP_OUT_FILENAME);
  m_OutBufferSize = file_size (output_path);

  //  Read in binary representation
  m_OutBuffer = (char*) realloc (m_OutBuffer, sizeof (char) * m_OutBufferSize);
  ifstream fp_in (g_TEMP_OUT_FILENAME.c_str (), ios::in|ios::binary);
  fp_in.read ((char*)&(m_OutBuffer[0]), sizeof (char) * m_OutBufferSize);
  fp_in.close ();
  m_OutBufferPtr = m_OutBufferSize;
  
  //  Remove temporary files
  bfs::path input_path (g_TEMP_IN_FILENAME);
  remove (input_path);
  remove (output_path);
  
  return;
}


