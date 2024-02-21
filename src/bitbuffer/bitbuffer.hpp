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
    \file bitbuffer.hpp
    Header file for BitBuffer class.
*/
/*******************************************************************/

#ifndef BITBUFFER_HPP
#define BITBUFFER_HPP

/*!
     Size of the main buffer in bytes
*/
const int g_BITBUFFER_SIZE = 131072;

/*!
     \enum e_READWRITE_MODE
     Mode of the BitBuffer (reading or writing).
*/
enum e_READWRITE_MODE { 
  e_MODE_UNSET,  /*!< Read/write mode not yet decided  */
  e_MODE_READ, /*!< Read from file mode  */
  e_MODE_WRITE, /*!< Write to file mode  */ 
  e_MODE_APPEND, /*!< Append to file mode  */
  e_MODE_LAST /*!< Last read/write mode  */
};


// const unsigned int g_MASK_LOWER = ~0;  //  4294967295 (32 1's)


/*!
     Mask for the lower-order byte
*/
const unsigned int g_MASK_LOWER_BYTE = 0xff;


/*! 
    \class BitBuffer

    \details Class used to buffer bits when reading from or writing to disk.
    A two-level buffer is employed -- a smaller one of size unsigned int and
    a larger one which is used to access the disk directly.

    Functions are available which read and write char's and unsigned int's.
    Unfortunately, they are not very efficient since they also access the bit
    buffers.  A more efficient approach would have them access the disk directly.
    But this would require more work to ensure that the bit buffer remains in an
    consistent state.

    Perhaps this will be for "future work".
*/
class BitBuffer {
  public:
    //  Constructors, destructors, and initializers  [bitbuffer.cpp]
    BitBuffer ();
    ~BitBuffer ();
    void Initialize (std::string fn, e_READWRITE_MODE mode, bool debug=false);

    //  Accessors/mutators  [bitbuffer.cpp]
    std::string GetFilename () const;
    enum e_READWRITE_MODE GetMode () const;
    bool GetDebug () const;
    void SetDebug (bool value);
    void SetFlushed (bool value);
    void SetClosed (bool value);

    //  Main functions  [io.cpp]
    unsigned int ReadBits (unsigned int num_bits);
    void WriteBits (unsigned int x, unsigned int bits);
    bool ReadUInts (unsigned int *buffer, int num_values);
    bool WriteUInts (unsigned int *buffer, int num_values);
    bool ReadChars (char *buffer, int num_values);
    bool WriteChars (char *buffer, int num_values);

    //  Finalizing functions  [finish.cpp]
    void Flush ();
    void Close ();
    void Finish ();
  private:
    //  Main functions  [io.cpp]
    bool ReadBitsLowLevel (unsigned int min_bits);

    //  Finalizing functions  [finish.cpp]
    bool IsFlushed ();
    bool FlushRead ();
    bool FlushWrite ();
    bool IsClosed ();
    bool CloseRead ();
    bool CloseWrite ();

    //!  Debug mode?
    bool m_Debug;
    //!  Input/output filename
    string m_Filename;
    //!  Input file pointer
    ifstream m_In_Fp;
    //!  Output file pointer
    ofstream m_Out_Fp;
    //!  Mode (reading or writing)
    enum e_READWRITE_MODE m_Mode;
    //!  Indicate whether the file pointer has been flushed
    bool m_Flushed;
    //!  Indicate whether the file pointer has been closed
    bool m_Closed;
    
    //!  Mini buffer of 4 bytes long
    unsigned int m_Mini_Buffer;
    //!  Number of useful bits in the mini-buffer
    unsigned int m_Mini_Buffer_Used;

    //!  Main buffer
    char *m_Main_Buffer;
    //!  Pointer to next available position in the buffer
    int m_Main_Buffer_Ptr;
    //!  Pointer to the end of the buffer; in the end, it should be less than BITBUFFER_SIZE because it will not be full
    int m_Main_Buffer_End;
};

#endif

