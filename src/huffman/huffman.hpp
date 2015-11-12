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
    \file huffman.hpp
    
    Header file for Huffman class.

    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan

    $Id: huffman.hpp 224 2011-11-06 15:32:09Z rwan $
*/
/*******************************************************************/

#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP


/*!
    \class Huffman

    \details Class used to represent Huffman coding.  Implementation is based on the description in Compression and Coding Algorithms by Moffat and Turpin [2002].  We assume symbols are 1-based (that is, the symbol 0 never occurs so that it can be reserved for other purposes.
    
    Also, the BitBuffer object cannot be made a private member of this class.  The reason is that the BitBuffer object is an abstraction of the low-level file which may be accessed by other functions; so making a copy of it does not make sense.  So, we have to pass it around.
    
    Minor changes have been made, including:
      * Functionality encapsulated within a class which is assumed to be created for each block.  So, re-initialization between blocks is never performed.
      * A final write of a 0 of length m_MaximumCodewordLen to make it easier for the decoder since it can read too many bits.
        [See EncodeFinish ().]
      * m_Table is of size m_MaximumSymbol in both the encoder and decoder when, in fact, it can be the size of the number of
        distinct symbols in the decoder.  Thus, more memory is being used.  [See PreDecodeMessage ().]
      * Made changes to Huffman::PreEncodeMessage () and Huffman::PreDecodeMessage () so that messages made up of an
        alphabet of two symbols can be processed.
      * Made a change to Huffman::CalculateHuffmanCode () so that messages made up of an alphabet of only one symbol
        can be processed (even though a run-length encoding would be preferred).
    
    See the test driver in testing.cpp for example usage.  For encoding, do the following:
    
    1)  Initialize the BitBuffer.
    2)  Create a Huffman object.
    3)  foreach vector, run UpdateFrequencies () to accumulate probabilities.
    4)  Perform block calculations and output the prelude using EncodeBegin ().
    5)  foreach vector, run EncodeMessage ().
    6)  Finalize using EncodeFinish ().
    
    For decoding:
    
    1)  Initialize the BitBuffer.
    2)  Create a Huffman object.
    3)  Decode the prelude with DecodeBegin ().
    4)  Decode a block of symbols using DecodeMessage ().  Setting the block size to the message length just reads it all in one go.
    5)  Finalize using DecodeFinish ()
    
    Additional functions such as SetDebug () and DebugCumulativeSum () are useful for debugging.
    
*/
class Huffman {
  //  Friend function to print out statistics for debugging  [huffman.cpp]
  friend ostream &operator<< (ostream &os, const Huffman& hc);
  
  public:
    //  Constructors/destructors  [huffman.cpp]
    Huffman (bool debug=false);
    ~Huffman ();
    bool GetDebug () const;
    void SetDebug ();
    unsigned int GetMessageLength () const;
    unsigned int GetTableValue (unsigned int pos) const;

    //  Encoding functions  [encode.cpp]    
    void EncodeBegin (BitBuffer &bitbuffer);
    void EncodeMessage (BitBuffer &bitbuffer, vector<unsigned int> x);
    void EncodeFinish (BitBuffer &bitbuffer);

    //  Decoding functions  [decode.cpp]
    void DecodeBegin (BitBuffer &bitbuffer);
    vector<unsigned int> DecodeMessage (BitBuffer &bitbuffer, unsigned int len);
    void DecodeFinish (BitBuffer &bitbuffer);

    //  Main processing functions  [process.cpp]
    void UpdateFrequencies (vector<unsigned int> x);

    //  Debugging functions  [debug.cpp]
    void DebugCumulativeSum ();
  private:
    //  Encoding functions  [encode.cpp]
    void PreEncodeMessage ();
    void EncodePrelude (BitBuffer &bitbuffer);
    void EncodeSymbol (BitBuffer &bitbuffer, unsigned int x);
  
    //  Decoding functions  [decode.cpp]
    void PreDecodeMessage ();
    void DecodePrelude (BitBuffer &bitbuffer);
    unsigned int DecodeSymbol (BitBuffer &bitbuffer);
    
    //  Main processing functions  [process.cpp]
    void CalculateHuffmanCode ();
    void SetWBaseOffset ();

    //  Sorting functions  [sort.cpp]
    void SortSymsUsed_Frequency ();
    void SortSymsUsed_Value ();

    //  Debugging functions  [debug.cpp]
    bool VerifyFrequencies ();

    //!  Debug mode?
    bool m_Debug;
    //!  Length of the message (i.e., number of symbols to encode/decode)
    unsigned int m_MessageLength;
    //!  Length of the message already decoded (must be <= m_MessageLength); not used for encoding
    unsigned int m_MessageLengthDecoded;
    //!  Maximum possible symbol
    unsigned int m_MaximumSymbol;
    //!  Number of distinct values, excluding the sentinel value
    unsigned int m_DistinctSymbols;
    //!  Maximum length of any codeword
    unsigned int m_MaximumCodewordLen;

    //!  Vector of symbols used
    vector<unsigned int> m_SymsUsed;
    //!  Main table used for calculating the Huffman codes
    vector<unsigned int> m_Table;

    //!  The w array (which indicates for each position i, the number of codewords of length i)
    vector<unsigned int> m_W;
    //!  The base array
    vector<unsigned int> m_Base;
    //!  The offset array
    vector<unsigned int> m_Offset;
    //!  ...
    vector<unsigned int> m_LJLimit;

    //!  The input buffer of bits
    unsigned int m_V;
    //!  Number of usable bits in V (i.e., less than or equal to m_MaximumCodewordLen)
    unsigned int m_VBits;
};

#endif

