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
    \file finish.cpp
    Member functions for BitBuffer class related to finishing reading/writing.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
    $Id: finish.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <fstream>
#include <cstdlib>  //  exit
#include <iostream>

using namespace std;

#include "systemcfg.hpp"
#include "bitbuffer.hpp"

//  -----------------------------------------------------------------
//  Private functions
//  -----------------------------------------------------------------


/*!
     Return m_Flushed

     \return Indicator of whether or not file pointer has been flushed.
*/
bool BitBuffer::IsFlushed () {
  return (m_Flushed);
}


/*!
     Flush the pointer for reading (i.e., basically nothing to do)

     \return Always returns true
*/
bool BitBuffer::FlushRead () {
  if (IsFlushed ()) {
    return true;
  }

  SetFlushed (true);

  return true;
}


/*!
     Flush the pointer for writing

     \return Always returns true
*/
bool BitBuffer::FlushWrite () {
  unsigned int unused_bits = g_UINT_SIZE_BITS - m_Mini_Buffer_Used;

  if (IsFlushed ()) {
    return true;
  }
  
  m_Mini_Buffer <<= unused_bits;      //  Pad remaining space with 0's
  unused_bits = 0;

  //  Copy the mini-buffer to the main-buffer
  while (unused_bits < g_UINT_SIZE_BITS) {
    m_Main_Buffer[m_Main_Buffer_Ptr] = static_cast<char>((m_Mini_Buffer >> (g_UINT_SIZE_BITS - g_CHAR_SIZE_BITS)) & g_MASK_LOWER_BYTE);
    m_Main_Buffer_Ptr++;
    m_Mini_Buffer = m_Mini_Buffer << g_CHAR_SIZE_BITS;
    unused_bits += g_CHAR_SIZE_BITS;
  }

  //  Write the main-buffer to disk
  if (m_Main_Buffer_Ptr > 0) {
    m_Out_Fp.write ((char*) m_Main_Buffer, m_Main_Buffer_Ptr);
    if (m_Out_Fp.fail ()) {
      cerr << "==\tError while writing to output file in FinishWrite ()." << endl;
      exit (EXIT_FAILURE);
    }
  }
  m_Main_Buffer_Ptr = 0;
  m_Mini_Buffer_Used = 0;
  m_Mini_Buffer = 0;

  SetFlushed (true);

  return true;
}


/*!
     Return m_Closed

     \return Indicator of whether or not file pointer has been closed.
*/
bool BitBuffer::IsClosed () {
  return (m_Closed);
}


/*!
     Close the pointer for reading

     \return Always returns true
*/
bool BitBuffer::CloseRead () {
  m_In_Fp.close ();

  SetClosed (true);

  return true;
}


/*!
     Close the pointer for writing

     \return Always returns true
*/
bool BitBuffer::CloseWrite () {
  m_Out_Fp.close ();

  SetClosed (true);

  return true;
}


//  -----------------------------------------------------------------
//  Public functions
//  -----------------------------------------------------------------


/*!
     Flush the file pointer
*/
void BitBuffer::Flush () {
  if (!IsFlushed ()) {
    bool result = false;
    if (GetMode () == e_MODE_READ) {
      result = FlushRead ();
    }
    else if (GetMode () == e_MODE_WRITE) {
      result = FlushWrite ();
    }
    else if (GetMode () == e_MODE_APPEND) {
      result = FlushWrite ();
    }
  }

  SetFlushed (true);

  return;
}


/*!
     Close the file pointer; flush the buffer if it hasn't yet been flushed.
*/
void BitBuffer::Close () {
  if (!IsFlushed ()) {
    Flush ();
  }
  
  if (!IsClosed ()) {
    bool result = false;
    if (GetMode () == e_MODE_READ) {
      result = CloseRead ();
    }
    else if (GetMode () == e_MODE_WRITE) {
      result = CloseWrite ();
    }
    else if (GetMode () == e_MODE_APPEND) {
      result = CloseWrite ();
    }
  }

  return;
}


/*!
     Flush the file pointer and close it
*/
void BitBuffer::Finish () {
  Flush ();
  Close ();

  return;
}

