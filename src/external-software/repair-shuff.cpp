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
    \file repair-shuff.cpp
    Main processing functions for Re-Pair, Des-Pair, and Shuff.

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: repair-shuff.cpp 224 2011-11-06 15:32:09Z rwan $

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


/*!
     Process the buffer of input using Re-Pair, Des-Pair, and Shuff.  In the end, the compressed data is in 
     m_OutBuffer, occupying m_OutBufferPtr bytes.

     \throw External_Software_Exception
*/
void ExternalSoftware::ProcessRePair () {
  unsigned int return_value = 0;
  string orig_fn = g_TEMP_IN_FILENAME;
  string seq_fn = g_TEMP_IN_FILENAME + ".seq";
  string prel_fn = g_TEMP_IN_FILENAME + ".prel";
  string shuff_fn = g_TEMP_IN_FILENAME + ".shuff";

  if (!GetInitializePaths ()) {
    InitializePaths ();
  }
  
  if ((!GetRepairCommand ()) || (!GetShuffCommand ())) {
    throw External_Software_Exception ();
  }
  
  FILE* fp = fopen (orig_fn.c_str (), "wb");
  if (!fp) {
    cerr << "EE\tError in initializing Re-Pair for compression." << endl;
    exit (EXIT_FAILURE);
  }
  
  return_value = fwrite (m_InBuffer, sizeof (unsigned char), m_InBufferPtr, fp);
  if (return_value != m_InBufferPtr) {
    cerr << "EE\tProblem writing to temporary file " << orig_fn << " while executing Re-Pair!" << endl;
    exit (EXIT_FAILURE);
  }
  fclose (fp);

  //  Run Re-Pair
  string cmd = GetRepairCommandPath () + " -i " + orig_fn;

  return_value = system (cmd.c_str ());
  if (return_value != g_COMMAND_SUCCESS) {
    throw External_Software_Exception ();
  }

  //  Run Shuff
  cmd = GetShuffCommandPath () + " -e -Z " + seq_fn + " >" + shuff_fn;
  
  return_value = system (cmd.c_str ());
  if (return_value != g_COMMAND_SUCCESS) {
    throw External_Software_Exception ();
  }
  
  //  Determine the size of the two binary representations
  bfs::path dictionary_path (prel_fn);
  m_DictionaryBufferSize = file_size (dictionary_path);
  bfs::path output_path (shuff_fn);
  m_OutBufferSize = file_size (output_path);

  //  Read in the two binary representations
  m_DictionaryBuffer = (char*) realloc (m_DictionaryBuffer, sizeof (char) * m_DictionaryBufferSize);
  ifstream fp_in (prel_fn.c_str (), ios::in|ios::binary);
  fp_in.read (&(m_DictionaryBuffer[0]), sizeof (char) * m_DictionaryBufferSize);
  fp_in.close ();
  m_DictionaryBufferPtr = m_DictionaryBufferSize;
  
  m_OutBuffer = (char*) realloc (m_OutBuffer, sizeof (char) * m_OutBufferSize);
  fp_in.open (shuff_fn.c_str (), ios::in|ios::binary);
  fp_in.read (&(m_OutBuffer[0]), sizeof (char) * m_OutBufferSize);
  fp_in.close ();
  m_OutBufferPtr = m_OutBufferSize;

  //  Remove temporary files
  bfs::path input_path (orig_fn);
  bfs::path seq_path (seq_fn);
  remove (input_path);
  remove (seq_path);
  remove (dictionary_path);
  remove (output_path);
  
  return;
}
  

/*!
     Unprocess a buffer of input using Re-Pair, Des-Pair, and Shuff.  In the end, the compressed data is in 
     m_OutBuffer, occupying m_OutBufferPtr bytes.

     \throw External_Software_Exception
*/
void ExternalSoftware::UnProcessRePair () {
  unsigned int return_value = 0;
  string orig_fn = g_TEMP_IN_FILENAME;
  string seq_fn = g_TEMP_IN_FILENAME + ".seq";
  string prel_fn = g_TEMP_IN_FILENAME + ".prel";
  string shuff_fn = g_TEMP_IN_FILENAME + ".shuff";
  string uncompressed_fn = g_TEMP_IN_FILENAME + ".u";

  if (!GetInitializePaths ()) {
    InitializePaths ();
  }
  
  if ((!GetDespairCommand ()) || (!GetShuffCommand ())) {
    throw External_Software_Exception ();
  }
  
  //  Write out the dictionary
  FILE* fp = fopen (prel_fn.c_str (), "wb");
  if (!fp) {
    cerr << "EE\tError in initializing Des-Pair for decompression." << endl;
    exit (EXIT_FAILURE);
  }
  
  return_value = fwrite (m_DictionaryBuffer, sizeof (unsigned char), m_DictionaryBufferPtr, fp);
  if (return_value != m_DictionaryBufferPtr) {
    cerr << "EE\tProblem writing to temporary file " << prel_fn << " while executing Des-Pair!" << endl;
    exit (EXIT_FAILURE);
  }
  fclose (fp);

  //  Write out the sequence
  fp = fopen (shuff_fn.c_str (), "wb");
  if (!fp) {
    cerr << "EE\tError in initializing Shuff for decompression." << endl;
    exit (EXIT_FAILURE);
  }
  
  return_value = fwrite (m_OutBuffer, sizeof (unsigned char), m_OutBufferPtr, fp);
  if (return_value != m_OutBufferPtr) {
    cerr << "EE\tProblem writing to temporary file " << shuff_fn << " while executing Shuff!" << endl;
    exit (EXIT_FAILURE);
  }
  fclose (fp);

  //  Execute shuff
  string cmd = GetShuffCommandPath () + " -d " + shuff_fn + " >" + seq_fn;

  return_value = system (cmd.c_str ());
  if (return_value != g_COMMAND_SUCCESS) {
    throw External_Software_Exception ();
  }

  //  Execute Des-Pair
  cmd = GetDespairCommandPath () + " -i " + orig_fn;

  return_value = system (cmd.c_str ());
  if (return_value != g_COMMAND_SUCCESS) {
    throw External_Software_Exception ();
  }

  //  Determine the size of the binary representation
  bfs::path uncompressed_path (uncompressed_fn);
  m_OutBufferSize = file_size (uncompressed_path);

  //  Read in binary representation
  m_OutBuffer = (char*) realloc (m_OutBuffer, sizeof (char) * m_OutBufferSize);
  ifstream fp_in (uncompressed_fn.c_str (), ios::in|ios::binary);
  fp_in.read (&(m_OutBuffer[0]), sizeof (char) * m_OutBufferSize);
  fp_in.close ();
  m_OutBufferPtr = m_OutBufferSize;
  
  //  Remove temporary files
  bfs::path prel_path (prel_fn);
  bfs::path shuff_path (shuff_fn);
  bfs::path seq_path (seq_fn);
  remove (prel_path);
  remove (shuff_path);
  remove (seq_path);
  remove (uncompressed_path);
  
  return;
}

