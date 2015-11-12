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
    \file gamma.hpp
    Header file for gamma encoding and decoding functions.
    Gamma encoding uses [unary, binary].  More specifically, for some
    number x (x > 0), it is encoded as "1 + floorLog (x)" in unary and
    "x - 2^(floorLog (x))" in binary.

    See Managing Gigabytes, second edition, pages 117-118 for further details.
    Note that our use of unary coding differs; see unary.hpp for more
    information.
    
    \author Raymond Wan (r-wan@cb.k.u-tokyo.ac.jp)
    \par Organizations
          - Department of Computational Biology, Graduate School of
            Frontier Science, University of Tokyo
          - Computational Biology Research Center, AIST, Japan
          
    $Id: gamma.hpp 224 2011-11-06 15:32:09Z rwan $

*/
/*******************************************************************/


#ifndef GAMMA_HPP
#define GAMMA_HPP

//  Encoding functions
unsigned int Gamma_Encode (BitBuffer &bitbuffer, unsigned int value);
unsigned int GammaLow_Encode (BitBuffer &bitbuffer, unsigned int value, unsigned int low);
unsigned int GammaLowHigh_Encode (BitBuffer &bitbuffer, unsigned int value, unsigned int low, unsigned int high);

//  Decoding functions
unsigned int Gamma_Decode (BitBuffer &bitbuffer);
unsigned int GammaLow_Decode (BitBuffer &bitbuffer, unsigned int low);
unsigned int GammaLowHigh_Decode (BitBuffer &bitbuffer, unsigned int low, unsigned int high);

#endif

