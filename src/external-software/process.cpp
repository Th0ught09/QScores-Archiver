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
    \file process.cpp
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

using namespace std;

#include "external-software-local.hpp"
#include "external-software-exception.hpp"
#include "external-software.hpp"


//  -----------------------------------------------------------------
//  Public functions
//  -----------------------------------------------------------------


/*!
     Process a buffer of input.  Add it to m_InBuffer, enlarging it if necessary.  If this is
     the last buffer, then compress it using zlib/gzip or bzlib/bzip2.

     \param[in] buffer The buffer of data to compress
     \param[in] buffer_size Size of the buffer to process
     \param[in] last Indicate if this is the last buffer
     \throw External_Software_Exception
*/
void ExternalSoftware::Process (char* buffer, unsigned int buffer_size, bool last) {
  //  Copy the data from the temporary buffer to m_InBuffer
  if (buffer_size != 0) {
    if (buffer_size > (m_InBufferSize - m_InBufferPtr)) {
      while (buffer_size > (m_InBufferSize - m_InBufferPtr)) {
        m_InBufferSize = m_InBufferSize + g_INIT_BUFFER_SIZE;
        if (m_InBufferSize == UINT_MAX) {
          cerr << "EE\tInBuffer size exhausted while executing ExternalSoftware::Process ()!" << endl;
          exit (EXIT_FAILURE);
        }
      }
      m_InBuffer = (char*) realloc (m_InBuffer, sizeof (char) * m_InBufferSize);
    }
    memcpy (&m_InBuffer[m_InBufferPtr], buffer, buffer_size);
    m_InBufferPtr += buffer_size;
  }
  
  //  If this is not the last block, return
  if (!last) {
    return;
  }

  //  If this is the last block, then compress the buffer
  try {
    switch (m_Method) {
      case e_EXTERNAL_METHOD_GZIP_ZLIB :
        if (g_USE_ZLIB) {
          ProcessZlib ();
        }
        else {
          ProcessGzip ();
        }
        break;
      case e_EXTERNAL_METHOD_BZIP_BZLIB :
        if (g_USE_BZLIB) {
          ProcessBZlib ();
        }
        else {
          ProcessBzip ();
        }
        break;
      default :
        cerr << "EE\tMethod not yet implemented!" << endl;
        exit (EXIT_FAILURE);
        break;
    }
  }
  catch (exception &External_Software_Exception) {
    cerr << "EE\tError in executing external compression." << endl;
    exit (EXIT_FAILURE);
  }
  
  return;
}


/*!
     Process a buffer of input using either gzip or the zlib library.  Arguments simply passed to the
     corresponding function.

     \param[in] buffer The buffer of data to compress
     \param[in] buffer_size Size of the buffer to process
     \param[in] last Indicate if this is the last buffer
     \throw External_Software_Exception
*/
void ExternalSoftware::UnProcess (char* buffer, unsigned int buffer_size, bool last) {
  //  Copy the data from the temporary buffer to m_InBuffer
  if (buffer_size != 0) {
    if (buffer_size > (m_InBufferSize - m_InBufferPtr)) {
      while (buffer_size > (m_InBufferSize - m_InBufferPtr)) {
        m_InBufferSize = m_InBufferSize + g_INIT_BUFFER_SIZE;
        if (m_InBufferSize == UINT_MAX) {
          cerr << "EE\tInBuffer size exhausted while executing ExternalSoftware::UnProcess ()!" << endl;
          exit (EXIT_FAILURE);
        }
      }
      m_InBuffer = (char*) realloc (m_InBuffer, sizeof (char) * m_InBufferSize);
    }
    memcpy (&m_InBuffer[m_InBufferPtr], buffer, buffer_size);
    m_InBufferPtr += buffer_size;
  }

  //  If this is not the last block, return
  if (!last) {
    return;
  }

  //  If this is the last block, then uncompress the buffer
  try {
    switch (m_Method) {
      case e_EXTERNAL_METHOD_GZIP_ZLIB :
        if (g_USE_ZLIB) {
          UnProcessZlib ();
        }
        else {
          UnProcessGzip ();
        }
        break;
      case e_EXTERNAL_METHOD_BZIP_BZLIB :
        if (g_USE_BZLIB) {
          UnProcessBZlib ();
        }
        else {
          UnProcessBzip ();
        }
        break;
      default :
        cerr << "EE\tMethod not yet implemented!" << endl;
        exit (EXIT_FAILURE);
        break;
    }
  }
  catch (exception &External_Software_Exception) {
    cerr << "EE\tError in executing external compression." << endl;
    exit (EXIT_FAILURE);
  }
  
  return;
}


/*!
     Return the number of valid bytes in m_InBuffer (i.e., m_InBufferPtr).  The bytes between m_InBufferPtr
     and m_InBufferSize are invalid bytes and not counted.
     
     \return Number of valid bytes in m_InBuffer
*/
unsigned int ExternalSoftware::GetInBufferLength () const {
  return m_InBufferPtr;
}


/*!
     Return the number of valid bytes in m_OutBuffer (i.e., m_OutBufferPtr).  The bytes between m_OutBufferPtr
     and m_OutBufferSize are invalid bytes and not counted.
     
     \return Number of valid bytes in m_OutBuffer
*/
unsigned int ExternalSoftware::GetOutBufferLength () const {
  return m_OutBufferPtr;
}


