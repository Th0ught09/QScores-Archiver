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
    \file qscores-single.hpp
    Header file for QScoresSingle class.
*/
/*******************************************************************/

#ifndef QSCORES_SINGLE_HPP
#define QSCORES_SINGLE_HPP


/*!
     \enum e_QSCORES_SINGLE_STATUS
     Status of the private member variables
*/
enum e_QSCORES_SINGLE_STATUS {
  e_QSCORES_SINGLE_STATUS_UNSET,  /*!< Status not yet known  */
  e_QSCORES_SINGLE_STATUS_STR,  /*!< String representation is current  */
  e_QSCORES_SINGLE_STATUS_INT,  /*!< Integer representation is current  */
  e_QSCORES_SINGLE_STATUS_BOTH,  /*!< Both representations are current  */
  e_QSCORES_SINGLE_STATUS_LAST /*!< Last status  */
};


/*!
    \class QScoresSingle

    \details Class used to represent the quality scores for a single read.
*/
class QScoresSingle {
  //  Friend function to print out quality score as a string  [io.cpp]
  friend ostream &operator<< (ostream &os, const QScoresSingle& qs);

  public:
    //  Constructors/destructors  [qscores-single.cpp]
    QScoresSingle ();
    QScoresSingle (string x, bool debug=false);
    QScoresSingle (vector<unsigned int> x, bool debug=false);
    ~QScoresSingle ();

    //  Accessors  [qscores-single.cpp]
    bool GetDebug () const;
    e_QSCORES_SINGLE_STATUS GetStatus () const;
    unsigned int GetMin () const;
    unsigned int GetMax () const;
    unsigned int GetQScoreLength () const;
    unsigned int GetIntLength () const;
    unsigned int GetIntSum ();
    double GetIntAverage ();
    void SetMinMax ();
    
    //  I/O functions  [io.cpp]
    void PrintQScore ();
    vector<unsigned int> GetQScoreInt () const;
    unsigned int GetQScoreIntAsBinary (char* buffer, unsigned int buffer_size);
    
    //  Mapping to/from quality scores  [mapping.cpp]
    void QScoreToInt ();
    void IntToQScore ();
    void StrToInt ();
    void Validate (e_QSCORES_MAP mode);

    //  Lossy transformations  [lossy.cpp]
    void ApplyLossyMinTruncation (unsigned int param);
    void ApplyLossyMaxTruncation (unsigned int param);
    void ApplyLossyRemapping (vector<unsigned int> lookup);
    void UnapplyLossyRemapping (vector<unsigned int> lookup);
    
    //  Lossless transformations  [lossless.cpp]
    unsigned int ApplyDifferenceCoding (unsigned int previous);
    void ApplyRescaling (unsigned int k);
//     void ApplyFinalize ();
    void ApplyLosslessRemapping (vector <unsigned int> lookup);
    unsigned int UnapplyDifferenceCoding (unsigned int previous);
    void UnapplyRescaling (unsigned int k);
//     void UnapplyFinalize ();
    void UnapplyLosslessRemapping (vector <unsigned int> lookup);

    //  Compression functions  [compress.cpp]
    void ApplyCompressionBinary (BitBuffer &bitbuffer, unsigned int param, unsigned int len);
    void ApplyCompressionGamma (BitBuffer &bitbuffer, unsigned int len);
    void ApplyCompressionDelta (BitBuffer &bitbuffer, unsigned int len);
    void ApplyCompressionGolomb (BitBuffer &bitbuffer, unsigned int param, unsigned int len);
    void ApplyCompressionRice (BitBuffer &bitbuffer, unsigned int param, unsigned int len);
    void ApplyCompressionInterP (BitBuffer &bitbuffer, unsigned int len);
    void UnapplyCompressionBinary (BitBuffer &bitbuffer, unsigned int param, unsigned int len);
    void UnapplyCompressionGamma (BitBuffer &bitbuffer, unsigned int len);
    void UnapplyCompressionDelta (BitBuffer &bitbuffer, unsigned int len);
    void UnapplyCompressionGolomb (BitBuffer &bitbuffer, unsigned int param, unsigned int len);
    void UnapplyCompressionRice (BitBuffer &bitbuffer, unsigned int param, unsigned int len);
    void UnapplyCompressionPackedGamma (BitBuffer &bitbuffer, unsigned int low, unsigned int high, unsigned int len);
    void UnapplyCompressionInterP (BitBuffer &bitbuffer, unsigned int len);
  private:
    //  I/O functions  [io.cpp]
    string ConvertUInt (unsigned int num);
    string GetQScoreStr () const;
    string GetQScoreIntAsStr ();

    //!  Debug mode?
    bool m_Debug;
    //!  Status of the private member variables
    e_QSCORES_SINGLE_STATUS m_Status;
    
    //!  Minimum value
    unsigned int m_Min;
    //!  Maximum value
    unsigned int m_Max;
    
    //!  Quality score as a string
    string m_QScoreStr;
    //!  Quality score as a vector of integers
    vector <unsigned int> m_QScoreInt;
};

#endif

