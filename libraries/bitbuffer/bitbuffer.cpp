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
    \file bitbuffer.cpp
    Member functions for BitBuffer class.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
    $Id: bitbuffer.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>  //  exit
#include <cassert>  //  assert

using namespace std;

#include "systemcfg.hpp"
#include "bitbuffer.hpp"


//  -----------------------------------------------------------------
//  Constructors and destructors
//  -----------------------------------------------------------------


/*!
     Default constructor
*/
BitBuffer::BitBuffer ()
  : m_Debug (false),
    m_Filename (),
    m_In_Fp (),
    m_Out_Fp (),
    m_Mode (e_MODE_UNSET),
    m_Flushed (false),
    m_Closed (false),
    m_Mini_Buffer (0),
    m_Mini_Buffer_Used (0),
    m_Main_Buffer (),
    m_Main_Buffer_Ptr (0),
    m_Main_Buffer_End (0)
{
  //  Allocate space for the buffer
  m_Main_Buffer = new char[g_BITBUFFER_SIZE];

  int i = 0;
  for (i = 0; i < g_BITBUFFER_SIZE; i++) {
    m_Main_Buffer[i] = 0;
  }
}


/*!
     Destructor that checks if any data needs to be written out or any
     data still left to be read.  Then it frees memory and destroys the object.
*/
BitBuffer::~BitBuffer () {
  Finish ();

  delete [] m_Main_Buffer;
}


/*!
     Initialization function

     \param[in] fn Filename to read from/write to
     \param[in] mode Indicate whether the object is in read, write, or append mode
     \param[in] debug Whether or not debugging is turned on
*/
void BitBuffer::Initialize (string fn, e_READWRITE_MODE mode, bool debug) {
  m_Debug = debug;
  m_Filename = fn;
  m_Mode = mode;

  //  Check if filename given
  if (GetFilename ().length () == 0) {
    cerr << "EE\tNo filename provided to BitBuffer initializer." << endl;
    exit (EXIT_FAILURE);
  }
  
  //  Open the file for reading or writing
  if (GetMode () == e_MODE_READ) {
    m_In_Fp.open (GetFilename ().c_str (), ios::in|ios::binary);
    if (!m_In_Fp) {
      cerr << "EE\tCannot open " << GetFilename () << " for reading." << endl;
      exit (EXIT_FAILURE);
    }
  }
  else if (GetMode () == e_MODE_WRITE) {
    m_Out_Fp.open (GetFilename ().c_str (), ios::out|ios::binary);
    if (!m_Out_Fp) {
      cerr << "EE\tCannot open " << GetFilename () << " for writing." << endl;
      exit (EXIT_FAILURE);
    }
  }
  else if (GetMode () == e_MODE_APPEND) {
    m_Out_Fp.open (GetFilename ().c_str (), ios::app|ios::binary);
    if (!m_Out_Fp) {
      cerr << "EE\tCannot open " << GetFilename () << " for appending." << endl;
      exit (EXIT_FAILURE);
    }
  }
  else {
    cerr << "==\tError:  Invalid read/write mode for BitBuffer class constructor." << endl;
    exit (EXIT_FAILURE);
  }

  return;
}


//  -----------------------------------------------------------------
//  Accessors and mutators
//  -----------------------------------------------------------------

/*!
     Return m_Filename

     \return The file that we're reading from or writing to
*/
string BitBuffer::GetFilename () const {
  return m_Filename;
}


/*!
     Return m_Mode

     \return The read/write mode  of the object
*/
enum e_READWRITE_MODE BitBuffer::GetMode () const {
  return m_Mode;
}



/*!
     Return m_Debug

     \return Debug mode or not
*/
bool BitBuffer::GetDebug () const {
  return m_Debug;
}


/*!
     Set m_Debug
     
     \param[in] value The value to set
*/
void BitBuffer::SetDebug (bool value) {
  m_Debug = value;
}


/*!
     Set m_Flushed

     \param[in] value The value to set
*/
void BitBuffer::SetFlushed (bool value) {
  m_Flushed = value;
}


/*!
     Set m_Closed

     \param[in] value The value to set
*/
void BitBuffer::SetClosed (bool value) {
  m_Closed = value;
}


