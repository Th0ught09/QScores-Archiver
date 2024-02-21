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
    \file gzip-zlib.cpp
    Main processing functions for gzip/zlib.
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


#if ZLIB_FOUND
/*!
     Process the buffer of input using the zlib library.  In the end, the compressed data is in 
     m_OutBuffer, occupying m_OutBufferPtr bytes.

     \throw External_Software_Exception
*/
void ExternalSoftware::ProcessZlib () {
  unsigned int compressed_size = 0;

  //  Use the standard malloc/free routines
  m_ZStream -> zalloc = Z_NULL;
  m_ZStream -> zfree = Z_NULL;
  m_ZStream -> opaque = Z_NULL;
  
  int return_value = deflateInit (m_ZStream, Z_BEST_COMPRESSION);
  if (return_value != Z_OK) {
    cerr << "EE\tError in initializing zlib for compression." << endl;
    exit (EXIT_FAILURE);
  }

  return_value = 0;
  
  //  Create temporary buffers that are of type unsigned char*, as
  //  required by the library
  unsigned char* in_buffer = (unsigned char*) calloc (m_InBufferSize, sizeof (unsigned char));
  unsigned char* out_buffer = (unsigned char*) calloc (m_OutBufferSize, sizeof (unsigned char));

  //  Copy the contents of m_InBuffer to in_buffer
  memcpy (in_buffer, m_InBuffer, m_InBufferPtr);
  
  m_ZStream -> avail_in = m_InBufferPtr;
  m_ZStream -> next_in = in_buffer;

  do {
    m_ZStream -> avail_out = (m_OutBufferSize - m_OutBufferPtr);
    m_ZStream -> next_out = &out_buffer[m_OutBufferPtr];
    
    return_value = deflate (m_ZStream, Z_FINISH);
    assert (return_value != Z_STREAM_ERROR);
    compressed_size = (m_OutBufferSize - m_OutBufferPtr) - m_ZStream -> avail_out;

    if (m_ZStream -> avail_out == 0) {
      m_OutBufferSize = m_OutBufferSize + g_INIT_BUFFER_SIZE;
      if (m_OutBufferSize == UINT_MAX) {
        cerr << "EE\tOutBuffer size exhausted while executing ExternalSoftware::ProcessZlib ()!" << endl;
        exit (EXIT_FAILURE);
      }
      out_buffer = (unsigned char*) realloc (out_buffer, sizeof (unsigned char) * m_OutBufferSize);
    }
    m_OutBufferPtr += compressed_size;
  } while (m_ZStream -> avail_out == 0);
  
  (void) deflateEnd (m_ZStream);

  //  Copy the contents of out_buffer to m_OutBuffer to 
  //  in_buffer, after allocating the necessary space
  m_OutBuffer = (char*) realloc (m_OutBuffer, sizeof (char) * m_OutBufferSize);
  memcpy (m_OutBuffer, out_buffer, m_OutBufferPtr);

  free (in_buffer);
  free (out_buffer);
  
  return;
}
  

/*!
     Unprocess a buffer of input using the zlib library.  In the end, the decompressed data is in 
     m_OutBuffer, occupying m_OutBufferPtr bytes.

     \throw External_Software_Exception
*/
void ExternalSoftware::UnProcessZlib () {
  unsigned int decompressed_size = 0;
  
  //  Use the standard malloc/free routines
  m_ZStream -> zalloc = Z_NULL;
  m_ZStream -> zfree = Z_NULL;
  m_ZStream -> opaque = Z_NULL;

  int result = inflateInit (m_ZStream);
  if (result != Z_OK) {
    cerr << "EE\tError in initializing zlib for compression." << endl;
    exit (EXIT_FAILURE);
  }

  int return_value = 0;
  
  //  Create temporary buffers that are of type char*, as
  //  required by the library
  unsigned char* in_buffer = (unsigned char*) calloc (m_InBufferSize, sizeof (char));
  unsigned char* out_buffer = (unsigned char*) calloc (m_OutBufferSize, sizeof (char));
  
  //  Copy the contents of m_InBuffer to in_buffer
  memcpy (in_buffer, m_InBuffer, m_InBufferPtr);
  
  m_ZStream -> avail_in = m_InBufferPtr;
  m_ZStream -> next_in = in_buffer;
  
  do {
    m_ZStream -> avail_out = (m_OutBufferSize - m_OutBufferPtr);
    m_ZStream -> next_out = &out_buffer[m_OutBufferPtr];
    
    return_value = inflate (m_ZStream, Z_NO_FLUSH);
    switch (return_value) {
      case Z_NEED_DICT:
        cerr << "EE\tZlib decompressor error -- Z_NEED_DICT." << endl;
        exit (EXIT_FAILURE);
      case Z_DATA_ERROR:
        cerr << "EE\tZlib decompressor error -- Z_DATA_ERROR." << endl;
        exit (EXIT_FAILURE);
      case Z_MEM_ERROR:
        cerr << "EE\tZlib decompressor error -- Z_MEM_ERROR." << endl;
        exit (EXIT_FAILURE);
    }
    decompressed_size = (m_OutBufferSize - m_OutBufferPtr) - m_ZStream -> avail_out;

    if (m_ZStream -> avail_out == 0) {
      m_OutBufferSize = m_OutBufferSize + g_INIT_BUFFER_SIZE;
      if (m_OutBufferSize == UINT_MAX) {
        cerr << "EE\tOutBuffer size exhausted while executing ExternalSoftware::UnProcessZlib ()!" << endl;
        exit (EXIT_FAILURE);
      }
      out_buffer = (unsigned char*) realloc (out_buffer, sizeof (unsigned char) * m_OutBufferSize);
    }
    m_OutBufferPtr += decompressed_size;
  } while (m_ZStream -> avail_out == 0);
  
  (void) inflateEnd (m_ZStream);

  //  Copy the contents of out_buffer to m_OutBuffer to 
  //  in_buffer, after allocating the necessary space
  m_OutBuffer = (char*) realloc (m_OutBuffer, sizeof (char) * m_OutBufferSize);
  memcpy (m_OutBuffer, out_buffer, m_OutBufferPtr);

  free (in_buffer);
  free (out_buffer);

  return;
}
#endif


/*!
     Process a buffer of input using an external call to gzip.  In the end, the compressed data is in 
     m_OutBuffer, occupying m_OutBufferPtr bytes.

     \throw External_Software_Exception
*/
void ExternalSoftware::ProcessGzip () {
  unsigned int return_value = 0;

  if (!GetInitializePaths ()) {
    InitializePaths ();
  }
  
  if (!GetGzipCommand ()) {
    throw External_Software_Exception ();
  }
  
  FILE* fp = fopen (g_TEMP_IN_FILENAME.c_str (), "wb");
  if (!fp) {
    cerr << "EE\tError in initializing gzip for compression." << endl;
    exit (EXIT_FAILURE);
  }
  
  return_value = fwrite (m_InBuffer, sizeof (unsigned char), m_InBufferPtr, fp);
  if (return_value != m_InBufferPtr) {
    cerr << "EE\tProblem writing to temporary file " << g_TEMP_IN_FILENAME << " while executing gzip!" << endl;
    exit (EXIT_FAILURE);
  }
  fclose (fp);

  string cmd = GetGzipCommandPath () + " -9 --stdout " + g_TEMP_IN_FILENAME + " >" + g_TEMP_OUT_FILENAME;

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
  fp_in.read (&(m_OutBuffer[0]), sizeof (char) * m_OutBufferSize);
  fp_in.close ();
  m_OutBufferPtr = m_OutBufferSize;

  //  Remove temporary files
  bfs::path input_path (g_TEMP_IN_FILENAME);
  remove (input_path);
  remove (output_path);
  
  return;
}


/*!
     Unprocess a buffer of input using an external call to gzip.  In the end, the decompressed data is in 
     m_OutBuffer, occupying m_OutBufferPtr bytes.

     \throw External_Software_Exception
*/
void ExternalSoftware::UnProcessGzip () {
  unsigned int return_value = 0;

  if (!GetInitializePaths ()) {
    InitializePaths ();
  }
  
  if (!GetGunzipCommand ()) {
    throw External_Software_Exception ();
  }
  
  FILE* fp = fopen (g_TEMP_IN_FILENAME.c_str (), "wb");
  if (!fp) {
    cerr << "EE\tError in initializing gzip for decompression." << endl;
    exit (EXIT_FAILURE);
  }
  
  return_value = fwrite (m_InBuffer, sizeof (unsigned char), m_InBufferPtr, fp);
  if (return_value != m_InBufferPtr) {
    cerr << "EE\tProblem writing to temporary file " << g_TEMP_IN_FILENAME << " while executing gzip!" << endl;
    exit (EXIT_FAILURE);
  }
  fclose (fp);

  string cmd = GetGunzipCommandPath () + " --stdout " + g_TEMP_IN_FILENAME + " >" + g_TEMP_OUT_FILENAME;

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
  fp_in.read (&(m_OutBuffer[0]), sizeof (char) * m_OutBufferSize);
  fp_in.close ();
  m_OutBufferPtr = m_OutBufferSize;
  
  //  Remove temporary files
  bfs::path input_path (g_TEMP_IN_FILENAME);
  remove (input_path);
  remove (output_path);
  
  return;
}

