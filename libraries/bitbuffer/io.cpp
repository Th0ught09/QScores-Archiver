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
    \file io.cpp
    IO functions for BitBuffer class.

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: io.cpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>  //  exit
#include <cassert>  //  assert

using namespace std;

#include "systemcfg.hpp"
#include "bitbuffer_exception.hpp"
#include "bitbuffer.hpp"


//  -----------------------------------------------------------------
//  Private functions
//  -----------------------------------------------------------------


/*!
     A low-level read (private member function) that reads at *least* the given number of bits
     from the main-buffer

     \param[in] min_bits The minimum number of bits requested
     \return Boolean indicating success or failure
     \throw BitBuffer_Input_Exception
*/
bool BitBuffer::ReadBitsLowLevel (unsigned int min_bits) {
  int bytes_read = 0;

  //  Cannot read from a closed file handle
  assert (IsClosed () == false);

  //  Ensure m_Main_Buffer_Ptr is not pointing out of bounds
  assert (m_Main_Buffer_Ptr <= m_Main_Buffer_End);

  //  Check if the main-buffer is empty and if so, read from file
  if (m_Main_Buffer_Ptr == m_Main_Buffer_End) {
    m_In_Fp.read ((char*) m_Main_Buffer, g_BITBUFFER_SIZE);
    bytes_read = m_In_Fp.gcount ();

    //  Check if either the failbit or badbit flags are set
    if (m_In_Fp.bad ()) {
      cerr << "EE\tError:  Serious error in reading from input buffer after reading in " << bytes_read << " bytes." << endl;
      exit (EXIT_FAILURE);
    }
    if (m_In_Fp.fail ()) {
      if (bytes_read < g_BITBUFFER_SIZE) {
        //  Clear the fail bit since we only reached the end of the buffer,
        //  which is not a problem
        m_In_Fp.clear ();
      }
      else {
        cerr << "EE\tError:  Fail while reading from input buffer after reading in " << bytes_read << " bytes." << endl;
      }
    }

    //  If nothing read, then error
    if (bytes_read == 0) {
      throw BitBuffer_Input_Exception ();
    }

    m_Main_Buffer_Ptr = 0;
    m_Main_Buffer_End = bytes_read;
  }

  //  Copy the main-buffer to the mini-buffer
  unsigned int new_bits = 0;
  while (new_bits < g_UINT_SIZE_BITS) {
    m_Mini_Buffer = m_Mini_Buffer << g_CHAR_SIZE_BITS;
    m_Mini_Buffer |= static_cast<unsigned int>(m_Main_Buffer[m_Main_Buffer_Ptr]) & g_MASK_LOWER_BYTE;
    m_Main_Buffer_Ptr++;
    new_bits += g_CHAR_SIZE_BITS;
    if (m_Main_Buffer_Ptr > m_Main_Buffer_End) {
      m_Main_Buffer_End = m_Main_Buffer_Ptr;
      break;
    }
  }

  m_Mini_Buffer_Used = new_bits;
  if (m_Mini_Buffer_Used < min_bits) {
    throw BitBuffer_Input_Exception ();
  }

  return true;
}


//  -----------------------------------------------------------------
//  Public functions (bit-based)
//  -----------------------------------------------------------------


/*!
     Read a specified number of bits.

     \param[in] num_bits The number of bits requested
     \return The value of the bits as an unsigned integer
     \throw BitBuffer_Input_Exception
*/
unsigned int BitBuffer::ReadBits (unsigned int num_bits) {
  unsigned int x = 0;

  //  Cannot read from a closed file handle
  assert (IsClosed () == false);

  if (IsFlushed ()) {
    SetFlushed (false);
  }
  
  if (num_bits > g_UINT_SIZE_BITS) {
    cerr << "EE\tMore bits (" << num_bits << ") requested than what can be provided [BitBuffer::ReadBits ()]." << endl;
    exit (EXIT_FAILURE);
  }

  //  No bits requested
  if (num_bits == 0) {
    return (0);
  }

  //  Initialize x depending on whether or not there are any bits in the
  //  mini-buffer
  if (m_Mini_Buffer_Used == 0) {
    x = 0;
  }
  else {
    x = (m_Mini_Buffer) >> (g_UINT_SIZE_BITS - num_bits);
  }

  //  Check if the mini-buffer has enough bits to satisfy our request
  if (num_bits <= m_Mini_Buffer_Used) {
    m_Mini_Buffer = m_Mini_Buffer << num_bits;
    m_Mini_Buffer_Used -= num_bits;
  }
  else {
    num_bits -= m_Mini_Buffer_Used;
    bool retval = ReadBitsLowLevel (num_bits);
    if (retval == false) {
      throw BitBuffer_Input_Exception ();
    }
    x |= (m_Mini_Buffer) >> (g_UINT_SIZE_BITS - num_bits);
    m_Mini_Buffer = m_Mini_Buffer << num_bits;
    m_Mini_Buffer_Used -= num_bits;
  }

  if (GetDebug ()) {
    cerr << "\tBitbuffer::ReadBits\t" << x << "\t" << num_bits << endl;
  }

  return x;
}


/*!
     Write a value using the specified number of bits.

     \param[in] value Number to write
     \param[in] num_bits Number of bits to use
*/
void BitBuffer::WriteBits (unsigned int value, unsigned int num_bits) {
  unsigned int x = value;
  unsigned int unused_bits = g_UINT_SIZE_BITS - m_Mini_Buffer_Used;

  //  Cannot write to a closed file handle
  assert (IsClosed () == false);

  //  Number of bits to be written out should never be greater than this limit
  assert (num_bits <= g_UINT_SIZE_BITS);

  if (IsFlushed ()) {
    SetFlushed (false);
  }

  if (GetDebug ()) {
    cerr << "\tBitbuffer::WriteBits\t" << value << "\t" << num_bits << endl;
  }

  //  Check if there is space in the mini-buffer for the bits that we want to write
  if (num_bits <= unused_bits) {
    if (num_bits != g_UINT_SIZE_BITS) {
      //  Make space for the bits and add them in.  The following line works even if
      //  (num_bits == g_UINT_SIZE_BITS) since (1 << g_UINT_SIZE_BITS) == 0.
      m_Mini_Buffer = (m_Mini_Buffer << num_bits) | (x & ((1 << num_bits) - 1));
    }
    else {
      m_Mini_Buffer = x;
    }

    //  Update the number of bits in use
    m_Mini_Buffer_Used += num_bits;
  }
  else {
    //  If there is no space in the mini-buffer, make some space and add what we can,
    //  (which is the first "unused_bits" of x)
    m_Mini_Buffer = m_Mini_Buffer << unused_bits;
    m_Mini_Buffer = m_Mini_Buffer | ((x >> (num_bits - unused_bits)) & ((1 << (unused_bits)) - 1));

    //  Copy the mini-buffer to the main-buffer
    unsigned int processed_bits = 0;
    while (processed_bits < g_UINT_SIZE_BITS) {
      m_Main_Buffer[m_Main_Buffer_Ptr] = static_cast<char>((m_Mini_Buffer >> (g_UINT_SIZE_BITS - g_CHAR_SIZE_BITS)) & g_MASK_LOWER_BYTE);

      m_Main_Buffer_Ptr++;
      m_Mini_Buffer = m_Mini_Buffer << g_CHAR_SIZE_BITS;
      processed_bits += g_CHAR_SIZE_BITS;
    }

    //  Add the remaining bits to the mini-buffer, which is "num_bits - unused_bits"
    if (num_bits - unused_bits != g_UINT_SIZE_BITS) {
      m_Mini_Buffer = x & ((1 << (num_bits - unused_bits)) - 1);
    }
    else {
      m_Mini_Buffer = x;
    }
    m_Mini_Buffer_Used = num_bits - unused_bits;

    //  Check if the main-buffer needs to be written out
    if (m_Main_Buffer_Ptr >= g_BITBUFFER_SIZE) {            /*  Write bits out  */
      m_Out_Fp.write ((char*) m_Main_Buffer, m_Main_Buffer_Ptr);
      if (m_Out_Fp.bad ()) {
        cerr << "EE\tError while writing to output file." << endl;
        exit (EXIT_FAILURE);
      }
      m_Main_Buffer_Ptr = 0;
    }
  }

  return;
}


//  -----------------------------------------------------------------
//  Public functions (unsigned int-based)
//  -----------------------------------------------------------------


/*!
     Read a specified number of unsigned integers.  Also allocates the space needed.

     \param[in] buffer The buffer where the values will go
     \param[in] num_values The number of values to read
     \return true upon success; false otherwise.
*/
bool BitBuffer::ReadUInts (unsigned int *buffer, int num_values) {
  //  Cannot read from a closed file handle
  assert (IsClosed () == false);

  if (IsFlushed ()) {
    SetFlushed (false);
  }

  for (int i = 0; i < num_values; i++) {
    buffer[i] = ReadBits (g_UINT_SIZE_BITS);
  }

  return true;
}


/*!
     Write a specified number of unsigned integers.

     \param[in] buffer The buffer where the values are from
     \param[in] num_values The number of values to write
     \return true upon success; false otherwise.
*/
bool BitBuffer::WriteUInts (unsigned int *buffer, int num_values) {
  //  Cannot write to a closed file handle
  assert (IsClosed () == false);

  if (IsFlushed ()) {
    SetFlushed (false);
  }

  for (int i = 0; i < num_values; i++) {
    WriteBits (buffer[i], g_UINT_SIZE_BITS);
  }

  return true;
}


//  -----------------------------------------------------------------
//  Public functions (unsigned char)
//  -----------------------------------------------------------------


/*!
     Read a specified number of unsigned characters.  Also allocates the space needed.

     \param[in] buffer The buffer where the values will go
     \param[in] num_values The number of values to read
     \return true upon success; false otherwise.
*/
bool BitBuffer::ReadChars (char *buffer, int num_values) {
  //  Cannot read from a closed file handle
  assert (IsClosed () == false);

  if (IsFlushed ()) {
    SetFlushed (false);
  }

  for (int i = 0; i < num_values; i++) {
    buffer[i] = static_cast<char> (ReadBits (8));
  }

  return true;
}


/*!
     Write a specified number of unsigned characters.

     \param[in] buffer The buffer where the values are from
     \param[in] num_values The number of values to write
     \return true upon success; false otherwise.
*/
bool BitBuffer::WriteChars (char *buffer, int num_values) {
  //  Cannot write to a closed file handle
  assert (IsClosed () == false);

  if (IsFlushed ()) {
    SetFlushed (false);
  }

  for (int i = 0; i < num_values; i++) {
    WriteBits (static_cast<unsigned int> (buffer[i]), 8);
  }

  return true;
}


