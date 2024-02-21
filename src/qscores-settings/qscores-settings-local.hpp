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
    \file qscores-settings-local.hpp
    Global definitions used only by QScoresSettings.
*/
/*******************************************************************/

#ifndef QSCORES_SETTINGS_LOCAL_HPP
#define QSCORES_SETTINGS_LOCAL_HPP

//!  The maximum parameter to --epbinning; any value up to infinity is in theory possible, but the larger the value, the more time it will take to calculate the lookup table.  Larger values can greatly slow down the computer.
const int g_ERRPROB_BINNING_MAX = 10000000;

//!  Compression method bitmask:  1111 1111 0000 0000
const unsigned int g_COMPRESSION_METHOD_BITMASK = 65280;  

//!  Lossless transformation bitmask:  1111 0000
const unsigned int g_LOSSLESS_TRANSFORM_BITMASK = 240;

//!  Lossy transformation bitmask:  1100
const unsigned int g_LOSSY_TRANSFORM_BITMASK = 12;

//!  Mapping bitmask:  11
const unsigned int g_MAPPING_BITMASK = 3;


/*!
     \enum e_QSCORES_BINARY_SETTINGS
     Position of QScores settings when output in binary format.
     
     Currently takes 16 bits:
       AAAAAAAA BBBB CC DD
       
     A:  Compression method
     B:  Lossless transformation
     C:  Lossy transformations (at most one)
     D:  Quality scores mapping (at most one)
*/
enum e_QSCORES_BINARY_SETTINGS {
  e_QSCORES_BINARY_SETTINGS_UNSET,  /*!< Lower boundary of enumerated type */
  e_QSCORES_BINARY_SETTINGS_MAP_SANGER = 1,  /*!< Sanger mapping */
  e_QSCORES_BINARY_SETTINGS_MAP_SOLEXA = 2,  /*!< Solexa mapping */
  e_QSCORES_BINARY_SETTINGS_MAP_ILLUMINA = 3,  /*!< Illumina mapping */
  e_QSCORES_BINARY_SETTINGS_LOSSY_MAXTRUNC = 4,  /*!< Max truncation */
  e_QSCORES_BINARY_SETTINGS_LOSSY_QSBIN = 8,  /*!< QS-binning lossy transformation */
  e_QSCORES_BINARY_SETTINGS_LOSSY_EPBIN = 12,  /*!< EP-binning lossy transformation */
  e_QSCORES_BINARY_SETTINGS_LOSSLESS_DIFF = 16,  /*!< Difference coding transformation */
  e_QSCORES_BINARY_SETTINGS_LOSSLESS_RESCALING = 32,  /*!< Re-scaling transformation */
  e_QSCORES_BINARY_SETTINGS_LOSSLESS_REMAPPING = 64,  /*!< Frequency-based remapping transformation */
  e_QSCORES_BINARY_SETTINGS_COMP_BINARY = 256,  /*!< Binary compression - 0000 0001 */  
  e_QSCORES_BINARY_SETTINGS_COMP_GAMMA = 512,  /*!< Gamma compression - 0000 0010 */
  e_QSCORES_BINARY_SETTINGS_COMP_DELTA = 768,  /*!< Delta compression - 0000 0011 */
  e_QSCORES_BINARY_SETTINGS_COMP_GOLOMB = 1536,  /*!< Golomb coding - 0000 0110 */
  e_QSCORES_BINARY_SETTINGS_COMP_RICE = 1792,  /*!< Rice coding - 0000 0111 */
  e_QSCORES_BINARY_SETTINGS_COMP_INTERP = 2048,  /*!< Interpolative coding - 0000 1000 */
  e_QSCORES_BINARY_SETTINGS_COMP_HUFFMAN = 8192,  /*!< Huffman coding - 0010 0000 */
  e_QSCORES_BINARY_SETTINGS_COMP_ARITHMETIC = 8448,  /*!< Arithmetic coding - 0010 0001 */  
  e_QSCORES_BINARY_SETTINGS_COMP_GZIP = 16384,  /*!< gzip - 0100 0000 */
  e_QSCORES_BINARY_SETTINGS_COMP_BZIP = 16640,  /*!< bzip2 - 0100 0001 */
  e_QSCORES_BINARY_SETTINGS_COMP_REPAIR = 16896,  /*!< Re-Pair - 0100 0010 */
  e_QSCORES_BINARY_SETTINGS_COMP_NONE = 65024,  /*!< No compression - 1111 1110 */
  e_QSCORES_BINARY_SETTINGS_LAST = 65535  /*!< Upper boundary of enumerated type - 1111 1111 1111 1111 */
};

#endif

