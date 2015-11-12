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
    \file gzip-zlib.cpp
    Main processing functions for gzip/zlib.

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: retrieve.cpp 224 2011-11-06 15:32:09Z rwan $

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


/*!
     Retrieve the next block of the buffer (up to buffer_size bytes).  If there is less than buffer_size bytes 
     available, then return what is left.  The return value is the number of bytes returned.
     
     The buffer needs to be allocated outside of this function.
     
     \param[in] buffer The temporary buffer to store the data, as an unsigned char*
     \param[in] buffer_size Size of the temporary buffer
     \param[in] last Indicate if this is the last buffer using call-by-reference
     \return The size of the buffer returned     
*/
unsigned int ExternalSoftware::RetrieveUCharBlock (unsigned char* buffer, unsigned int buffer_size, bool& last) {
  unsigned int retrieval_size = buffer_size;
  last = false;
  
  if ((m_OutBufferPtr - m_OutBufferRetrieval) < buffer_size) {
    retrieval_size = m_OutBufferPtr - m_OutBufferRetrieval;
    last = true;
  }
  
  memcpy (buffer, &m_OutBuffer[m_OutBufferRetrieval], retrieval_size);
  m_OutBufferRetrieval += retrieval_size;
  
  return (retrieval_size);
}


/*!
     Retrieve the next block of the buffer (up to buffer_size bytes).  If there is less than buffer_size bytes 
     available, then return what is left.  The return value is the number of bytes returned.
     
     The buffer needs to be allocated outside of this function.
     
     \param[in] buffer The temporary buffer to store the data, as a char*
     \param[in] buffer_size Size of the temporary buffer
     \param[in] last Indicate if this is the last buffer using call-by-reference
     \return The size of the buffer returned     
*/
unsigned int ExternalSoftware::RetrieveCharBlock (char* buffer, unsigned int buffer_size, bool& last) {
  unsigned int retrieval_size = buffer_size;
  last = false;
  
  if ((m_OutBufferPtr - m_OutBufferRetrieval) < buffer_size) {
    retrieval_size = m_OutBufferPtr - m_OutBufferRetrieval;
    last = true;
  }
  
  memcpy (buffer, &m_OutBuffer[m_OutBufferRetrieval], retrieval_size);
  m_OutBufferRetrieval += retrieval_size;
  
  return (retrieval_size);
}


/*!
     Retrieve the entire buffer as an unsigned char*.  It is the calling function's responsibility to 
     free the buffer's memory.
     
     Note that since it is making a copy of the buffer, we are now using twice the amount of memory.
     Use this function with care!
     
     \return The buffer as an unsigned char*
*/
unsigned char* ExternalSoftware::RetrieveUChar () {
  unsigned char* result = (unsigned char*) calloc (GetOutBufferLength () + 1, sizeof (unsigned char));  
  
  memcpy (result, m_OutBuffer, GetOutBufferLength ());
  
  return (result);
}


/*!
     Retrieve the entire buffer as a char*.  It is the calling function's responsibility to 
     free the buffer's memory.
     
     Note that since it is making a copy of the buffer, we are now using twice the amount of memory.
     Use this function with care!
     
     \return The buffer as a char*
*/
char* ExternalSoftware::RetrieveChar () {
  char* result = (char*) calloc (GetOutBufferLength () + 1, sizeof (char));  
  
  result = (char*) memcpy (result, m_OutBuffer, GetOutBufferLength ());
  
  return (result);
}

