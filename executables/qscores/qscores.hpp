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
    \file qscores-set.hpp
    Header file for QScores class.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: qscores.hpp 224 2011-11-06 15:32:09Z rwan $
*/
/*******************************************************************/

#ifndef QSCORES_HPP
#define QSCORES_HPP

/*!
    \class QScores

    \details Class used to encompass a set of quality scores.
*/
class QScores {
  public:
    //  Constructors/destructors  [qscores.cpp]
    QScores ();
    ~QScores ();
    bool Initialize ();
    
    //  Execution  [run.cpp]
    bool Run ();

    //  Parameter checking  [parameters.cpp]
    bool ProcessOptions (int argc, char *argv[]);
    bool CheckSettings ();

    //  Reading in data  [io.cpp]
    bool OpenFiles ();
    bool CloseFiles ();
    int ReadInFileBlock (int blocksize);
    void WriteOutFileBlock ();
    
    //  Block transformation functions  [transform.cpp]
    void PerformBinningCheck ();
    void PerformUnbinningCheck ();
    void PreprocessBlock (int current_blocksize);
    void UnPreprocessBlock (int current_blocksize);

    //  Block encoding functions  [encode.cpp]
    void EncodeEOF ();
    void EncodeHeaderBlock (int current_blocksize, int block_count);
    void EncodeStaticCodesBlock (int current_blocksize);
    void EncodeHuffmanBlock (int current_blocksize);
    void EncodeExternalBlock (int current_blocksize);
    void EncodeIntToQScore (int current_blocksize);

    //  Block decoding functions  [decode.cpp]
    int DecodeHeaderBlock (int block_count);
    void DecodeStaticCodesBlock (int current_blocksize);
    void DecodeHuffmanBlock (int current_blocksize);
    void DecodeExternalBlock (int current_blocksize);

    //  External compression software [external.cpp]
    void PerformExternalSoftwareCheck ();
    
    //  Accessors  [accessors.cpp]
    bool GetDebug () const;
    bool GetVerbose () const;
    bool GetEncode () const;
    bool GetDecode () const;
    bool GetExternalSoftwareCheck () const;
    bool GetBinningCheck () const;
    bool GetUnbinningCheck () const;
    enum e_QSCORES_MAP GetQScoresMapping () const;
    string GetQScoresMappingStr () const;
    int GetBlocksize () const;
    
    //  Mutators  [mutators.cpp]
    void SetDebug ();
    void SetVerbose ();
    void SetEncode ();
    void SetDecode ();
    void SetExternalSoftwareCheck ();
    void SetBinningCheck ();
    void SetUnbinningCheck ();
    void SetQScoresMapping (string x);
    void SetBlocksize (int x);
  private:
    //!  Debug mode?
    bool m_Debug;
    //!  Verbose mode?
    bool m_Verbose;
    //!  Encoding mode?
    bool m_Encode;
    //!  Decoding mode?
    bool m_Decode;
    //!  Perform a check of external software?
    bool m_ExternalSoftwareCheck;
    //!  Output the bins created from lossy binning
    bool m_BinningCheck;
    //!  Output the reverse bins created from lossy binning
    bool m_UnbinningCheck;
    
    //!  Input bitbuffer
    BitBuffer m_BitBuff_In;
    //!  Output bitbuffer
    BitBuffer m_BitBuff_Out;
    //!  Input file pointer for text
    ifstream m_Text_In;
    //!  Output file pointer for text
    ofstream m_Text_Out;
    
    //!  Settings to the program
    QScoresSettings m_QScoresSettings;

    //!  Management of external software
    ExternalSoftware m_ExternalSoftware;
    
    //!  Vector of quality score objects
    vector <QScoresSingle> m_Qscores;

    //!  Read length for the entire data file
    unsigned int m_FileReadLength;
    //!  Block size fo the entire file
    int m_FileBlockSize;
    
    //  Variables related to the current block; need to make them into a vector of values
    //  to make this work for multiple processors
    //!  Read length for the block; encoded as 0 if same as m_FileReadLength for this block
    unsigned int m_BlockReadLength;
    //!  Block size
    int m_Blocksize;
    //!  Minimum for the current block
    unsigned int m_BlockMinimum;
    //!  Statistics for the current block
    BlockStatistics m_BlockStatistics;
    //!  Parameter to be used for some coding schemes
    unsigned int m_CompressionParameter;
    //!  Second parameter to be used for some coding schemes
    unsigned int m_CompressionParameter_2;
};

#endif

