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
    \file qscores-settings.hpp
    Header file for QScoresSettings class.
*/
/*******************************************************************/

#ifndef QSCORES_SETTINGS_HPP
#define QSCORES_SETTINGS_HPP

/*!
    \class QScoresSettings

    \details Class used to represent the settings provided by the user or
    the encoded data file.
*/
class QScoresSettings {
  friend ostream &operator<< (ostream &os, const QScoresSettings& qs);

  public:
    //  Constructors/destructors  [qscores-settings.cpp]
    QScoresSettings ();
    ~QScoresSettings ();
    bool CheckFileSettings () const;
    bool CheckLossySettings () const;
    bool CheckLosslessSettings () const;
    bool CheckCompressionSettings () const;    
    bool CheckSettings () const;
    bool WriteBinarySettings (BitBuffer &bitbuffer);
    bool ReadBinarySettings (BitBuffer &bitbuffer);

    //  Accessors  [accessors.cpp]
    bool GetDebug () const;
    std::string GetInputFn () const;
    std::string GetOutputFn () const;
    enum e_QSCORES_MAP GetQScoresMapping () const;
    string GetQScoresMappingStr () const;
    int GetQScoresMappingMin () const;
    int GetQScoresMappingMax () const;
    int GetQScoresMappingRange () const;

    //  Lossless transformations
    bool GetTransformGapTrans () const;
    bool GetTransformMinShift () const;
    bool GetTransformFreqOrder () const;

    //  Lossy transformations
    bool GetLossyMinTruncation () const;
    bool GetLossyMaxTruncation () const;
    bool GetLossyLogBinning () const;
    bool GetLossyUniBinning () const;
    int GetLossyMinTruncationParameter () const;
    int GetLossyMaxTruncationParameter () const;
    int GetLossyLogBinningParameter () const;
    int GetLossyUniBinningParameter () const;

    //  Compression
    bool GetCompressionBinary () const;
    bool GetCompressionGamma () const;
    bool GetCompressionDelta () const;
    bool GetCompressionInterP () const;
    bool GetCompressionGolomb () const;
    bool GetCompressionRice () const;
    bool GetCompressionHuffman () const;
    bool GetCompressionArithmetic () const;
    bool GetCompressionGzip () const;
    bool GetCompressionBzip () const;
    bool GetCompressionRepair () const;
    bool GetCompressionPPM () const;
    bool GetCompressionNone () const;

    //  Compression parameters
    unsigned int GetCompressionGlobalParameter () const;

    //  Mutators  [mutators.cpp]
    void SetInputFn (string x);
    void SetOutputFn (string x);
    void SetDebug ();
    bool SetQScoresMapping (string x);

    //  Lossless transformations
    void SetTransformGapTrans ();
    void SetTransformMinShift ();
    void SetTransformFreqOrder ();
    
    //  Lossy transformations
    void SetLossyMinTruncation ();
    void SetLossyMaxTruncation ();
    void SetLossyLogBinning ();
    void SetLossyUniBinning ();
    void SetLossyMinTruncationParameter (int param);
    void SetLossyMaxTruncationParameter (int param);
    void SetLossyLogBinningParameter (int param);
    void SetLossyUniBinningParameter (int param);

    //  Compression
    void SetCompressionBinary ();
    void SetCompressionGamma ();
    void SetCompressionDelta ();
    void SetCompressionInterP ();
    void SetCompressionGolomb ();
    void SetCompressionRice ();
    void SetCompressionHuffman ();
    void SetCompressionArithmetic ();
    void SetCompressionGzip ();
    void SetCompressionBzip ();
    void SetCompressionRepair ();
    void SetCompressionPPM ();
    void SetCompressionNone ();
    
    //  Compression parameters
    void SetCompressionGlobalParameter (unsigned int x);
  private:
    //!  Debug mode?
    bool m_Debug;
    //!  Input filename
    string m_InputFn;
    //!  Output filename
    string m_OutputFn;

    //!  QScores mapping used
    enum e_QSCORES_MAP m_Mapping;

    //!  Lossy transformation -- truncate values to a minimum?
    bool m_LossyMinTruncation;
    //!  Lossy transformation -- truncate values to a maximum?
    bool m_LossyMaxTruncation;
    //!  Lossy transformation -- apply equally-sized binning of quality scores?
    bool m_LossyLogBinning;
    //!  Lossy transformation -- apply equally-sized binning of probabilities?
    bool m_LossyUniBinning;
    //!  Lossy transformation -- minimum to truncate values to
    int m_LossyMinTruncationParameter;
    //!  Lossy transformation -- maximum to truncate values to
    int m_LossyMaxTruncationParameter;
    //!  Lossy transformation -- equally-sized binning of quality scores
    int m_LossyLogBinningParameter;
    //!  Lossy transformation -- equally-sized binning of probabilities
    int m_LossyUniBinningParameter;
    
    //!  Lossless transformation -- gap transforming?
    bool m_TransformGapTrans;
    //!  Lossless transformation -- minshifting?
    bool m_TransformMinShift;
    //!  Lossless transformation -- frequency-based alphabet reordering?
    bool m_TransformFreqOrder;

    //!  Compression -- Binary coding?
    bool m_CompressionBinary;
    //!  Compression -- Gamma coding?
    bool m_CompressionGamma;
    //!  Compression -- Delta coding?
    bool m_CompressionDelta;
    //!  Compression -- Golomb coding?
    bool m_CompressionGolomb;
    //!  Compression -- Rice coding?
    bool m_CompressionRice;
    //!  Compression -- Interpolative coding?
    bool m_CompressionInterP;
    
    //!  Compression -- Global parameter for Golomb/Rice coding; not encoded in the main header and unnecessary for decoding
    unsigned int m_CompressionGlobalParameter;
    
    //!  Compression -- Huffman coding?
    bool m_CompressionHuffman;
    //!  Compression -- Arithmetic coding?
    bool m_CompressionArithmetic;
    //!  Compression -- gzip?
    bool m_CompressionGzip;
    //!  Compression -- bzip?
    bool m_CompressionBzip;
    //!  Compression -- Re-Pair?
    bool m_CompressionRepair;
    //!  Compression -- PPM?
    bool m_CompressionPPM;
    //!  Compression -- None?
    bool m_CompressionNone;
};

#endif

